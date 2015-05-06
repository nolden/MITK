/*===================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center,
Division of Medical and Biological Informatics.
All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.

See LICENSE.txt or http://www.mitk.org for details.

===================================================================*/

#ifndef QmitkNormalizedMutualInformationHistogramMetricViewWidgetHIncluded
#define QmitkNormalizedMutualInformationHistogramMetricViewWidgetHIncluded

#include "ui_QmitkNormalizedMutualInformationHistogramMetricControls.h"
#include "MitkRigidRegistrationUIExports.h"
#include "QmitkRigidRegistrationMetricsGUIBase.h"
#include <itkArray.h>
#include <itkObject.h>
#include <itkImage.h>

/*!
* \brief Widget for rigid registration
*
* Displays options for rigid registration.
*/
class MITKRIGIDREGISTRATIONUI_EXPORT QmitkNormalizedMutualInformationHistogramMetricView : public QmitkRigidRegistrationMetricsGUIBase
{

public:

  QmitkNormalizedMutualInformationHistogramMetricView( QWidget* parent = nullptr, Qt::WindowFlags f = nullptr );
  ~QmitkNormalizedMutualInformationHistogramMetricView();

  virtual mitk::MetricParameters::MetricType GetMetricType() override;

  virtual itk::Object::Pointer GetMetric() override;

  virtual itk::Array<double> GetMetricParameters() override;

  virtual void SetMetricParameters(itk::Array<double> metricValues) override;

  virtual QString GetName() override;

  virtual void SetupUI(QWidget* parent) override;

  virtual bool Maximize() override;


private:

  template < class TPixelType, unsigned int VImageDimension >
  itk::Object::Pointer GetMetric2(itk::Image<TPixelType, VImageDimension>* itkImage1);

protected:

  Ui::QmitkNormalizedMutualInformationHistogramMetricControls m_Controls;

  itk::Object::Pointer m_MetricObject;

};

#endif
