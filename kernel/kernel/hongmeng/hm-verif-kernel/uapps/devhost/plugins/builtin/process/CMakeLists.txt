# import cmakeng support
if(NOT DEFINED HMBUILD_LEGACY_CMAKE_LIBRARY)
	add_link_options("-L${CMAKE_CURRENT_SOURCE_DIR}")
	include(HongmengCMakeNG/HMbuild)
	return()
endif()

add_devhost_builtin_plugin(NAME process
	SRC_LIST
	process_if.c
	)
