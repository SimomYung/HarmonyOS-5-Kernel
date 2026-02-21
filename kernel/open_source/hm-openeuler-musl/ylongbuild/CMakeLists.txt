cmake_minimum_required (VERSION 3.05.1)
project(hlibc LANGUAGES C)

if(MODULE_TYPE)
    add_subdirectory(test)
else()
    add_subdirectory(src)
endif()
