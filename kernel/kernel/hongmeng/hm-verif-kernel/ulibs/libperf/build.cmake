if (CONFIG_PERF_INTERNAL_PROFILING)
	add_hm_library(internal-profiling/perf.c)
	add_hm_library(internal-profiling/arm_pmu_common.c)
endif()

if (ARCH STREQUAL "aarch64")
        add_hm_library(pmuv3.c)
endif()

if (ARCH STREQUAL "arm")
        add_hm_library(pmuv1.c)
endif()

add_hm_library(dsu_pmu.c)
add_hm_library(perf.c)
