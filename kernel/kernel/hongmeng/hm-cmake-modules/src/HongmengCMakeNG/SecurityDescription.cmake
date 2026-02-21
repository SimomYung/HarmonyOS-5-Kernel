# Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
# Description: Provide common functions to generate code for security description
# Author: Huawei OS Kernel Lab
# Create: Fri Dec 23 10:44:38 2022

include_guard(GLOBAL)

function(_hmsd_execute)
    cmake_parse_arguments (PARSE_ARGV 0 _exe "" "VAR" "CMD")
    execute_process(
        COMMAND ${_exe_CMD}
        OUTPUT_VARIABLE output
        ERROR_VARIABLE error
        RESULT_VARIABLE result
        )
    string(REPLACE "\n" ";" output "${output}")
    list(FILTER output EXCLUDE REGEX "^$")
    if(result AND NOT result EQUAL 0)
        hmcmakeng_error("Execution failed: ${result} Commands: ${_exe_CMD} Error info: ${error}")
    endif()
    set(${_exe_VAR} ${output} PARENT_SCOPE)
endfunction(_hmsd_execute)

function(_hmsd_tool_path var)
    if (NOT DEFINED _hmsdtool)
        if (NOT (DEFINED CONFIG_HMSD_PYTHON_PATH))
            hmcmakeng_error("CONFIG_HMSD_PYTHON_PATH not defined")
        endif()
        find_program(hmsdtool_path
            NAMES hmsdtool
            PATHS "${HMNATIVESDKPATH}/usr/bin"
            NO_DEFAULT_PATH)
        set(_hmsdtool env
            "PYTHONPATH=${hmsdtool_path}/../hm-dsl:${HMNATIVESDKPATH}/usr/lib/python3/site-packages:${HMNATIVESDKPATH}/usr/lib/python3.10/site-packages"
            ${CONFIG_HMSD_PYTHON_PATH} ${hmsdtool_path}
            CACHE INTERNAL "hmsd tool command")
    endif()
    set(${var} ${_hmsdtool} PARENT_SCOPE)
endfunction(_hmsd_tool_path)

function(_hmsd_execute_tool)
    _hmsd_tool_path(hmsdtool)
    cmake_parse_arguments (PARSE_ARGV 0 _exe "" "VAR;DESC" "CMD;FILES;DEPENDS")
    _hmsd_execute(
    CMD ${hmsdtool} ${_exe_CMD} -d ${_exe_FILES}
        VAR gen_files
        )
    hmcmakeng_status("Generated ${_exe_DESC} files: ${gen_files}")
    _hmsd_execute(
    CMD ${hmsdtool} ${_exe_CMD}  ${_exe_FILES}
        )
    set(${_exe_VAR} ${gen_files} PARENT_SCOPE)
endfunction(_hmsd_execute_tool)

function(_hmsd_all_description_files dir var)
    file(GLOB_RECURSE files
        CONFIGURE_DEPENDS
        "${dir}/*.json"
        "${dir}/*.toml"
        "${dir}/*.yaml"
        "${dir}/*.xml"
        )
    list(LENGTH files file_num)
    if (file_num EQUAL 0)
        hmcmakeng_error("No description files are found")
    endif()
    hmcmakeng_status("Found description files: ${files}")
    set(${var} ${files} PARENT_SCOPE)
endfunction(_hmsd_all_description_files)

function(_hmsd_options dac capability encaps seharmony acl userns yama dec hkids isolate keyring audit skip_mod need_close_pan trend option)
    set(opts --code-module buckle --code-module mac --code-module ability --code-module misc)
    if (dac)
        set(opts ${opts} --code-module dac)
    endif()
    if (dec)
        set(opts ${opts} --code-module dec)
    endif()    
    if (hkids)
        set(opts ${opts} --code-module hkids)
    endif()
    if (isolate)
        set(opts ${opts} --code-module isolate)
    endif()
    if (capability)
        set(opts ${opts} --code-module capability)
    endif()
    if (encaps)
        set(opts ${opts} --code-module encaps)
    endif()
    if (seharmony)
        set(opts ${opts} -s --code-module seharmony)
    endif()
    if (acl)
        set(opts ${opts} --code-module acl)
    endif()
    if (userns)
        set(opts ${opts} --code-module userns)
    endif()
    if (yama)
        set(opts ${opts} --code-module yama)
    endif()
    if (keyring)
        set(opts ${opts} --code-module keyring)
    endif()
    if (audit)
        set(opts ${opts} --code-module audit)
    endif()
    if (skip_mod)
        set(opts ${opts} --code-skip-mod)
    endif()
    if (need_close_pan)
        set(opts ${opts} --code-need-close-pan)
    endif()
    if (NOT ("${trend}" STREQUAL ""))
        set(opts ${opts} --code-trend ${trend})
    endif()
    if (NOT ("${HMSD_CONFIGS}" STREQUAL ""))
        set(opts ${opts} --code-configs ${HMSD_CONFIGS})
    endif()
    hmcmakeng_status("Generation options: ${opts}")
    set(${option} ${opts} PARENT_SCOPE)
