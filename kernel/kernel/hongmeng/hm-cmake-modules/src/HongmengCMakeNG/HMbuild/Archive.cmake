# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: Common archive settings for HMbuild
# Author: Huawei OS Kernel Lab
# Create: Sat May 29 02:50:08 2021

include_guard(GLOBAL)

# When language is set to NONE for a project, CMAKE_HONGMENG_ARCHIVER_ID is not set
if (NOT ("${CMAKE_HONGMENG_ARCHIVER_ID}" STREQUAL ""))
    include(HongmengCMakeNG/HMbuild/Archive/${CMAKE_HONGMENG_ARCHIVER_ID})
else()
    # Provide a fallback hmbuild_create_regular_archive to make sure scripts
    # require this function get processed
    function(hmbuild_create_regular_archive target src)
        add_custom_command(OUTPUT ${target}
                           COMMAND ${CMAKE_COMMAND} -E remove -f ${target}
                           COMMAND ${CMAKE_COMMAND} -E touch ${target}
                           DEPENDS ${src}
                           VERBATIM)
    endfunction()
endif()

# vim:ts=4:sw=4:expandtab
