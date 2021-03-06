/*============================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center (DKFZ)
All rights reserved.

Use of this source code is governed by a 3-clause BSD license that can be
found in the LICENSE file.

============================================================================*/

#ifndef MITKMESHVTKMAPPER3D_H_HEADER_INCLUDED
#define MITKMESHVTKMAPPER3D_H_HEADER_INCLUDED

#include "MitkMapperExtExports.h"
#include "mitkBaseRenderer.h"
#include "mitkCommon.h"
#include "mitkMesh.h"
#include "mitkVtkMapper.h"

#include <vtkAppendPolyData.h>
#include <vtkCellArray.h>
#include <vtkFloatArray.h>
#include <vtkLinearTransform.h>
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkSphereSource.h>
#include <vtkTextSource.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkTubeFilter.h>
#include <vtkVectorText.h>

class vtkActor;
class vtkAssembly;
class vtkFollower;
class vtkPolyDataMapper;
class vtkPropAssembly;

namespace mitk
{
  /**
   * \brief Vtk-based mapper for PointList
   * \ingroup Mapper
   */
  class MITKMAPPEREXT_EXPORT MeshVtkMapper3D : public VtkMapper
  {
  public:
    mitkClassMacro(MeshVtkMapper3D, VtkMapper);

    itkFactorylessNewMacro(Self) itkCloneMacro(Self)

      virtual const mitk::Mesh *GetInput();

    vtkProp *GetVtkProp(mitk::BaseRenderer *renderer) override;
    void UpdateVtkTransform(mitk::BaseRenderer *renderer) override;

    LocalStorageHandler<BaseLocalStorage> m_LSH;

  protected:
    MeshVtkMapper3D();

    ~MeshVtkMapper3D() override;

    void GenerateDataForRenderer(mitk::BaseRenderer *renderer) override;

    void ResetMapper(BaseRenderer *renderer) override;

    vtkPropAssembly *m_PropAssembly;

    vtkActor *m_SpheresActor;
    vtkActor *m_ContourActor;
    vtkPolyDataMapper *m_ContourMapper;
    vtkPolyDataMapper *m_SpheresMapper;

    vtkPolyDataMapper *m_TextVtkPolyDataMapper;

    vtkAppendPolyData *m_Spheres;
    vtkPolyData *m_Contour;
  };

} // namespace mitk

#endif /* MITKMESHVTKMAPPER3D_H_HEADER_INCLUDED*/
