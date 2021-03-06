/*============================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center (DKFZ)
All rights reserved.

Use of this source code is governed by a 3-clause BSD license that can be
found in the LICENSE file.

============================================================================*/

#include "CustomViewer.h"
#include "CustomViewerWorkbenchAdvisor.h"

#include <berryPlatformUI.h>

CustomViewer::CustomViewer()
{
}

CustomViewer::~CustomViewer()
{
}
//! [MinimalApplicationClass_StartMethod]
int CustomViewer::Start()
{
  berry::Display *display = berry::PlatformUI::CreateDisplay();

  wbAdvisor.reset(new berry::WorkbenchAdvisor);
  int code = berry::PlatformUI::CreateAndRunWorkbench(display, wbAdvisor.data());

  // exit the application with an appropriate return code
  return code == berry::PlatformUI::RETURN_RESTART ? EXIT_RESTART : EXIT_OK;
}
//! [MinimalApplicationClass_StartMethod]
void CustomViewer::Stop()
{
}
