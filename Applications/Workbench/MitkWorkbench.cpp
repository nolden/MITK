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

#include <application/berryStarter.h>
#include <Poco/Util/MapConfiguration.h>

#include <QtGlobal>
#include <QTime>
#include <QDir>
#include <QDesktopServices>
#include <QStringList>
#include <QString>
#include <QMap>
#include <QHash>

#include <usModuleSettings.h>

#include <mitkCommon.h>
#include <mitkException.h>


int main(int argc, char** argv)
{
  // Create a QApplication instance first
  QtSafeApplication qSafeApp(argc, argv);
  qSafeApp.setApplicationName("MITK Workbench");
  qSafeApp.setOrganizationName("DKFZ");

  // This function checks if an instance is already running
  // and either sends a message to it (containing the command
  // line arguments) or checks if a new instance was forced by
  // providing the BlueBerry.newInstance command line argument.
  // In the latter case, a path to a temporary directory for
  // the new application's storage directory is returned.
  QString storageDir = handleNewAppInstance(&qSafeApp, argc, argv, "BlueBerry.newInstance");

  if (storageDir.isEmpty())
  {
    // This is a new instance and no other instance is already running. We specify
    // the storage directory here (this is the same code as in berryInternalPlatform.cpp
    // so that we can re-use the location for the persistent data location of the
    // the CppMicroServices library.

    // Append a hash value of the absolute path of the executable to the data location.
    // This allows to start the same application from different build or install trees.
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    storageDir = QStandardPaths::writableLocation(QStandardPaths::DataLocation) + '_';
#else
    storageDir = QDesktopServices::storageLocation(QDesktopServices::DataLocation) + '_';
#endif
    storageDir += QString::number(qHash(QCoreApplication::applicationDirPath())) + QDir::separator();
  }
  us::ModuleSettings::SetStoragePath((storageDir + QString("us") + QDir::separator()).toStdString());

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

  QMap<QString, QString> preloadLibVersion;

#ifdef Q_OS_MAC
  const QString libSuffix = ".dylib";
#elif defined(Q_OS_UNIX)
  const QString libSuffix = ".so";
#elif defined(Q_OS_WIN)
  const QString libSuffix = ".dll";
#else
  const QString libSuffix;
#endif

  for (QStringList::Iterator preloadLibIter = preloadLibs.begin(),
       iterEnd = preloadLibs.end(); preloadLibIter != iterEnd; ++preloadLibIter)
  {
    QString& preloadLib = *preloadLibIter;
    // In case the application is started from an install directory
    QString tempLibraryPath = basePath.absoluteFilePath("plugins/" + preloadLib + libSuffix);
    QFile preloadLibrary (tempLibraryPath);
#ifdef Q_OS_MAC
    if (!preloadLibrary.exists())
    {
      // In case the application is started from a build tree
      QString relPath = "../../../plugins/" + preloadLib + libSuffix;
      tempLibraryPath = QDir::cleanPath(basePath.absoluteFilePath(relPath));
      preloadLibrary.setFileName(tempLibraryPath);
    }
#endif
    if(preloadLibrary.exists())
    {
      preloadLib = tempLibraryPath;
    }
    // Else fall back to the QLibrary search logic
  }

  QString preloadConfig;
  Q_FOREACH(const QString& preloadLib, preloadLibs)
  {
    preloadConfig += preloadLib + preloadLibVersion[preloadLib] + ",";
  }
  preloadConfig.chop(1);

  extConfig->setString(berry::Platform::ARG_PRELOAD_LIBRARY.toStdString(), preloadConfig.toStdString());

  // Seed the random number generator, once at startup.
  QTime time = QTime::currentTime();
  qsrand((uint)time.msec());

  // Run the workbench.
  return berry::Starter::Run(argc, argv, extConfig);
}
