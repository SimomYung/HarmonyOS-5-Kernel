set(NOLSE_OPT "")
if (CONFIG_AARCH64_V8_1)
    set(NOLSE_OPT "-march=armv8.1-a+nolse")
endif()

if (CONFIG_SNAPSHOT_COMPRESS)
    add_hm_library(kbox_snapshot.c)
endif()

add_hm_library(kbox.c)
append_dir_flag("-I${HM_VERIFY_KERNEL}/sysmgr/ ${NOLSE_OPT}")
