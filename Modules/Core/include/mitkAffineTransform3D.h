/*============================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center (DKFZ)
All rights reserved.

Use of this source code is governed by a 3-clause BSD license that can be
found in the LICENSE file.

============================================================================*/

#ifndef MITKAFFINETRANSFORM3D_H_
#define MITKAFFINETRANSFORM3D_H_

#include "itkScalableAffineTransform.h"
#include <mitkVector.h>

namespace mitk
{
  typedef itk::ScalableAffineTransform<ScalarType, 3> AffineTransform3D;
}

#endif /* MITKAFFINETRANSFORM3D_H_ */
