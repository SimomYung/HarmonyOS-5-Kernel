# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: Log module for HMCMakeNG
# Author: Huawei OS Kernel Lab
# Create: Tue May 18 14:39:27 2021

include_guard(GLOBAL)

function(hmcmakeng_error message)
    message(FATAL_ERROR "HMCMAKENG ERROR: ${message}")
endfunction(hmcmakeng_error)

function(hmcmakeng_warn message)
    message(WARNING "HMCMAKENG WARN: ${message}")
endfunction(hmcmakeng_warn)

function(hmcmakeng_status message)
    message(STATUS "HMCMAKENG STATUS: ${message}")
endfunction(hmcmakeng_status)

function(hmcmakeng_info message)
    message(VERBOSE "HMCMAKENG INFO: ${message}")
endfunction(hmcmakeng_info)

function(hmcmakeng_debug message)
    message(DEBUG "HMCMAKENG DEBUG: ${message}")
endfunction(hmcmakeng_debug)

function(hmcmakeng_trace message)
    message(DEBUG "HMCMAKENG TRACE: ${message}")
endfunction(hmcmakeng_trace)

# vim:ts=4:sw=4:expandtab
