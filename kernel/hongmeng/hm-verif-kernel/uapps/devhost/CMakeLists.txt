cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(devhost LANGUAGES NONE)

# add local cmake modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/scripts")

# import cmakeng
include(HongmengCMakeNG/Kconfig)
include(HongmengCMakeNG/PreHmbuild)

# temporary hack for global include
include_directories(include)
include_directories(include/api)
include_directories(include/common)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

####### Import configs from Kconfig
set(CONFIG_NET_IO_URING ${KCONFIG_CONFIG_NET_IO_URING} CACHE BOOL "net io_uring")

# devhost need export for dlopen
add_link_options(-Wl,--export-dynamic)

# temporary hack for -nostdlib link
add_link_options(-nodefaultlibs)

# temporary hack for lds library path
add_link_options(-L${CMAKE_CURRENT_SOURCE_DIR})

# temporary hack for HMbuild triggered reconfig
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS HMbuild)

# devhost.elf need --whole-archive for builtin targets
hmbuild_config_target(devhost.elf PROPERTY HMBUILD_LINK_WHOLE_BUILTIN TRUE)

if (NOT DEFINED CONFIG_HMBUILD_MAKE_BINARIES)
    include(HongmengCMakeNG/HMbuild)
else()
    include(HongmengCMakeNG/PreHmbuild)
    include(HongmengCMakeNG/PreSecuritybuild)
    include(HongmengCMakeNG/HMbuild)
    include(HongmengCMakeNG/SecurityDescription)

    hmbuild_get_interface_target_property(devhost devhost.elf HMBUILD_CMAKETARGET_RAW)
    if (CONFIG_SEHARMONY)
        set (HMSD_OPTIONS SEHARMONY)
    endif()
    hmsd_generate(
        TARGET ${devhost}
        DIRECTORY plugins/builtin/security/hmsd
        ${HMSD_OPTIONS}
        TREND ${CONFIG_HMSD_TREND}
        )
endif()

install(FILES
    ${CMAKE_SOURCE_DIR}/manifest/devhost.elf.manifest
    ${CMAKE_SOURCE_DIR}/manifest/devhost.elf-corable.manifest
    ${CMAKE_SOURCE_DIR}/manifest/devhost.elf-spin.manifest
    DESTINATION "${CONFIG_HMBUILD_INSTALL_DIR_CONFDIR}/manifest"
    COMPONENT manifest_install
)
