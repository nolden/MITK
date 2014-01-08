/*===================================================================
=======
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the demonstration applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center,
Division of Medical and Biological Informatics.
All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.

See LICENSE.txt or http://www.mitk.org for details.

===================================================================*/

#include <QQmlEngine>
#include "QmlMitkRenderWindowItem.h"

#include <vtkOpenGLExtensionManager.h>

#include <QVTKInteractor.h>
#include<vtkEventQtSlotConnect.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <QVTKInteractorAdapter.h>

// MITK event types
#include "mitkMousePressEvent.h"
#include "mitkMouseMoveEvent.h"
#include "mitkMouseDoubleClickEvent.h"
#include "mitkMouseReleaseEvent.h"
#include "mitkInteractionKeyEvent.h"
#include "mitkMouseWheelEvent.h"
#include "mitkInternalEvent.h"
#include "mitkPlaneGeometryDataMapper2D.h"

#include "QmlMitkBigRenderLock.h"


QmlMitkRenderWindowItem* QmlMitkRenderWindowItem::GetInstanceForVTKRenderWindow( vtkRenderWindow* rw )
{
  return GetInstances()[rw];
}

QMap<vtkRenderWindow*, QmlMitkRenderWindowItem*>& QmlMitkRenderWindowItem::GetInstances()
{
  static QMap<vtkRenderWindow*, QmlMitkRenderWindowItem*> s_Instances;
  return s_Instances;
}


QmlMitkRenderWindowItem
::QmlMitkRenderWindowItem(QQuickItem* parent,
                          const QString& name,
                          mitk::VtkPropRenderer* /*renderer*/,
                          mitk::RenderingManager* renderingManager)
: QVTKQuickItem(parent),m_PlaneNodeColorObserver(0)
{
  mitk::RenderWindowBase::Initialize( renderingManager, name.toStdString().c_str() );

  /* from QmitkRenderWindow. Required?
  setFocusPolicy(Qt::StrongFocus);
  setMouseTracking(true);
  */

  GetInstances()[QVTKQuickItem::GetRenderWindow()] = this;

  QQmlEngine engine;
  QQmlComponent component(&engine);
  component.setData("import QtQuick 2.0 \nRectangle { objectName: \"colorFrame\" \n color: Qt.rgba(1, 1, 1, 0.0) ; \n radius: 1; \n border.width: 3; border.color: 'magenta';  anchors.fill: parent;}",QUrl());
  std::cout << qPrintable( component.errorString() );
  QQuickItem *item = qobject_cast<QQuickItem *>(component.create());
  //this->property("data").toList().append();
  item->setParentItem(this);
  item->setParent(this);
  // QObject *rectangleInstance = component.create();
}

// called from QVTKQuickItem when window is painted for the first time!
void QmlMitkRenderWindowItem::init()
{
  QVTKQuickItem::init();

  mitk::DataStorage::Pointer m_DataStorage = mitk::RenderWindowBase::GetRenderer()->GetDataStorage();
  if (m_DataStorage.IsNotNull())
  {
    mitk::RenderingManager::GetInstance()->InitializeViews( m_DataStorage->ComputeBoundingGeometry3D(m_DataStorage->GetAll()) );
  }


  if ( mitk::RenderWindowBase::GetRenderer()->GetMapperID() == mitk::BaseRenderer::Standard2D
      && m_DataStorage.IsNotNull() )
  {

    mitk::DataNode::Pointer planeNode = mitk::RenderWindowBase::GetRenderer()->GetCurrentWorldGeometry2DNode();
    switch ( mitk::RenderWindowBase::GetRenderer()->GetSliceNavigationController()->GetDefaultViewDirection() )
    {
      case mitk::SliceNavigationController::Axial:
        planeNode->SetColor(1.0,0.0,0.0);
        break;
      case mitk::SliceNavigationController::Sagittal:
        planeNode->SetColor(0.0,1.0,0.0);
        break;
      case mitk::SliceNavigationController::Frontal:
        planeNode->SetColor(0.0,0.0,1.0);
        break;
      default:
        planeNode->SetColor(1.0,1.0,0.0);
    }

    planeNode->SetProperty("layer", mitk::IntProperty::New(1000) );
    planeNode->SetProperty("visible", mitk::BoolProperty::New(true) );
    planeNode->SetProperty("helper object", mitk::BoolProperty::New(true) );

    mitk::PlaneGeometryDataMapper2D::Pointer mapper = mitk::PlaneGeometryDataMapper2D::New();
    planeNode->SetMapper( mitk::BaseRenderer::Standard2D, mapper );

    m_DataStorage->Add( planeNode );
    itk::SimpleMemberCommand<QmlMitkRenderWindowItem>::Pointer cmd = itk::SimpleMemberCommand<QmlMitkRenderWindowItem>::New();
    cmd->SetCallbackFunction(this,&QmlMitkRenderWindowItem::UpdateFrameColor);
    m_PlaneNodeColorObserver = planeNode->GetProperty("color")->AddObserver(itk::ModifiedEvent(),cmd);
  }
}

