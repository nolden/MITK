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

#include "mitkBaseApplication.h"
#include "mitkLogMacros.h"

#include "mitkProvisioningInfo.h"
#include "QmitkSafeApplication.h"
#include "QmitkSingleApplication.h"

#include <ctkPluginConstants.h>
#include <ctkPluginFramework.h>
#include <ctkPluginFrameworkLauncher.h>
#include <ctkPluginFramework_global.h>

#include <Poco/Util/HelpFormatter.h>

#include <QTime>
#include <QDir>
#include <QDesktopServices>
#include <QStringList>
#include <QCoreApplication>

#include <iostream>

namespace mitk {

QString BaseApplication::ARG_NEWINSTANCE = "BlueBerry.newInstance";
QString BaseApplication::ARG_CLEAN = "BlueBerry.clean";
QString BaseApplication::ARG_APPLICATION = "BlueBerry.application";
QString BaseApplication::ARG_HOME = "BlueBerry.home";
QString BaseApplication::ARG_STORAGE_DIR = "BlueBerry.storageDir";
QString BaseApplication::ARG_PLUGIN_CACHE = "BlueBerry.plugin_cache_dir";
QString BaseApplication::ARG_PLUGIN_DIRS = "BlueBerry.plugin_dirs";
QString BaseApplication::ARG_FORCE_PLUGIN_INSTALL = "BlueBerry.forcePlugins";
QString BaseApplication::ARG_PRELOAD_LIBRARY = "BlueBerry.preloadLibrary";
QString BaseApplication::ARG_PROVISIONING = "BlueBerry.provisioning";
QString BaseApplication::ARG_DEBUG = "BlueBerry.debug";
QString BaseApplication::ARG_CONSOLELOG = "BlueBerry.consoleLog";
QString BaseApplication::ARG_TESTPLUGIN = "BlueBerry.testplugin";
QString BaseApplication::ARG_TESTAPPLICATION = "BlueBerry.testapplication";

QString BaseApplication::ARG_NO_REGISTRY_CACHE = "BlueBerry.noRegistryCache";
QString BaseApplication::ARG_NO_LAZY_REGISTRY_CACHE_LOADING = "BlueBerry.noLazyRegistryCacheLoading";
QString BaseApplication::ARG_REGISTRY_MULTI_LANGUAGE = "BlueBerry.registryMultiLanguage";

QString BaseApplication::ARG_XARGS = "xargs";

struct BaseApplication::Impl
{
  ctkProperties m_FWProps;

  QScopedPointer<QCoreApplication> m_QApp;

  int m_Argc;
  char** m_Argv;

  QString m_AppName;
  QString m_OrgaName;
  QString m_OrgaDomain;

  bool m_SingleMode;
  bool m_SafeMode;

  QStringList m_PreloadLibs;

  Impl(int argc, char** argv)
    : m_Argc(argc)
    , m_Argv(argv)
    , m_SingleMode(false)
    , m_SafeMode(true)
  {}

  void handleBooleanOption(const std::string& name, const std::string& /*value*/)
  {
    QString fwKey = QString::fromStdString(name);

    // translate some keys to proper framework properties
    if (fwKey == ARG_CONSOLELOG)
    {
      fwKey = ctkPluginFrameworkLauncher::PROP_CONSOLE_LOG;
    }

    m_FWProps[fwKey] = true;
  }

  void handlePreloadLibraryOption(const std::string& /*name*/, const std::string& value)
  {
    QString oldVal;
    if (m_FWProps.contains(BaseApplication::ARG_PRELOAD_LIBRARY))
    {
      oldVal = m_FWProps[BaseApplication::ARG_PRELOAD_LIBRARY].toString();
    }
    m_FWProps[BaseApplication::ARG_PRELOAD_LIBRARY] = oldVal + "," + QString::fromStdString(value);
  }

