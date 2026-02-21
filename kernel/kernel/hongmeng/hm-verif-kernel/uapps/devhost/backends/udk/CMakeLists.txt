# import cmakeng support
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(udk LANGUAGES NONE)

# import cmakeng
include(HongmengCMakeNG/Kconfig)
include(HongmengCMakeNG/PreHmbuild)

include_directories("${CMAKE_CURRENT_BINARY_DIR}/generated/include")

include(HongmengCMakeNG/HMbuild)
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS HMbuild)

if (DEFINED CONFIG_HMBUILD_MAKE_BINARIES)
    hmbuild_get_interface_target_property(udk_target dh_udk HMBUILD_CMAKETARGET_RAW)
    target_link_options(${udk_target} PRIVATE "-Wl,--version-script=${CMAKE_CURRENT_SOURCE_DIR}/scripts/libdh-udk.map")
    include(scripts/fusion.cmake)
endif()

if(CONFIG_HMBUILD_PROFILE_DEBUG)
    set(UDK_MUTEX_IMPL "UDK_RAW_MUTEX_IMPL_WITH_LOCKID")
else()
    set(UDK_MUTEX_IMPL "UDK_RAW_MUTEX_IMPL_NO_LOCKID")
endif()
file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/generated/include/udk")
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/include/udk/mutex.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/generated/include/udk/mutex.h" @ONLY)



if (DEFINED CONFIG_HMBUILD_MAKE_HEADERS)
    install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/generated/include/"
            DESTINATION "${CONFIG_HMBUILD_INSTALL_DIR_INCLUDEDIR}"
    )

    install(FILES modules/headers.list modules/prepare-headers.sh
        DESTINATION /lib/udk
        PERMISSIONS OWNER_READ OWNER_WRITE
        GROUP_READ
        WORLD_READ
    )

    install(DIRECTORY modules/scripts/cmake/
        DESTINATION /lib/udk/share/cmake/Modules
        DIRECTORY_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
                              GROUP_READ             GROUP_EXECUTE
                              WORLD_READ             WORLD_EXECUTE
    )

    set(TTY_DIR ../common/tty)
    install(FILES ${TTY_DIR}/tty.h ${TTY_DIR}/tty_api.h ${TTY_DIR}/tty_line_buf.h
        DESTINATION /usr/include/udk/tty
        COMPONENT udk_tty_headers_install
        PERMISSIONS OWNER_READ OWNER_WRITE
        GROUP_READ
        WORLD_READ
    )

    # for UDK to fetch dtb from LDK
    install(FILES ../common/sd/include/api.h
        DESTINATION /usr/include/udk/sdproxy
        PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
    )
endif()
# vim:ts=4:sw=4:expandtab
