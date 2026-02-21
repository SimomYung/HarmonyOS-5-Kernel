# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: CMakeLists for Tests
# Author: Huawei OS Kernel Lab
# Create: Wed Sep 22 16:29:20 2021

set(build_generator_args --build-generator ${CMAKE_GENERATOR})

macro(ADD_CROSS_TEST_MACRO DIR_NAME)
    if (ENABLE_TEST_WITH_SDK)
        list(APPEND cross_build_option "-DHMNATIVESDKPATH:PATH=${TEST_HM_NATIVE_SDK_PATH}")
        list(APPEND cross_build_option "-DCMAKE_SYSROOT=${TEST_HM_SDK_SYSROOT}")
        list(APPEND cross_build_option "-DCMAKE_TOOLCHAIN_FILE=${TEST_HM_TC_FILE}")
        ADD_TEST_MACRO(${DIR_NAME}  ${ARGN} BUILD_OPTION "${cross_build_option}")
        unset(cross_build_option)
    endif()
endmacro()
# a macro for tests that have a simple format where the name matches the
# directory and project
macro(ADD_TEST_MACRO DIR_NAME)
    cmake_parse_arguments("_add_test" "BUILD_ONE" "ALIAS;COMMAND" "BUILD_OPTION" ${ARGN})
    if(_add_test_ALIAS)
         set(_add_test_name ${_add_test_ALIAS})
    else()
         set(_add_test_name ${DIR_NAME})
    endif()
    if(_add_test_COMMAND)
        separate_arguments(_add_test_COMMAND)
        set(_test_command --test-command ${_add_test_COMMAND})
    endif()
    if(NOT _add_test_BUILD_ONE)
        set(_test_build_twice "--build-two-config")
    endif()
    string(REPLACE "." "/" dir "${DIR_NAME}")
    string(REGEX REPLACE "[^.]*\\." "" proj "${_add_test_name}")
    if (NOT _add_test_BUILD_OPTION)
        list(APPEND _add_test_BUILD_OPTION "-DCMAKE_SYSTEM_NAME=Hongmeng")
        list(APPEND _add_test_BUILD_OPTION "-DCMAKE_MODULE_PATH=${CMAKE_SOURCE_DIR}/src\;${CMAKE_SOURCE_DIR}/src/Findmodules")
    endif()
    list(APPEND _add_test_BUILD_OPTION "--log-level=DEBUG")
    list(APPEND _add_test_BUILD_OPTION "-Wno-dev")
    message("NAME = ${_add_test_name} project = ${proj}")
    add_test(NAME "${_add_test_name}" COMMAND "${CMAKE_CTEST_COMMAND}"
        --build-and-test
        "${CMAKE_SOURCE_DIR}/tests/${dir}"
        "${CMAKE_BINARY_DIR}/tests/${dir}"
        ${_test_build_twice}
        ${build_generator_args}
        --build-project ${proj}
        ${${_add_test_name}_CTEST_OPTIONS}
        -V
        --build-options
          ${_add_test_BUILD_OPTION}
          ${${_add_test_name}_BUILD_OPTIONS}
        ${_test_command})
    unset(_test_command)
    unset(_test_build_twice)
    unset(_add_test_name)
    unset(_add_test_COMMAND)
    unset(_add_test_ALIAS)
    unset(_add_test_BUILD_ONE)
    unset(_add_test_BUILD_OPTION)
    list(APPEND TEST_BUILD_DIRS "${CMAKE_BINARY_DIR}/tests/${dir}")
endmacro()

ADD_TEST_MACRO(cmd.exec.dynamic COMMAND "dynamic.elf" )
ADD_TEST_MACRO(cmd.exec.static COMMAND "static.elf")
ADD_CROSS_TEST_MACRO(cmd.exec.cross COMMAND "${CMAKE_LS_COMMAND} dynamic.elf")

ADD_TEST_MACRO(cmd.exec.no_condition)
set_tests_properties("cmd.exec.no_condition" PROPERTIES WILL_FAIL true)

include(cmd/cflags.cmake)
include(cmd/exclude.cmake)
include(cmd/libversion.cmake)
include(cmd/extracflags.cmake)
include(cmd/subdir.cmake)
include(cmd/output.cmake)
include(cmd/lib.cmake)
include(cmd/include.cmake)
include(cmd/src.cmake)
include(cmd/installinc.cmake)
include(cmd/inclib.cmake)
include(kconfig/kconfig.cmake)

include(others/builtin.cmake)
include(others/features.cmake)

ADD_TEST_MACRO(others.env_affect COMMAND "test.elf")
set_property(TEST others.env_affect
             APPEND PROPERTY
             ENVIRONMENT "LDFLAGS=-lother")

# If this is not an in-source build, provide a target to wipe out
# all the test build directories. This must come at the end after
# all the above logic has finished adding to TEST_BUILD_DIRS
if(NOT EXISTS "${CMAKE_BINARY_DIR}/CMakeLists.txt")
    configure_file(${CMAKE_SOURCE_DIR}/tests/test_clean.cmake.in
                   ${CMAKE_BINARY_DIR}/tests/test_clean.cmake @ONLY)
    add_custom_target(test_clean
      COMMAND ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/tests/test_clean.cmake
      COMMENT "Removing test build directories."
    )
endif()
