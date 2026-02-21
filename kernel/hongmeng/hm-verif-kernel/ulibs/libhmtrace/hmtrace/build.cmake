add_hm_library(trace_class_api.c)
add_hm_library(trace_event_api.c)
add_hm_library(trace_buffer_api.c)
add_hm_library(trace_efmt_api.c)
add_hm_library(trace_iter_ops.c)

if (CONFIG_HMTRACE)
	include(${CMAKE_CURRENT_LIST_DIR}/core/build.cmake)
endif()
