# cmakeng build
if(NOT DEFINED HMBUILD_LEGACY_CMAKE_LIBRARY)
    cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
    project(dhext-fileguard C ASM)
    add_link_options("-L${CMAKE_CURRENT_SOURCE_DIR}")

    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

    # import cmakeng
    include(HongmengCMakeNG/Kconfig)
    include(HongmengCMakeNG/HMbuild)

    return()
endif()
