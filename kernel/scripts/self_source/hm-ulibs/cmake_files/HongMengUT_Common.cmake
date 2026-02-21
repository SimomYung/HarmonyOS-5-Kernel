set(HW_CMAKE_VERSION_MIN 3.12.1) # Should equal BEST most of time
set(HW_CMAKE_VERSION_BEST 3.20.5)

cmake_minimum_required(VERSION ${HW_CMAKE_VERSION_MIN} FATAL_ERROR)
if (CMAKE_VERSION VERSION_LESS ${HW_CMAKE_VERSION_BEST})
    message(WARNING "Non-Preference cmake version ${CMAKE_VERSION}."
        "Please install cmake-${HW_CMAKE_VERSION_BEST}")
endif()
if (CMAKE_VERSION VERSION_GREATER ${HW_CMAKE_VERSION_BEST})
    message(WARNING "Non-Preference cmake version ${CMAKE_VERSION}."
        "Please install cmake-${HW_CMAKE_VERSION_BEST}")
endif()

function(hmut_append_definitions target)
    set(flags ${ARGN})
    list(LENGTH flags num_flags)
    if (num_flags GREATER 0)
        foreach (f ${flags})
            target_compile_definitions(${target} PRIVATE ${f})
        endforeach()
    endif()
endfunction()

function(hmut_append_definitions_auto target prefix kconf_prefix)
    get_cmake_property(_vars VARIABLES)
    string(REGEX MATCHALL "(^|;)${prefix}[A-Za-z0-9_]*" _matchedVars "${_vars}")
    list(SORT _vars)
    foreach (_v ${_matchedVars})
        get_property(_type CACHE "${_v}" PROPERTY TYPE)
        if (NOT "${_type}" STREQUAL "STATIC")
            if ("${_type}" STREQUAL "STRING")
                hmut_append_definitions(${target} "${_v}=${${kconf_prefix}${_v}}")
            elseif ("${_type}" STREQUAL "BOOL" AND ${_v})
                hmut_append_definitions(${target} ${_v})
            endif()
        endif()
    endforeach()
endfunction()

# set wrap symbols
function(hmut_target_wrap_options target)
    cmake_parse_arguments(
        HMUTLINKOPS
        ""
        ""
        "WRAPGROUP;LINKOPTIONS"
        ${ARGN}
    )
    #set wrap symbols group
    if(HMUTLINKOPS_WRAPGROUP)
        set(wrap_op "-Wl,--wrap")
        foreach (wrap_s ${HMUTLINKOPS_WRAPGROUP})
            get_target_property(old_link_flags ${target} LINK_FLAGS)
            if (old_link_flags)
                set_target_properties(${target} PROPERTIES
                    LINK_FLAGS "${old_link_flags} ${wrap_op}=${wrap_s}")
            else()
                set_target_properties(${target} PROPERTIES
                    LINK_FLAGS "${wrap_op}=${wrap_s}")
            endif()
        endforeach()
    endif()
endfunction()

# Set target link properties
function(hmut_target_link_options target)
    set(flags ${ARGN})
    list(LENGTH flags num_flags)
    if (num_flags GREATER 0)
        get_target_property(old_link_flags ${target} LINK_FLAGS)
        if (old_link_flags)
            set_target_properties(${target} PROPERTIES
                LINK_FLAGS "${old_link_flags} ${flags}")
        else()
            set_target_properties(${target} PROPERTIES
                LINK_FLAGS "${flags}")
        endif()
    endif()
endfunction()

function(hmut_target_link_libraries target)
    cmake_parse_arguments(
        HMUTLIB
        "DYNAMIC"
        ""
        "LINKGROUP;OBJECTS;WHOLEARCHIVE;LIBRARIES"
        ${ARGN}
    )

    set(flags ${HMUTLIB_LIBRARIES})
    list(LENGTH flags num_flags)
    if (num_flags GREATER 0)
        foreach (f ${flags})
            target_link_libraries(${target} PRIVATE ${f})
        endforeach()
    endif()

    # link group
    target_link_libraries(${target} PRIVATE
        -Wl,--start-group
            ${HMUTLIB_LINKGROUP}
        -Wl,--end-group
    )
endfunction()

macro(warn_on condition comment)
    if (${condition})
        message(WARNING ${comment})
    endif()
endmacro()

macro(warn_on_not condition comment)
    if (NOT ${condition})
        message(WARNING ${comment})
    endif()
endmacro()

function(hmut_find_c_compiler_component comp_name comp_path)
    set(cmd_opt "--print-file-name=${comp_name}")
    execute_process(
        COMMAND ${CMAKE_C_COMPILER} ${cmd_opt}
        RESULT_VARIABLE ret
        OUTPUT_VARIABLE path
        OUTPUT_STRIP_TRAILING_WHITESPACE)
    warn_on(ret "${CMAKE_C_COMPILER} cannot tell where is ${comp_name}")
    warn_on_not(path "The ${comp_name} path is invalid")
    set(${comp_path} ${path} PARENT_SCOPE)
endfunction()

# copy target file from src to dest
function(hmut_copy_target_file target src_path src_file dest_path dest_file)
    add_custom_command(
        TARGET ${target} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${src_path}/${src_file}
                ${dest_path}/${dest_file})
endfunction()
