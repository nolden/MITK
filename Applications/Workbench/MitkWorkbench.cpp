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

#include <QtGlobal>
#include <QString>
#include <QMap>
#include <QHash>

#include <usModuleSettings.h>

#include <mitkCommon.h>
#include <mitkException.h>


int main(int argc, char** argv)
{
  // Create a QApplication instance first

  mitk::BaseApplication app(argc, argv);

  app.setSingleMode(true);
  app.setApplicationName("MITK Workbench");
  app.setOrganizationName("DKFZ");


  // These paths replace the .ini file and are tailored for installation
  // packages created with CPack. If a .ini file is presented, it will
  // overwrite the settings in MapConfiguration
  QDir basePath(QCoreApplication::applicationDirPath());

  QString provFile = basePath.absoluteFilePath("MitkWorkbench.provisioning");

  Poco::Util::MapConfiguration* extConfig(new Poco::Util::MapConfiguration());
  if (!storageDir.isEmpty())
  {
    extConfig->setString(berry::Platform::ARG_STORAGE_DIR.toStdString(), storageDir.toUtf8().constData());
  }
  extConfig->setString(berry::Platform::ARG_PROVISIONING.toStdString(), provFile.toStdString());
  extConfig->setString(berry::Platform::ARG_APPLICATION.toStdString(), "org.mitk.qt.extapplication");

  QStringList preloadLibs;

  // Preload the org.mitk.gui.qt.ext plug-in (and hence also QmitkExt) to speed
  // up a clean-cache start. This also works around bugs in older gcc and glibc implementations,
  // which have difficulties with multiple dynamic opening and closing of shared libraries with
  // many global static initializers. It also helps if dependent libraries have weird static
  // initialization methods and/or missing de-initialization code.
  preloadLibs << "liborg_mitk_gui_qt_ext";



  // Run the workbench.
  return berry::Starter::Run(argc, argv, extConfig);
}