endfunction(_hmsd_options)

function(hmsd_generate)
    cmake_parse_arguments (PARSE_ARGV 0 _gen "DAC;CAPABILITY;ENCAPS;SEHARMONY;ACL;USERNS;YAMA;DEC;HKIDS;ISOLATE;KEYRING;AUDIT;SKIP_MOD;NEED_CLOSE_PAN" "TARGET;DIRECTORY;TREND" "FILES")

    _hmsd_all_description_files(${_gen_DIRECTORY} sd_files)
    _hmsd_options(${_gen_DAC} ${_gen_CAPABILITY} ${_gen_ENCAPS} ${_gen_SEHARMONY} ${_gen_ACL} ${_gen_USERNS} ${_gen_YAMA} ${_gen_DEC} ${_gen_HKIDS} ${_gen_ISOLATE} ${_gen_KEYRING} ${_gen_AUDIT} ${_gen_SKIP_MOD} ${_gen_NEED_CLOSE_PAN} "${_gen_TREND}" hmsd_options)

    _hmsd_execute_tool(
        CMD -g code -o ${CMAKE_CURRENT_BINARY_DIR}/generated/hmsd ${hmsd_options}
        FILES ${sd_files}
        VAR gen_src_files
        DESC "source"
        )
    _hmsd_execute_tool(
        CMD -g header -o ${CMAKE_CURRENT_BINARY_DIR}/include/generated/hmsd ${hmsd_options}
        FILES ${sd_files}
        VAR gen_hdr_files
        DESC "header"
        )
    target_sources(${_gen_TARGET} PRIVATE ${gen_src_files} ${gen_hdr_files})
    include_directories($<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>)
endfunction(hmsd_generate)

function(_hmsd_add_library libname)
    cmake_parse_arguments (PARSE_ARGV 0 _add "" "" "SOURCES")
    hmcmakeng_status("Add library: ${libname}")
    hmbuild_cmd_lib(
        CMDLIST lib;dynamic;y
        ARG ${libname}
        )
    hmbuild_cmd_src(
        CMDLIST src;${libname};y
        ARG ${_add_SOURCES}
        )
    hmbuild_cmd_linklib(
        CMDLIST linklib;${libname};y
        ARG HMLibC::HMUserLibC_shared
        )
    hmbuild_cmd_install(
        CMDLIST install;${libname};y
        ARG LIBDIR
        )
endfunction(_hmsd_add_library)

function(hmsd_test_generate)
    cmake_parse_arguments (PARSE_ARGV 0 _gen "" "DIRECTORY;SERVER" "")

    _hmsd_all_description_files(${_gen_DIRECTORY} sd_files)

    _hmsd_execute_tool(
        CMD -g test_header -o ${CMAKE_CURRENT_BINARY_DIR}/generated/hmsd
        FILES ${sd_files}
        VAR gen_test_header_files
        DESC "test header"
        )
    _hmsd_execute_tool(
        CMD -g test_script -o ${CMAKE_CURRENT_BINARY_DIR}/generated/hmsd
        FILES ${sd_files}
        VAR gen_test_script_files
        DESC "test script"
        )
    _hmsd_execute_tool(
        CMD -g test -o ${CMAKE_CURRENT_BINARY_DIR}/generated/hmsd
        FILES ${sd_files}
        VAR gen_test_files
        DESC "test source"
        DEPENDS "${gen_test_script_files}"
        )
    foreach(src ${gen_test_files})
        string(REGEX REPLACE ".*/sysif/([a-zA-Z0-9]+)/([a-zA-Z0-9]+)\.c"
            "hmsdt_\\1_\\2" libname ${src}
            )
        string(REGEX REPLACE "(.*)\.c" "\\1.h" hdr ${src})
        string(REGEX REPLACE "(.*)/sysif/([a-zA-Z0-9]+)/([a-zA-Z0-9]+)\.c"
            "\\1/hmsdtc_\\2_\\3" script ${src}
            )
        _hmsd_add_library(${libname} SOURCES ${src} ${hdr})
    endforeach()

    install(
        FILES ${gen_test_script_files}
        DESTINATION ${CONFIG_HMBUILD_INSTALL_DIR_BINDIR}/hmsdt/hmsdtc
        PERMISSIONS
            OWNER_READ OWNER_WRITE OWNER_EXECUTE
            GROUP_READ GROUP_EXECUTE
            WORLD_READ WORLD_EXECUTE
        )

    install(
        FILES "${HMCMAKENG_KBUILD_DOT_CONFIG}"
        PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
        DESTINATION ${CONFIG_HMBUILD_INSTALL_DIR_BINDIR}/hmsdt/configs
        RENAME "${_gen_SERVER}.config"
        )

    hmbuild_lib_add_all_dynamic_targets()
    hmbuild_install_all_targets()
endfunction(hmsd_test_generate)

# vim:ts=4:sw=4:expandtab