void QmlMitkRenderWindowItem::InitView( mitk::BaseRenderer::MapperSlotId mapperID,
                                        mitk::SliceNavigationController::ViewDirection viewDirection )
{
  m_MapperID = mapperID;
  m_ViewDirection = viewDirection;
}


void QmlMitkRenderWindowItem::SetDataStorage(mitk::DataStorage::Pointer storage)
{
  m_DataStorage = storage;
}

mitk::Point2D QmlMitkRenderWindowItem::GetMousePosition(QMouseEvent* me) const
{

  mitk::Point2D point;
  point[0] = me->x();
  point[1] = me->y();
  return point;
}

mitk::Point2D QmlMitkRenderWindowItem::GetMousePosition(QWheelEvent* we) const
{
  mitk::Point2D point;
  point[0] = we->x();
  point[1] = we->y();
  return point;
}

mitk::InteractionEvent::MouseButtons QmlMitkRenderWindowItem::GetEventButton(QMouseEvent* me) const
{
  mitk::InteractionEvent::MouseButtons eventButton;
  switch (me->button())
  {
  case Qt::LeftButton:
    eventButton = mitk::InteractionEvent::LeftMouseButton;
    break;
  case Qt::RightButton:
    eventButton = mitk::InteractionEvent::RightMouseButton;
    break;
  case Qt::MidButton:
    eventButton = mitk::InteractionEvent::MiddleMouseButton;
    break;
  default:
    eventButton = mitk::InteractionEvent::NoButton;
    break;
  }
  return eventButton;
}

mitk::InteractionEvent::MouseButtons QmlMitkRenderWindowItem::GetButtonState(QMouseEvent* me) const
{
  mitk::InteractionEvent::MouseButtons buttonState = mitk::InteractionEvent::NoButton;

  if (me->buttons() & Qt::LeftButton)
  {
    buttonState = buttonState | mitk::InteractionEvent::LeftMouseButton;
  }
  if (me->buttons() & Qt::RightButton)
  {
    buttonState = buttonState | mitk::InteractionEvent::RightMouseButton;
  }
  if (me->buttons() & Qt::MidButton)
  {
    buttonState = buttonState | mitk::InteractionEvent::MiddleMouseButton;
  }
  return buttonState;
}

mitk::InteractionEvent::ModifierKeys QmlMitkRenderWindowItem::GetModifiers(QInputEvent* me) const
{
  mitk::InteractionEvent::ModifierKeys modifiers = mitk::InteractionEvent::NoKey;

  if (me->modifiers() & Qt::ALT)
  {
    modifiers = modifiers | mitk::InteractionEvent::AltKey;
  }
  if (me->modifiers() & Qt::CTRL)
  {
    modifiers = modifiers | mitk::InteractionEvent::ControlKey;
  }
  if (me->modifiers() & Qt::SHIFT)
  {
    modifiers = modifiers | mitk::InteractionEvent::ShiftKey;
  }
  return modifiers;
}

mitk::InteractionEvent::MouseButtons QmlMitkRenderWindowItem::GetButtonState(QWheelEvent* we) const
{
  mitk::InteractionEvent::MouseButtons buttonState = mitk::InteractionEvent::NoButton;

  if (we->buttons() & Qt::LeftButton)
  {
    buttonState = buttonState | mitk::InteractionEvent::LeftMouseButton;
  }
  if (we->buttons() & Qt::RightButton)
  {
    buttonState = buttonState | mitk::InteractionEvent::RightMouseButton;
  }
  if (we->buttons() & Qt::MidButton)
  {
    buttonState = buttonState | mitk::InteractionEvent::MiddleMouseButton;
  }
  return buttonState;
}


void QmlMitkRenderWindowItem::mousePressEvent(QMouseEvent* me)
{
  mitk::Point2D mousePosition = GetMousePosition(me);
  mitk::MousePressEvent::Pointer mPressEvent =
    mitk::MousePressEvent::New(mitk::RenderWindowBase::GetRenderer(), mousePosition,  GetButtonState(me), GetModifiers(me), GetEventButton(me));

  mitk::RenderWindowBase::HandleEvent(mPressEvent.GetPointer());

  QVTKQuickItem::mousePressEvent(me);

//  if (m_ResendQtEvents)
//    me->ignore();
}

