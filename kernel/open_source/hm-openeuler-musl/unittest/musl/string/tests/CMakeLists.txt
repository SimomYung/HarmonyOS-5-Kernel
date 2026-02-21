set(HMUT_MUSL_STRING_TARGET hmut_musl_string.elf)

message("log:"${HMSDKLIB})
message("log:"${HMSDKINCLUDE})

link_directories(
	${UTCMOCKALIB}
	${CMAKE_SOURCE_DIR}/build/musl/string
	${HMSDKLIB}
	${HMSDK}/lib
	)

aux_source_directory(. MUSL_STRING_DIR)
add_executable(${HMUT_MUSL_STRING_TARGET}
	${MUSL_STRING_DIR}
	)

# Add inlcude directory
target_include_directories(
	${HMUT_MUSL_STRING_TARGET}
	PRIVATE
	${UTCMOCKAINCLUDE}
	${UT_HM_HEADERS}
	#musl
	${HM_LIBC}/include
	# hm
	${HM_LIBC}/hm
	${HM_LIBC}/hm/hmlibc
	# bits
	${CMAKE_SOURCE_DIR}/${HMLIBC_BUILD_DIR}/usr/include/
	${HMSDKINCLUDE}
	)

# Set static link
set(CMAKE_EXE_LINKER_FLAGS -static)

# get libc.a and libgcov.a in cross-compiler
hmut_find_c_compiler_component(libc.a COMPILER_LIBC_PATH)
hmut_find_c_compiler_component(libgcov.a COMPILER_LIBGCOV_PATH)

target_compile_options(${HMUT_MUSL_STRING_TARGET} PRIVATE -Wno-unused-parameter)

hmut_target_link_libraries(${HMUT_MUSL_STRING_TARGET} STATIC
	LIBRARIES
	LINKGROUP
	cmocka-static
	${LIBHMUT_MUSL_STRING}
	${COMPILER_LIBC_PATH} # link libc.a in cross-compile
	${COMPILER_LIBGCOV_PATH} # link libgcov.a in cross-compile
	)

# set wrap symbols
hmut_target_wrap_options(${HMUT_MUSL_STRING_TARGET}
	WRAPGROUP
	)

hmut_copy_target_file(${HMUT_MUSL_STRING_TARGET}
	${CMAKE_CURRENT_BINARY_DIR} ${HMUT_MUSL_STRING_TARGET}
	${CMAKE_SOURCE_DIR}/${CMAKE_BUILD_DIR} ${HMUT_MUSL_STRING_TARGET}
	)
