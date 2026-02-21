## About
This doc is for postbuild module of HMbuild.
Post-build modules is to handle the adjustment requirement on compiled out binary.
Features that related with build is not recomended to contain post-build process.
So this module is maintained for handling corner cases.

## Design for PostBuild
To make post-build extendible, consider each post-build process as an independent module.
Each module handle only one feature requirement.

1. Each post-build module is a cmake module which contains the process function
   ``hmbuild_postbuild_xxx()`` and a command that call it. The process function use ``add_custom_target``
   , append self to the process chain, customize postbuild actions by ``COMMAND`` arguments.

2. The framework fucntion ``hmbuid_postbuld()`` include each post-build modules and assemble
   the sequence of function all.

3. Cmake module AddExec or AddDynamicLib etc. call ``hmbuid_postbuld(${target})`` to add postbuild
   processes for specific hmbuild target.

4. ``hmbuid_postbuld_xxx()`` in each module
   1) HMbuild target ``target`` and ``tail_target_var`` as parameter.
   2) Fetch per-target configuration properties from ``cmake_target``(covert from HMbuild target).
   3) Fetch global configuration by ``hmbuild_postbuild_get_arg(<module_name> <arg_name> <return_var>)``
      Other modules use ``hmbuild_postbuild_set_arg(<module_name> <arg_name> <arg_value>)`` to set them.
   4) Fetch parent scope variable ``tail_target`` by ``${${tail_target_var}}``, and update it to self-customized
      target after invoke ``add_custom_target``.

## Example to add PostBuild module

``## <--`` is for highlighting extra comment

1. HMbuild/PostBuild/Xom.cmake (new file implement ``hmbuid_postbuld_xom``)
```
function(hmbuild_postbuild_xom target tail_target_var)
    if (NOT HMBUILD_FEATURE_XOM OR NOT "${CMAKE_C_FLAGS}" MATCHES ".*\\-\\-xom\\-march64.*")
        return()
    endif()

    hmbuild_target_cmakename_of(cmake_target ${target})

    get_target_property(_type ${cmake_target} TYPE) ## <--get per-target config
    hmbuild_postbuild_get_arg("XOM" "TOOL_PATH" XOM_ELF) ## <--get global config
    if (NOT XOM_ELF)
        return()
    endif()
    if (_type STREQUAL "EXECUTABLE" OR _type STREQUAL "SHARED_LIBRARY")
        add_custom_target(${cmake_target}.xom ALL ## <--custom target
                      COMMAND ${BASH} -c "${XOM_ELF} $<TARGET_FILE:${cmake_target}>" ## <--custom command
                      DEPENDS ${${tail_target_var}}) ## <--depends tail target in parent scope
        set(${tail_target_var} ${cmake_target}.xom PARENT_SCOPE) ## <--update tail target to self-defined target
    endif()
endfunction(hmbuild_postbuild_xom)

hmbuild_postbuild_xom(${target} tail_target)
```
2. HMbuild/PostBuild/Utils.cmake (include Xom.cmake)
```
...
function(hmbuild_postbuild target)
    hmbuild_target_cmakename_of(cmake_target ${target})
    # Post-build modules update tail target for next module to depends
    set(tail_target ${cmake_target})
    include(HongmengCMakeNG/HMbuild/PostBuild/Xom) ## <--include xom module
    include(HongmengCMakeNG/HMbuild/PostBuild/Strip)
    add_custom_target(${cmake_target}.final ALL DEPENDS ${tail_target})
endfunction(hmbuild_postbuild)
```
3. HMbuild/Features/XOM.cmake (set global argument)
```
   ...
   if(NOT "${__MATCH_RESULT}" STREQUAL "")
       # xom feature is enabled and it is work
       set(__HMBUILD_FEATURE_XOM_VALUE "TRUE")
       if("${CMAKE_C_FLAGS}" MATCHES ".*\\-\\-xom\\-march64.*")
           # march64 program need post process, set up argument to xom post process module.
           hmbuild_postbuild_set_arg("XOM" "TOOL_PATH" "${XOM_ELF}") ## <--set global args for xom
       endif()
   endif()
   ...
```