void QmlMitkRenderWindowItem::mouseReleaseEvent(QMouseEvent* me)
{
  mitk::Point2D mousePosition = GetMousePosition(me);

  mitk::MouseReleaseEvent::Pointer mReleaseEvent =
    mitk::MouseReleaseEvent::New(mitk::RenderWindowBase::GetRenderer(), mousePosition, GetButtonState(me), GetModifiers(me), GetEventButton(me));

  mitk::RenderWindowBase::HandleEvent(mReleaseEvent.GetPointer());


  QVTKQuickItem::mouseReleaseEvent(me);

//  if (m_ResendQtEvents)
//    me->ignore();
}

void QmlMitkRenderWindowItem::mouseMoveEvent(QMouseEvent* me)
{
  mitk::Point2D mousePosition = GetMousePosition(me);
  mitk::MouseMoveEvent::Pointer mMoveEvent =
    mitk::MouseMoveEvent::New(mitk::RenderWindowBase::GetRenderer(), mousePosition, GetButtonState(me), GetModifiers(me));

  mitk::RenderWindowBase::HandleEvent(mMoveEvent.GetPointer());


  QVTKQuickItem::mouseMoveEvent(me);

// TODO: why was this not put here initially? What is special about mouse move?
//  if (m_ResendQtEvents)
//    me->ignore();
}

void QmlMitkRenderWindowItem::wheelEvent(QWheelEvent *we)
{
  mitk::Point2D mousePosition = GetMousePosition(we);
  mitk::MouseWheelEvent::Pointer mWheelEvent =
    mitk::MouseWheelEvent::New(mitk::RenderWindowBase::GetRenderer(), mousePosition, GetButtonState(we), GetModifiers(we), we->delta());

  mitk::RenderWindowBase::HandleEvent(mWheelEvent.GetPointer());


  QVTKQuickItem::wheelEvent(we);

//  if (m_ResendQtEvents)
  //    we->ignore();
}

void QmlMitkRenderWindowItem::UpdateFrameColor()
{
  mitk::DataNode::Pointer planeNode = mitk::RenderWindowBase::GetRenderer()->GetCurrentWorldGeometry2DNode();
  float rgb[3];
  planeNode->GetColor(rgb);
  SetFrameColor(QColor(rgb[0]*255,rgb[1]*255,rgb[2]*255));
}


void QmlMitkRenderWindowItem::prepareForRender()
{
//  Adjust camera is kaputt wenn nicht der renderingmanager dem vtkprop bescheid sagt!
//  this is just a workaround
  QmlMitkBigRenderLock::GetMutex().lock();
  mitk::RenderWindowBase::GetRenderer()->ForceImmediateUpdate();
}

void QmlMitkRenderWindowItem::cleanupAfterRender()
{
  QmlMitkBigRenderLock::GetMutex().unlock();
}

void QmlMitkRenderWindowItem::SetCrossHairPositioningOnClick(bool enabled)
{
  if (enabled)
  {
    mitk::GlobalInteraction::GetInstance()->AddListener( mitk::RenderWindowBase::GetSliceNavigationController() );
  }
  else
  {
    mitk::GlobalInteraction::GetInstance()->RemoveListener( mitk::RenderWindowBase::GetSliceNavigationController() );
  }
}

void QmlMitkRenderWindowItem::SetFrameColor(QColor frameColor)
{
  if (frameColor != m_FrameColor)
  {
    this->m_FrameColor = frameColor;

    QQuickItem* frame = this->findChild<QQuickItem*>("colorFrame");
    if (frame)
    {
      QQmlProperty::write( frame, "border.color", frameColor );
    }
    else
    {
      MITK_ERROR << "No frame child!";
    }

    mitk::DataNode::Pointer planeNode = mitk::RenderWindowBase::GetRenderer()->GetCurrentWorldGeometry2DNode();
    planeNode->SetColor(frameColor.redF(),frameColor.greenF(),frameColor.blueF());
    emit FrameColorChanged(frameColor);
  }
}

QColor QmlMitkRenderWindowItem::GetFrameColor()
{
  return this->m_FrameColor;
}

void QmlMitkRenderWindowItem::SetPlaneNodeParent( mitk::DataNode::Pointer node )
{
  m_PlaneNodeParent = node;
}

void QmlMitkRenderWindowItem::SetReferenceLineVisibility(mitk::BaseRenderer *renderer, bool visible)
{

}

void QmlMitkRenderWindowItem::geometryChanged(const QRectF & newGeometry, const QRectF & oldGeometry)
{
  QVTKQuickItem::geometryChanged(newGeometry, oldGeometry);
}

vtkRenderWindow* QmlMitkRenderWindowItem::GetVtkRenderWindow()
{
  return QVTKQuickItem::GetRenderWindow();
}

vtkRenderWindowInteractor* QmlMitkRenderWindowItem::GetVtkRenderWindowInteractor()
{
  return QVTKQuickItem::GetInteractor();
}


