# DO NOT CHANGE BELOW
# Pull in SDK
if (NOT _HMSDKSYSROOTPATH OR NOT _HMSDKLIB OR NOT _HMSDKINCLUDE)
    set(USE_STAGING FALSE)
    if (SUBARCH STREQUAL "be")
        set(be "y")
    else()
        set(be " ")
    endif()
    execute_process(
        COMMAND bash ${PROJECT_SOURCE_DIR}/scripts/hmsdk-sysroot.sh ${ARCH} ${be} sysroot
        OUTPUT_VARIABLE hmsysroot
        OUTPUT_STRIP_TRAILING_WHITESPACE
        )
    execute_process(
        COMMAND bash ${PROJECT_SOURCE_DIR}/scripts/hmsdk-sysroot.sh ${ARCH} ${be} target
        OUTPUT_VARIABLE hmtarget
        OUTPUT_STRIP_TRAILING_WHITESPACE
        )
    if (NOT EXISTS ${PROJECT_BINARY_DIR}/.SDK)
        message(FATAL_ERROR "The SDK directory does not exist. Download it and try again.")
    else()
        set(HMSDKSYSROOTPATH ${PROJECT_BINARY_DIR}/.SDK/sysroots/${hmsysroot})
    endif()
    set(HMSDKINCLUDE ${HMSDKSYSROOTPATH}/usr/include)
    set(HMSDKLIB ${HMSDKSYSROOTPATH}/usr/lib)
    set(LIBLINUX_KDIR ${HMSDKSYSROOTPATH}/lib/modules)
else()
    set(USE_STAGING TRUE)
    set(HMSDKSYSROOTPATH ${_HMSDKSYSROOTPATH})
    set(HMSDKINCLUDE ${_HMSDKINCLUDE})
    set(HMSDKLIB ${_HMSDKLIB})
    set(LIBLINUX_KDIR ${_LIBLINUX_KDIR})
endif()

# Add sysif apis sdk path
set(HMSDKSYSIFCLIENT ${HMSDKINCLUDE}/generated/sysif_client)
set(HMSDKSYSIFSERVER ${HMSDKINCLUDE}/generated/sysif_server)
set(HMSDKSYSIFBASE ${HMSDKINCLUDE}/generated/sysif_base)

# Add SDK cmake module path
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${HMSDKSYSROOTPATH}/usr/share/cmake/Modules")
