set (LIBHMUT_MUSL_STRING hmut-musl-string)

add_library(${LIBHMUT_MUSL_STRING} STATIC
	${STRING_SOURCE_DIR}/strcmp.c
	${STRING_SOURCE_DIR}/memcmp.c
	)

target_include_directories(
	${LIBHMUT_MUSL_STRING} PRIVATE
	${HMSDKINCLUDE}
	)

# ut_syscall
target_include_directories(
	${LIBHMUT_MUSL_STRING} PRIVATE
	${CMAKE_SOURCE_DIR}/${HMLIBC_BUILD_DIR}/usr/include/
	)

target_compile_options(${LIBHMUT_MUSL_STRING} PRIVATE -fprofile-arcs -ftest-coverage)
