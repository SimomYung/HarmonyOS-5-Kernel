if (NOT HMSDKPATH)
	message(FATAL_ERROR "HMSDKPATH is not installed.")
endif()

execute_process(
	COMMAND bash ${HM_VERIFY_KERNEL}/scripts/hmsdk-sysroot.sh ${ARCH} ${CONFIG_BIG_ENDIAN}
	OUTPUT_VARIABLE sysrootpath)
string(STRIP ${sysrootpath} sysrootpath)

if (NOT HMSDKSYSROOTPATH)
	set(HMSDKSYSROOTPATH "${HMSDKPATH}/sysroots/${sysrootpath}")
endif()
if (NOT HMSDKINCLUDE)
	set(HMSDKINCLUDE "${HMSDKSYSROOTPATH}/usr/include")
endif()
if (NOT HMSDKLIB)
	set(HMSDKLIB "${HMSDKSYSROOTPATH}/usr/lib")
endif()

# Add sysif apis sdk path
set(HMSDKSYSIFCLIENT "${HMSDKINCLUDE}/generated/sysif_client")
set(HMSDKSYSIFSERVER "${HMSDKINCLUDE}/generated/sysif_server")
set(HMSDKSYSIFBASE "${HMSDKINCLUDE}/generated/sysif_base")
