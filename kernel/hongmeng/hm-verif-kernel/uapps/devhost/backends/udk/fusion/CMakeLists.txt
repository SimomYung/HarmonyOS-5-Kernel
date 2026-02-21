# Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
# Description: HMbuild for udk fusion mode
# Author: Huawei OS Kernel Lab
# Create: Fri May 19 09:51:30 2023

if(NOT DEFINED CONFIG_HMBUILD_MAKE_BINARIES)
    return()
endif()

include(HongmengCMakeNG/Kconfig)
include(HongmengCMakeNG/PreHmbuild)

hmbuild_config_target(udk.o PROPERTY HMBUILD_LINK_WHOLE_BUILTIN TRUE)
hmbuild_config_target(udk.o PROPERTY HMBUILD_NO_DEFAULT_LINK TRUE)

include(HongmengCMakeNG/HMbuild)

hmbuild_get_interface_target_property(__target udk.o HMBUILD_CMAKETARGET_RAW)
if(CONFIG_UDK_FUSION_KCFI)
    target_compile_options(${__target} PRIVATE ${CONFIG_UDK_FUSION_KCFI_CFLAGS})
endif()
set(KASAN_CFLAGS_LIST ${CONFIG_UDK_FUSION_KASAN_CFLAGS})
separate_arguments(KASAN_CFLAGS_LIST)
while(KASAN_CFLAGS_LIST)
    list(POP_FRONT KASAN_CFLAGS_LIST out_first)
    if((out_first STREQUAL "-mllvm") OR (out_first STREQUAL "--param"))
        list(POP_FRONT KASAN_CFLAGS_LIST out_second)
        target_compile_options(${__target} PRIVATE "SHELL:${out_first} ${out_second}")
    else()
        target_compile_options(${__target} PRIVATE "${out_first}")
    endif()
endwhile()

hmbuild_target_cmakename_of(udk_a udk)
set(udk_o udk.o.unstripped)
set(udk_symbol_file ${CMAKE_BINARY_DIR}/udk.symbols)

add_dependencies(${udk_a} ${udk_o})
add_custom_command(TARGET ${udk_a} POST_BUILD
    COMMAND sh -c "echo udk_fusion_entry > ${udk_symbol_file}"
    COMMAND sh -c "echo udk_pm_action >> ${udk_symbol_file}"
    COMMAND sh -c "echo libdh_blk_submit_bio >> ${udk_symbol_file}"
    COMMAND sh -c "echo libdh_blk_submit_cmd >> ${udk_symbol_file}"
    COMMAND sh -c "echo libdh_blk_bio_polling >> ${udk_symbol_file}"
    COMMAND sh -c "echo libdh_dev_open >> ${udk_symbol_file}"
    COMMAND sh -c "echo libdh_dev_close >> ${udk_symbol_file}"
    COMMAND sh -c "echo libdh_dev_ioctl >> ${udk_symbol_file}"
    COMMAND sh -c "echo libdh_dev_read >> ${udk_symbol_file}"
    COMMAND sh -c "echo libdh_dev_write >> ${udk_symbol_file}"
    COMMAND sh -c "echo libdh_dev_read_iter >> ${udk_symbol_file}"
    COMMAND sh -c "echo libdh_dev_write_iter >> ${udk_symbol_file}"
    COMMAND sh -c "echo libdh_dev_poll >> ${udk_symbol_file}"
    COMMAND sh -c "echo libdh_dev_epoll_ctl >> ${udk_symbol_file}"
    COMMAND sh -c "echo libdh_dev_llseek >> ${udk_symbol_file}"
    COMMAND sh -c "echo libdh_dev_fcntl >> ${udk_symbol_file}"
    COMMAND sh -c "echo libdh_dev_mmap >> ${udk_symbol_file}"
    COMMAND sh -c "echo libdh_dev_munmap >> ${udk_symbol_file}"
    COMMAND sh -c "echo hmfs_blk_busyidle_event_subscribe >> ${udk_symbol_file}"
    COMMAND sh -c "echo hmfs_blk_busyidle_event_unsubscribe >> ${udk_symbol_file}"
    COMMAND sh -c "echo disable_ufs_crypto_get >> ${udk_symbol_file}"
    COMMAND sh -c "echo disable_ufs_crypto_set >> ${udk_symbol_file}"
    COMMAND sh -c "echo dma_buf_ext_get >> ${udk_symbol_file}"
    COMMAND sh -c "echo dma_buf_ext_get_iovec >> ${udk_symbol_file}"
    COMMAND sh -c "echo dma_buf_ext_put_iovec >> ${udk_symbol_file}"
    COMMAND sh -c "echo dma_buf_ext_put >> ${udk_symbol_file}"
    COMMAND sh -c "echo udk_device_is_enable_dm >> ${udk_symbol_file}"
    COMMAND sh -c "echo udk_device_is_enable_loop_nolock >> ${udk_symbol_file}"
    COMMAND sh -c "echo loop_device_get_file_data_nolock >> ${udk_symbol_file}"
    COMMAND ${CMAKE_OBJCOPY} --keep-global-symbols=${udk_symbol_file} $<TARGET_FILE:${udk_o}>
    COMMAND ${CMAKE_AR} cDPrST $<TARGET_FILE:${udk_a}> $<TARGET_FILE:${udk_o}>
    VERBATIM
)
