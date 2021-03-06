project(CoreApp)

# Create a cache entry for the provisioning file which is used to export
# the file name in the MITKConfig.cmake file. This will keep external projects
# which rely on this file happy.
set(MITK_COREAPP_PROVISIONING_FILE "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/MitkCoreApp.provisioning" CACHE INTERNAL "CoreApp provisioning file" FORCE)

set(_plugins
  org.mitk.gui.qt.coreapplication
  org.mitk.gui.qt.stdmultiwidgeteditor
)

# Plug-ins listed below will not be
#  - added as a build-time dependency to the executable
#  - listed in the provisioning file for the executable
#  - installed if they are external plug-ins

set(_exclude_plugins )

mitkFunctionCreateBlueBerryApplication(
  NAME MitkCoreApp
  DESCRIPTION "MITK - CoreApp Application"
  PLUGINS ${_plugins}
)

# subproject support
add_dependencies(MITK-CoreUI MitkCoreApp)
