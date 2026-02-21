if (NOT KCONFIG_PATH)
	message(FATAL_ERROR "Not setting varaible 'KCONFIG_PATH'")
endif()
execute_process(
	COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/include/generated)
execute_process(
	COMMAND bash ${HM_VERIFY_KERNEL}/scripts/cmake/conf_expand.sh
	${KCONFIG_PATH} ${CMAKE_CURRENT_BINARY_DIR}/config.cmake
	${CMAKE_CURRENT_BINARY_DIR}/include/generated/autoconf.h)

if (EXISTS "${CMAKE_CURRENT_BINARY_DIR}/config.cmake")
	include(${CMAKE_CURRENT_BINARY_DIR}/config.cmake)
elseif()
	message(FATAL_ERROR "failed to create config.cmake")
endif()

# Define some functions and macroes
function(add_objs objs)
	set(OBJS "${OBJS} ${objs}" PARENT_SCOPE)
endfunction()

macro(add_hm_library source_file)
	set(LIBRARY_SRCS ${LIBRARY_SRCS} ${CMAKE_CURRENT_LIST_DIR}/${source_file})
	#	set(LIBRARY_SRCS ${LIBRARY_SRCS} PARENT_SCOPE)
endmacro()

macro(append_src_flag source_file compile_flag)
	set_source_files_properties(${CMAKE_CURRENT_LIST_DIR}/${source_file} PROPERTIES COMPILE_FLAGS ${compile_flag})
endmacro()

macro(append_dir_flag compile_flag)
	file(GLOB_RECURSE SRCS ${CMAKE_CURRENT_LIST_DIR}/*.c)
	foreach (src ${SRCS})
		set_source_files_properties(${src} PROPERTIES COMPILE_FLAGS ${compile_flag})
	endforeach()
endmacro()

macro(append_link_flags target)
    set(flags ${ARGN})
    list(LENGTH flags num_flags)
    if (num_flags GREATER 0)
        foreach (f ${flags})
            get_target_property(old_link_flags ${target} LINK_FLAGS)
            if (old_link_flags)
                set_target_properties(${target} PROPERTIES
                    LINK_FLAGS "${old_link_flags} ${f}")
            else()
                set_target_properties(${target} PROPERTIES
                    LINK_FLAGS "${f}")
            endif()
        endforeach()
    endif()
endmacro()

include(${HM_VERIFY_KERNEL}/scripts/cmake/compiler.cmake)
include(CheckCCompilerFlag)
