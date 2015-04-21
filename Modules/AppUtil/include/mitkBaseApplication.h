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

#ifndef MITKBASEAPPLICATION_H
#define MITKBASEAPPLICATION_H

#include <MitkAppUtilExports.h>

#include <Poco/Util/Application.h>

#include <QString>
#include <QScopedPointer>
#include <QSharedPointer>

class ctkPluginContext;
class ctkPluginFramework;

namespace mitk {

class MITKAPPUTIL_EXPORT BaseApplication : public Poco::Util::Application
{
public:

  static QString ARG_NEWINSTANCE;
  static QString ARG_CLEAN;
  static QString ARG_APPLICATION;
  static QString ARG_HOME;
  static QString ARG_STORAGE_DIR;
  static QString ARG_PLUGIN_CACHE;
  static QString ARG_PLUGIN_DIRS;
  static QString ARG_FORCE_PLUGIN_INSTALL;
  static QString ARG_PRELOAD_LIBRARY;
  static QString ARG_PROVISIONING;
  static QString ARG_DEBUG;

  static QString ARG_CONSOLELOG;
  static QString ARG_TESTPLUGIN;
  static QString ARG_TESTAPPLICATION;

  static QString ARG_NO_REGISTRY_CACHE;
  static QString ARG_NO_LAZY_REGISTRY_CACHE_LOADING;
  static QString ARG_REGISTRY_MULTI_LANGUAGE;

  static QString ARG_XARGS;

  BaseApplication(int argc, char** argv);
  ~BaseApplication();

  int run() override;

  void printHelp(const std::string& name, const std::string& value);

  void setApplicationName(const QString& name);
  QString getApplicationName() const;

  void setOrganizationName(const QString& name);
  QString getOrganizationName() const;

  void setOrganizationDomain(const QString& name);
  QString getOrganizationDomain() const;

  void setSingleMode(bool singleMode);
  bool getSingleMode() const;

  void setSafeMode(bool safeMode);
  bool getSafeMode() const;

  void setProperty(const QString& property, const QVariant& value);
  QVariant getProperty(const QString& property) const;

protected:

  void initialize(Poco::Util::Application& self) override;
  void uninitialize();

  /**
   * Runs the application for which the platform was started. The platform
   * must be running.
   * <p>
   * The given argument is passed to the application being run.  If it is an invalid QVariant
   * then the command line arguments used in starting the platform, and not consumed
   * by the platform code, are passed to the application as a <code>QStringList</code>.
   * </p>
   * @param argument the argument passed to the application. May be <code>invalid</code>
   * @return the result of running the application
   * @throws std::exception if anything goes wrong
   */
  int main(const std::vector<std::string>& args) override;

  void defineOptions(Poco::Util::OptionSet& options) override;

  QSharedPointer<ctkPluginFramework> getFramework() const;
  ctkPluginContext* getFrameworkContext() const;

private:

  struct Impl;
  QScopedPointer<Impl> d;

};

}

#endif // MITKBASEAPPLICATION_H
