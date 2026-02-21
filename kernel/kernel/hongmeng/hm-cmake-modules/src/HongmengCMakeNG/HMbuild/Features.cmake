# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: HMbuild features checking
# Author: Huawei OS Kernel Lab
# Create: Thu Jul 29 15:47:33 2021

include_guard(GLOBAL)

include(HongmengCMakeNG/Log)

function(hmbuild_features_check feature lang default)
    set(var_checked "HMBUILD_FEATURE_${feature}_CHECKED")
    set(var_value   "HMBUILD_FEATURE_${feature}")

    if (NOT DEFINED CACHE{${var_checked}})
        if ((NOT ("${lang}" STREQUAL "")) AND (NOT DEFINED HMBUILD_LANG_ENABLED_${lang}))
            hmcmakeng_status("Skip feature check ${feature}: no binary building")

            set(${var_checked}  "TRUE" CACHE INTERNAL
                "HMbuild feature ${feature} is checked" FORCE)
            set(${var_value}     ${default} CACHE INTERNAL
                "Value of HMbuild feature ${feature}" FORCE)
        else()
            hmcmakeng_status("Check feature ${feature}")
            set(__HMBUILD_FEATURE_${feature}_TEMPDIR
                    ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/FeatureCheck${feature})

            file(REMOVE_RECURSE ${__HMBUILD_FEATURE_${feature}_TEMPDIR})
            file(MAKE_DIRECTORY ${__HMBUILD_FEATURE_${feature}_TEMPDIR})

            include(HongmengCMakeNG/HMbuild/Features/${feature})

            set(${var_checked}  "TRUE" CACHE INTERNAL
                "HMbuild feature ${feature} is checked" FORCE)
            set(${var_value}    ${__HMBUILD_FEATURE_${feature}_VALUE} CACHE INTERNAL
                "Value of HMbuild feature ${feature}" FORCE)

            hmcmakeng_status("Check feature ${feature} - done, result: $CACHE{${var_value}}")
        endif()
    else()
        hmcmakeng_debug("Check feature ${feature} - cached, result: $CACHE{${var_value}}")
    endif()
endfunction(hmbuild_features_check)

# Feature check depends on language settings
# This is the second setup phase, should depends on language detection result
function(hmbuild_setup_features)
    if (NOT DEFINED HMCMAKENG_HMBUILD_FEATURES_INITED)
        set(HMCMAKENG_HMBUILD_FEATURES_INITED "TRUE")
        hmbuild_features_check(KCFI        "C" "FALSE")
        hmbuild_features_check(ShadowStack "C" "FALSE")
        hmbuild_features_check(SFI         "C" "FALSE")
        hmbuild_features_check(KASAN       "C" "FALSE")
        hmbuild_features_check(UBSAN       "C" "FALSE")
        hmbuild_features_check(XOM         "C" "FALSE")
        hmbuild_features_check(DEBUG       ""  "FALSE")
        hmbuild_features_check(UCFI        "C" "FALSE")
        hmbuild_features_check(StackProtector "C" "FALSE")
        if(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64")
            hmbuild_features_check(FFIXED_X18  "C" "FALSE")
        endif()
    endif()
endfunction(hmbuild_setup_features)
# vim:ts=4:sw=4:expandtab
