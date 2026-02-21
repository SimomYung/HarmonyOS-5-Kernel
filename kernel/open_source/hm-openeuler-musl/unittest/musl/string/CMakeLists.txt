add_compile_options(-Wno-unused-parameter)

add_compile_definitions(
    CONFIG_MODULES=y
    CONFIG_LIBLINUX=y
    )

# set var
set(STRING_SOURCE_DIR src)
set(STRING_SOURCE_LINK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${STRING_SOURCE_DIR})

# copmpile and archive string to lib
include(${CMAKE_CURRENT_SOURCE_DIR}/hmut-string.cmake)

# compile string ut testcase
add_subdirectory(tests)
