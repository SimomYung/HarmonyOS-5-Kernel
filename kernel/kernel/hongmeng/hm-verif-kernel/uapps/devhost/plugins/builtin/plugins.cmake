set (DEVHOST_INC ${DEVHOST_SOURCE_DIR}
		 ${DEVHOST_SOURCE_DIR}/include
		 ${DEVHOST_SOURCE_DIR}/include/api
		 ${DEVHOST_SOURCE_DIR}/include/common)

function(add_devhost_builtin_plugin)
	cmake_parse_arguments(
		DH_PLUGIN
		""
		"NAME"
		"SRC_LIST"
		${ARGN}
	)

	hm_add_library(${DH_PLUGIN_NAME} STATIC SSTACK_MEM
		SOURCES
		${DH_PLUGIN_SRC_LIST}

		PRIVATE_INCLUDES
		${DEVHOST_INC}
		${HMSDKINCLUDE}
		${CMAKE_CURRENT_SOURCE_DIR}
	)
	set_property(TARGET ${DH_PLUGIN_NAME} APPEND_STRING
		PROPERTY COMPILE_FLAGS "-fvisibility=hidden -D_DH_PLUGIN_NAME=${DH_PLUGIN_NAME}"
		)
	set(DEVHOST_BUILTIN_PLUGINS ${DEVHOST_BUILTIN_PLUGINS} ${DH_PLUGIN_NAME} CACHE STRING "" FORCE)
endfunction(add_devhost_builtin_plugin)