  void handleClean(const std::string& /*name*/, const std::string& /*value*/)
  {
    m_FWProps[ctkPluginConstants::FRAMEWORK_STORAGE_CLEAN] = ctkPluginConstants::FRAMEWORK_STORAGE_CLEAN_ONFIRSTINIT;
  }

};

BaseApplication::BaseApplication(int argc, char** argv)
  : Application()
  , d(new Impl(argc, argv))
{
}

BaseApplication::~BaseApplication()
{

}

void BaseApplication::printHelp(const std::string& /*name*/, const std::string& /*value*/)
{
  Poco::Util::HelpFormatter help(this->options());
  help.setAutoIndent();
  help.setCommand(this->commandName());
  help.format(std::cout);

  exit(EXIT_OK);
}

void BaseApplication::setApplicationName(const QString& name)
{
  if (qApp) qApp->setApplicationName(name);
  d->m_AppName = name;
}

QString BaseApplication::getApplicationName() const
{
  if (qApp)
  {
    return qApp->applicationName();
  }
  return d->m_AppName;
}

void BaseApplication::setOrganizationName(const QString& name)
{
  if (qApp) qApp->setOrganizationName(name);
  d->m_OrgaName = name;
}

QString BaseApplication::getOrganizationName() const
{
  if (qApp) return qApp->organizationName();
  return d->m_OrgaName;
}

void BaseApplication::setOrganizationDomain(const QString& domain)
{
  if (qApp) qApp->setOrganizationDomain(domain);
  d->m_OrgaDomain = domain;
}

QString BaseApplication::getOrganizationDomain() const
{
  if (qApp) return qApp->organizationDomain();
  return d->m_OrgaDomain;
}


void BaseApplication::setSingleMode(bool singleMode)
{
  if (qApp) return;
  d->m_SingleMode = singleMode;
}

bool BaseApplication::getSingleMode() const
{
  return d->m_SingleMode;
}

void BaseApplication::setSafeMode(bool safeMode)
{
  if (qApp && !d->m_QApp) return;
  d->m_SafeMode = safeMode;
  if (d->m_QApp)
  {
    if (getSingleMode())
    {
      static_cast<QmitkSingleApplication*>(d->m_QApp.data())->setSafeMode(safeMode);
    }
    else
    {
      static_cast<QmitkSafeApplication*>(d->m_QApp.data())->setSafeMode(safeMode);
    }
  }
}

bool BaseApplication::getSafeMode() const
{
  return d->m_SafeMode;
}

void BaseApplication::setPreloadLibraries(const QStringList& libraryBaseNames)
{
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


  d->m_PreloadLibs = libraryBaseNames;
}

QStringList BaseApplication::getPreloadLibraries() const
{
  return d->m_PreloadLibs;
}

void BaseApplication::initialize(Poco::Util::Application& self)
{
  Poco::Util::Application::initialize(self);

  // The CTK PluginFramework needs a QCoreApplication
  if (!qApp)
  {
    if (getSingleMode())
    {
      d->m_QApp.reset(new QmitkSingleApplication(d->m_Argc, d->m_Argv, getSafeMode()));
    }
    else
    {
      QmitkSafeApplication* safeApp = new QmitkSafeApplication(d->m_Argc, d->m_Argv);
      safeApp->setSafeMode(d->m_SafeMode);
      d->m_QApp.reset(safeApp);
    }
  }

  this->loadConfiguration();

  // Seed the random number generator, once at startup.
  QTime time = QTime::currentTime();
  qsrand((uint)time.msec());

  // Initialize the CTK Plugin Framework

  // add all configuration key / value pairs as framework properties
  Poco::Util::LayeredConfiguration& configuration = this->config();
  Poco::Util::LayeredConfiguration::Keys keys;
  Poco::Util::LayeredConfiguration::Keys keyStack;
  configuration.keys(keyStack);
  std::vector<std::string> keyChain;
  while(!keyStack.empty())
  {
    std::string currSubKey = keyStack.back();
    if (!keyChain.empty() && keyChain.back() == currSubKey)
    {
      keyChain.pop_back();
      keyStack.pop_back();
      continue;
    }
    Poco::Util::LayeredConfiguration::Keys subKeys;
    configuration.keys(currSubKey, subKeys);
    if (subKeys.empty())
    {
      keyStack.pop_back();
      std::string finalKey;
      for (auto k = keyChain.begin(); k != keyChain.end(); ++k)
      {
        finalKey += *k + ".";
      }
      finalKey += currSubKey;
      keys.push_back(finalKey);
    }
    else
    {
      keyChain.push_back(currSubKey);
      for (auto s : subKeys)
      {
        keyStack.push_back(s);
      }
    }
  }

  for (auto key : keys)
  {
    QString qKey = QString::fromStdString(key);
    if (!d->m_FWProps.contains(qKey) && configuration.hasProperty(key))
    {
      d->m_FWProps[qKey] = QString::fromStdString(configuration.getString(key));
    }
  }

  bool consoleLog = this->getProperty(ctkPluginFrameworkLauncher::PROP_CONSOLE_LOG).toBool();

  // read initial plugins from a provisioning file
  QStringList pluginsToStart;

  QString provisioningFile = this->getProperty(ARG_PROVISIONING).toString();
  if (!provisioningFile.isEmpty())
  {
    MITK_INFO(consoleLog) << "Using provisioning file: " << provisioningFile.toStdString();
    ProvisioningInfo provInfo(provisioningFile);
    // it can still happen, that the encoding is not compatible with the fromUtf8 function ( i.e. when manipulating the LANG variable
    // in such case, the QStringList in provInfo is empty which we can easily check for
    if( provInfo.getPluginDirs().empty() )
    {
      MITK_ERROR << "Cannot search for provisioning file, the retrieved directory list is empty.\n" <<
                    "This can occur if there are some special (non-ascii) characters in the install path.";
    }
    else
    {
      foreach(QString pluginPath, provInfo.getPluginDirs())
      {
        ctkPluginFrameworkLauncher::addSearchPath(pluginPath);
      }

      //bool forcePluginOverwrite = this->getProperty(ARG_FORCE_PLUGIN_INSTALL).toBool();
      QList<QUrl> pluginUrlsToStart = provInfo.getPluginsToStart();
      for(auto url : pluginUrlsToStart)
      {
        pluginsToStart.push_back(url.toString());
      }
      //foreach(QUrl pluginUrl, provInfo.getPluginsToInstall())
      //{
        // TODO for "uninstall", we need a proper configuration agent, e.g. a dedicated
        // plug-in for provisioning of the platform
        /*
        if (forcePluginOverwrite)
        {
          uninstallPugin(pluginUrl, context);
        }
        */
        //try
        //{
          //MITK_INFO(consoleLog) << "Installing CTK plug-in from: " << pluginUrl.toString().toStdString();
          /*
          QSharedPointer<ctkPlugin> plugin = context->installPlugin(pluginUrl);
          if (pluginsToStart.contains(pluginUrl))
          {
            m_CTKPluginsToStart << plugin->getPluginId();
          }
          */
          /*
        }
        catch (const ctkPluginException& e)
        {
          QString errorMsg;
          QDebug dbg(&errorMsg);
          dbg << e.printStackTrace();
          BERRY_ERROR << qPrintable(errorMsg);
        }
        */
      //}
    }
  }
  else
  {
    MITK_INFO(consoleLog) << "No provisioning file set.";
  }

  if(!pluginsToStart.isEmpty())
  {
    d->m_FWProps[ctkPluginFrameworkLauncher::PROP_PLUGINS] = pluginsToStart;
  }

  // Finally, set the CTK Plugin Framework properties
  ctkPluginFrameworkLauncher::setFrameworkProperties(d->m_FWProps);
}

void BaseApplication::uninitialize()
{
  QSharedPointer<ctkPluginFramework> pfw = this->getFramework();
  if (pfw)
  {
    pfw->stop();

    // wait 10 seconds for the CTK plugin framework to stop
    pfw->waitForStop(10000);
  }

  Poco::Util::Application::uninitialize();
}

int BaseApplication::main(const std::vector<std::string>& /*args*/)
{
  // Start the plugin framework and all installed plug-ins according with
  // their auto-start setting.
  return ctkPluginFrameworkLauncher::run().toInt();
}

void BaseApplication::defineOptions(Poco::Util::OptionSet& options)
{
  Poco::Util::Option helpOption("help", "h", "print this help text");
  helpOption.callback(Poco::Util::OptionCallback<BaseApplication>(this, &BaseApplication::printHelp));
  options.addOption(helpOption);

  Poco::Util::Option newInstanceOption(ARG_NEWINSTANCE.toStdString(), "", "forces a new instance of this application");
  newInstanceOption.callback(Poco::Util::OptionCallback<Impl>(d.data(), &Impl::handleBooleanOption));
  options.addOption(newInstanceOption);

  Poco::Util::Option cleanOption(ARG_CLEAN.toStdString(), "", "cleans the plugin cache");
  cleanOption.callback(Poco::Util::OptionCallback<Impl>(d.data(), &Impl::handleClean));
  options.addOption(cleanOption);

  Poco::Util::Option appOption(ARG_APPLICATION.toStdString(), "", "the id of the application extension to be executed");
  appOption.argument("<id>").binding("blueberry.application");
  options.addOption(appOption);

  Poco::Util::Option provOption(ARG_PROVISIONING.toStdString(), "", "the location of a provisioning file");
  provOption.argument("<prov file>").binding(ARG_PROVISIONING.toStdString());
  options.addOption(provOption);

  Poco::Util::Option storageDirOption(ARG_STORAGE_DIR.toStdString(), "", "the location for storing persistent application data");
  storageDirOption.argument("<dir>").binding(ctkPluginConstants::FRAMEWORK_STORAGE.toStdString());
  options.addOption(storageDirOption);

  Poco::Util::Option consoleLogOption(ARG_CONSOLELOG.toStdString(), "", "log messages to the console");
  consoleLogOption.callback(Poco::Util::OptionCallback<Impl>(d.data(), &Impl::handleBooleanOption));
  options.addOption(consoleLogOption);

  Poco::Util::Option debugOption(ARG_DEBUG.toStdString(), "", "enable debug mode");
  debugOption.argument("<options file>", false).binding(ctkPluginFrameworkLauncher::PROP_DEBUG.toStdString());
  options.addOption(debugOption);

  Poco::Util::Option forcePluginOption(ARG_FORCE_PLUGIN_INSTALL.toStdString(), "", "force installing plug-ins with same symbolic name");
  forcePluginOption.callback(Poco::Util::OptionCallback<Impl>(d.data(), &Impl::handleBooleanOption));
  options.addOption(forcePluginOption);

  Poco::Util::Option preloadLibsOption(ARG_PRELOAD_LIBRARY.toStdString(), "", "preload a library");
  preloadLibsOption.argument("<library>").repeatable(true).callback(Poco::Util::OptionCallback<Impl>(d.data(), &Impl::handlePreloadLibraryOption));
  options.addOption(preloadLibsOption);

  Poco::Util::Option testPluginOption(ARG_TESTPLUGIN.toStdString(), "", "the plug-in to be tested");
  testPluginOption.argument("<id>").binding(ARG_TESTPLUGIN.toStdString());
  options.addOption(testPluginOption);

  Poco::Util::Option testAppOption(ARG_TESTAPPLICATION.toStdString(), "", "the application to be tested");
  testAppOption.argument("<id>").binding(ARG_TESTAPPLICATION.toStdString());
  options.addOption(testAppOption);

  Poco::Util::Option noRegistryCacheOption(ARG_NO_REGISTRY_CACHE.toStdString(), "", "do not use a cache for the registry");
  noRegistryCacheOption.callback(Poco::Util::OptionCallback<Impl>(d.data(), &Impl::handleBooleanOption));
  options.addOption(noRegistryCacheOption);

  Poco::Util::Option noLazyRegistryCacheLoadingOption(ARG_NO_LAZY_REGISTRY_CACHE_LOADING.toStdString(), "", "do not use lazy cache loading for the registry");
  noLazyRegistryCacheLoadingOption.callback(Poco::Util::OptionCallback<Impl>(d.data(), &Impl::handleBooleanOption));
  options.addOption(noLazyRegistryCacheLoadingOption);

  Poco::Util::Option registryMultiLanguageOption(ARG_REGISTRY_MULTI_LANGUAGE.toStdString(), "", "enable multi-language support for the registry");
  registryMultiLanguageOption.callback(Poco::Util::OptionCallback<Impl>(d.data(), &Impl::handleBooleanOption));
  options.addOption(registryMultiLanguageOption);

  Poco::Util::Option xargsOption(ARG_XARGS.toStdString(), "", "Extended argument list");
  xargsOption.argument("<args>").binding(ARG_XARGS.toStdString());
  options.addOption(xargsOption);

  Poco::Util::Application::defineOptions(options);

}

QSharedPointer<ctkPluginFramework> BaseApplication::getFramework() const
{
  return ctkPluginFrameworkLauncher::getPluginFramework();
}

ctkPluginContext* BaseApplication::getFrameworkContext() const
{
  QSharedPointer<ctkPluginFramework> framework = getFramework();
  if (framework) return framework->getPluginContext();
  return nullptr;
}

int BaseApplication::run()
{
  this->init(d->m_Argc, d->m_Argv);

  if (d->m_SingleMode)
  {
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
  }

  us::ModuleSettings::SetStoragePath((storageDir + QString("us") + QDir::separator()).toStdString());


  return Application::run();
}

void BaseApplication::setProperty(const QString& property, const QVariant& value)
{
  d->m_FWProps[property] = value;
}

QVariant BaseApplication::getProperty(const QString& property) const
{
  auto iter = d->m_FWProps.find(property);
  return iter == d->m_FWProps.end() ? QVariant() : iter.value();
}

}
