#-----------------------------------------------------------------------------
# GDCM
#-----------------------------------------------------------------------------

# Sanity checks
if(DEFINED GDCM_DIR AND NOT EXISTS ${GDCM_DIR})
  message(FATAL_ERROR "GDCM_DIR variable is defined but corresponds to non-existing directory")
endif()

# Check if an external ITK build tree was specified.
# If yes, use the GDCM from ITK, otherwise ITK will complain
if(ITK_DIR)
  find_package(ITK)
  if(ITK_GDCM_DIR)
    set(GDCM_DIR ${ITK_GDCM_DIR})
  endif()
endif()


set(proj GDCM)
set(proj_DEPENDENCIES )
set(GDCM_DEPENDS ${proj})

if(NOT DEFINED GDCM_DIR)

  set(additional_args )
  if(CTEST_USE_LAUNCHERS)
    list(APPEND additional_args
      "-DCMAKE_PROJECT_${proj}_INCLUDE:FILEPATH=${CMAKE_ROOT}/Modules/CTestUseLaunchers.cmake"
    )
  endif()

  # On Mac some assertions fail that prevent reading certain DICOM files. Bug #19995
  if(APPLE)
    list(APPEND additional_args
      "-DCMAKE_CXX_FLAGS_DEBUG:STRING=${CMAKE_CXX_FLAGS_DEBUG} -DNDEBUG"
    )
  endif()

  mitk_query_custom_ep_vars()

  ExternalProject_Add(${proj}
     LIST_SEPARATOR ${sep}
     URL https://downloads.sourceforge.net/project/gdcm/gdcm%203.x/GDCM%203.0.4/gdcm-3.0.4.tar.gz
     # PATCH_COMMAND ${PATCH_COMMAND} -N -p1 -i ${CMAKE_CURRENT_LIST_DIR}/GDCM.patch
     CMAKE_GENERATOR ${gen}
     CMAKE_GENERATOR_PLATFORM ${gen_platform}
     CMAKE_ARGS
       ${ep_common_args}
       ${additional_args}
       -DGDCM_BUILD_SHARED_LIBS:BOOL=ON
       ${${proj}_CUSTOM_CMAKE_ARGS}
     CMAKE_CACHE_ARGS
       ${ep_common_cache_args}
       ${${proj}_CUSTOM_CMAKE_CACHE_ARGS}
     CMAKE_CACHE_DEFAULT_ARGS
       ${ep_common_cache_default_args}
       ${${proj}_CUSTOM_CMAKE_CACHE_DEFAULT_ARGS}
     DEPENDS ${proj_DEPENDENCIES}
    )
  set(GDCM_DIR ${ep_prefix})
  mitkFunctionInstallExternalCMakeProject(${proj})

else()

  mitkMacroEmptyExternalProject(${proj} "${proj_DEPENDENCIES}")

  find_package(GDCM)

endif()
