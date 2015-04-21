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

#include <ctkPluginFrameworkLauncher.h>

int main(int argc, char** argv)
{
  mitk::BaseApplication app(argc, argv);

  app.setApplicationName("MitkCoreApp");
  app.setOrganizationName("DKFZ");

  // These paths replace the .ini file and are tailored for installation
  // packages created with CPack. If a .ini file is presented, it will
  // overwrite the settings
  //QDir basePath(QCoreApplication::applicationDirPath());

  //QString provFile = basePath.absoluteFilePath("MitkCoreApp.provisioning");

  //Poco::Util::MapConfiguration* coreConfig(new Poco::Util::MapConfiguration());
  //coreConfig->setString(berry::Platform::ARG_PROVISIONING.toStdString(), provFile.toStdString());
  //coreConfig->setString(berry::Platform::ARG_APPLICATION.toStdString(), "org.mitk.qt.coreapplication");
  app.setProperty("blueberry.application", "org.mitk.qt.coreapplication");

  // Preload the org.mitk.gui.qt.common plug-in (and hence also Qmitk) to speed
  // up a clean-cache start. This also works around bugs in older gcc and glibc implementations,
  // which have difficulties with multiple dynamic opening and closing of shared libraries with
  // many global static initializers. It also helps if dependent libraries have weird static
  // initialization methods and/or missing de-initialization code.
  //coreConfig->setString(berry::Platform::ARG_PRELOAD_LIBRARY.toStdString(), "liborg_mitk_gui_qt_common");

  return app.run();
}
