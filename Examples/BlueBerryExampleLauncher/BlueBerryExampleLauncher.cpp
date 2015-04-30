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

#include <mitkBaseApplication.h>

#include "BlueBerryExampleLauncherDialog.h"


int main(int argc, char** argv)
{
  mitk::BaseApplication app(argc, argv);
  app.setApplicationName("BlueBerryExampleLauncher");
  app.setOrganizationName("DKFZ");
  app.initializeQt();

  BlueBerryExampleLauncherDialog demoDialog;
  QString selectedConfiguration = demoDialog.getDemoConfiguration();

  if (selectedConfiguration.isEmpty()) return EXIT_SUCCESS;

  app.setProvisioningFilePath(selectedConfiguration);
  return app.run();
}
