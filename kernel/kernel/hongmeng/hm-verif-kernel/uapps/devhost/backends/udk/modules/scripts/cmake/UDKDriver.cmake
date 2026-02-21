# Build UDK drivers
include_guard(GLOBAL)
include(UDKDriverBundle)

#
# Currently this function implicitly depends on HongMengCommon.
# Add include_guard to HongMengCommon to make `include' correct.
#
function(udk_driver_bundle target)
    cmake_parse_arguments(
        UDK
        ""
        "LIBS_INSTALL_PATH;HEADERS_INSTALL_PATH"
        "PUBLIC_INCLUDES"
        ${ARGN}
        )
    hm_add_library(${target} SHARED ${ARGN} NO_INSTALL)
    # Redundant setup if it will also be done in make_bundle
    udk_driver_common_setup(${target})

    if (NOT UDK_LIBS_INSTALL_PATH)
        set(UDK_LIBS_INSTALL_PATH ${LIBS_INSTALL_DIR})
    endif()

    if (NOT UDK_HEADERS_INSTALL_PATH)
        set(UDK_HEADERS_INSTALL_PATH ${HEADERS_INSTALL_DIR})
    endif()

    if (UDK_INCLUDE_INTERNAL)
        target_include_directories(${target} PRIVATE ${UDK_INCLUDE_INTERNAL})
    endif()

    if (UDK_PREINCL_INTERNAL)
      target_include_directories(${target} BEFORE PRIVATE ${UDK_PREINCL_INTERNAL})
    endif()

    if (UDK_DEPLIBS_INTERNAL)
        target_link_libraries(${target} PRIVATE ${UDK_DEPLIBS})
    endif()

    # A special component for install public headers ONLY!
    # It is excluded from the default 'make install'.
    foreach (inc ${UDK_PUBLIC_INCLUDES})
      get_filename_component(abs_inc ${inc} ABSOLUTE)
      if (IS_DIRECTORY "${abs_inc}")
        install(DIRECTORY ${inc}
          DESTINATION ${UDK_HEADERS_INSTALL_PATH}
          COMPONENT headers_install
          EXCLUDE_FROM_ALL
        )
        install(DIRECTORY ${inc}
          DESTINATION ${UDK_HEADERS_INSTALL_PATH}
          COMPONENT ${target}_headers_install
          EXCLUDE_FROM_ALL
        )
      else()
        install(FILES ${inc}
          DESTINATION ${UDK_HEADERS_INSTALL_PATH}
          COMPONENT headers_install
          EXCLUDE_FROM_ALL
          PERMISSIONS OWNER_READ OWNER_WRITE
          GROUP_READ
          WORLD_READ
        )
        install(FILES ${inc}
          DESTINATION ${UDK_HEADERS_INSTALL_PATH}
          COMPONENT ${target}_headers_install
          EXCLUDE_FROM_ALL
          PERMISSIONS OWNER_READ OWNER_WRITE
          GROUP_READ
          WORLD_READ
        )
      endif()
    endforeach()

    udk_driver_make_bundle(${target})
endfunction()

macro(udk_driver_setup)
    cmake_parse_arguments(
        UDK
        ""
        ""
        "INCLUDE;PREINCL;DEPLIBS"
        ${ARGN}
    )
    set(UDK_INCLUDE_INTERNAL "${UDK_INCLUDE}" CACHE INTERNAL
        "Last value of UDK_INCLUDE_INTERNAL")
    set(UDK_PREINCL_INTERNAL "${UDK_PREINCL}" CACHE INTERNAL
        "Last value of UDK_PREINCL_INTERNAL")
    set(UDK_DEPLIBS_INTERNAL "${UDK_DEPLIBS}" CACHE INTERNAL
        "Last value of UDK_DEPLIBS_INTERNAL")
endmacro()
