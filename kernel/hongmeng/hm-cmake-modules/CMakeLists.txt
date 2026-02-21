# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: CMakeLists.txt of cmakeng-modules
# Author: Huawei OS Kernel Lab
# Create: Fri Sep 17 15:07:53 2021

cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(hm-cmakeng-modules)

if (NOT MODULE_INSTALL_DIR)
    set(MODULE_INSTALL_DIR "/usr/share/cmake/Modules")
endif()

set(MODULES_SRC_DIR src)
set(FIND_MODULES_SRC_DIR src/Findmodules)
set(CMAKENG_ROOT_INSTALL
    ${MODULES_SRC_DIR}/HMFindUtils.cmake
    ${MODULES_SRC_DIR}/HongmengCMakeNG.cmake
    ${FIND_MODULES_SRC_DIR}/FindHMUlibs.cmake
    ${FIND_MODULES_SRC_DIR}/FindHMUlibs_sysk.cmake
    ${FIND_MODULES_SRC_DIR}/FindCompilerLibs.cmake
    ${FIND_MODULES_SRC_DIR}/FindCompilerLibs_GNU.cmake
    ${FIND_MODULES_SRC_DIR}/FindCompilerLibs_iTrustee.cmake
    ${FIND_MODULES_SRC_DIR}/FindCompilerLibs_LLVM.cmake
    ${FIND_MODULES_SRC_DIR}/FindHWSecureC.cmake
    ${FIND_MODULES_SRC_DIR}/FindHWSecureC_sysk.cmake
    ${FIND_MODULES_SRC_DIR}/FindHMBase.cmake
    ${FIND_MODULES_SRC_DIR}/FindHMLwIPLib.cmake
    ${FIND_MODULES_SRC_DIR}/FindHMVispLibs.cmake
    ${FIND_MODULES_SRC_DIR}/FindHMLibC.cmake
    ${FIND_MODULES_SRC_DIR}/FindHMLibC_sysk.cmake
    ${FIND_MODULES_SRC_DIR}/FindHMLz4Lib.cmake
    ${FIND_MODULES_SRC_DIR}/FindHMLz4Lib_sysk.cmake
    ${FIND_MODULES_SRC_DIR}/FindHMLz4kLib.cmake
    ${FIND_MODULES_SRC_DIR}/FindHMLz4kLib_sysk.cmake
    ${FIND_MODULES_SRC_DIR}/FindHMUtf8procLib.cmake
    ${FIND_MODULES_SRC_DIR}/FindHMLzmaLib.cmake
    ${FIND_MODULES_SRC_DIR}/FindHMLzmaLib_sysk.cmake
    ${FIND_MODULES_SRC_DIR}/FindHMZlib.cmake
    ${FIND_MODULES_SRC_DIR}/FindHMZlib_sysk.cmake
    ${FIND_MODULES_SRC_DIR}/FindHMKCrypto.cmake
    ${FIND_MODULES_SRC_DIR}/FindHMKLib.cmake
    ${FIND_MODULES_SRC_DIR}/FindHMLibOpt.cmake
    ${FIND_MODULES_SRC_DIR}/FindLibfdt.cmake
    ${FIND_MODULES_SRC_DIR}/FindLibfdt_sysk.cmake
    ${FIND_MODULES_SRC_DIR}/FindCMSCBBLib.cmake
    ${FIND_MODULES_SRC_DIR}/FindCMSCBBLib_sysk.cmake
    ${FIND_MODULES_SRC_DIR}/FindHMHitlsLib.cmake
    ${FIND_MODULES_SRC_DIR}/FindHMHitlsLib_sysk.cmake
    ${FIND_MODULES_SRC_DIR}/FindHMMetaLibs.cmake
    ${FIND_MODULES_SRC_DIR}/FindHMMetaLibs_sysk.cmake
    ${FIND_MODULES_SRC_DIR}/FindHMOpensslLib.cmake
    ${FIND_MODULES_SRC_DIR}/FindHMOpensslLib_sysk.cmake
    ${FIND_MODULES_SRC_DIR}/FindHMFdt.cmake
    ${FIND_MODULES_SRC_DIR}/FindHMFdt_sysk.cmake
)

install(FILES ${CMAKENG_ROOT_INSTALL}
    COMPONENT file_install
    DESTINATION ${MODULE_INSTALL_DIR})

install(DIRECTORY ${PROJECT_SOURCE_DIR}/${MODULES_SRC_DIR}/HongmengCMakeNG
    COMPONENT find_install
    DESTINATION ${MODULE_INSTALL_DIR})

install(DIRECTORY ${PROJECT_SOURCE_DIR}/${MODULES_SRC_DIR}/Platform
    COMPONENT find_install
    DESTINATION  ${MODULE_INSTALL_DIR})

install(DIRECTORY ${PROJECT_SOURCE_DIR}/${MODULES_SRC_DIR}/Compiler
    COMPONENT find_install
    DESTINATION  ${MODULE_INSTALL_DIR})

if (IS_DIRECTORY ${PROJECT_SOURCE_DIR}/tests)
    function(findcmd_standalone cmd variable)
        find_program(${variable}
                     NAMES ${cmd})
        if (NOT ${variable})
            message(FATAL_ERROR "${cmd} not found")
        else()
            message("${cmd} found at ${${variable}}")
        endif()
    endfunction(findcmd_standalone)

    enable_testing()
    get_filename_component(CMake_BIN_PATH "${CMAKE_COMMAND}" DIRECTORY)
    #-----------------------------------------------------------------------
    # a macro to determine the generator and ctest executable to use
    #-----------------------------------------------------------------------
    macro(CMAKE_SETUP_TESTING)
        findcmd_standalone(ctest CMAKE_CTEST_COMMAND)
        findcmd_standalone(cmake CMAKE_CMAKE_COMMAND)
        findcmd_standalone(cpack CMAKE_CPACK_COMMAND)
        findcmd_standalone(nm CMAKE_NM_COMMAND)
        findcmd_standalone(ls CMAKE_LS_COMMAND)
        findcmd_standalone(readelf CMAKE_READELF_COMMAND)
        findcmd_standalone(cat CMAKE_CAT_COMMAND)
    endmacro(CMAKE_SETUP_TESTING)

    CMAKE_SETUP_TESTING()
    add_subdirectory(tests)
endif()
