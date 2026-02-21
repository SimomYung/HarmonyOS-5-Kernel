# import cmakeng support
if(NOT DEFINED HMBUILD_LEGACY_CMAKE_LIBRARY)
	include(HongmengCMakeNG/HMbuild)
	return()
endif()

add_devhost_builtin_plugin(NAME process
	SRC_LIST
	process_if.c
	)
