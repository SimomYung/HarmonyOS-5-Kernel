# Post-process UDK driver targets to contain
# extra ingredients
include_guard(GLOBAL)

if(DEFINED BUILD_USE_HMBUILD)
    include(HongmengCMakeNG/Kconfig)
endif()
if(DEFINED CONFIG_HMBUILD_STRIP_OPTION)
    set(UDK_STRIP_OPTION ${CONFIG_HMBUILD_STRIP_OPTION})
elseif(DEFINED CONFIG_STRIP_OPTION)
    set(UDK_STRIP_OPTION ${CONFIG_STRIP_OPTION})
else()
    set(UDK_STRIP_OPTION "--strip-debug")
endif()

set(udk_cpio_options "-o;-H;newc;-R;0:0;--quiet" CACHE STRING "udk cpio options")

# Test whether cpio supports "reproducible" in case the downstream
# uses old versions of cpio
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/udk_cpio_test "test")
execute_process(
    COMMAND echo udk_cpio_test
    COMMAND cpio ${udk_cpio_options} --reproducible
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    RESULT_VARIABLE test_result
    OUTPUT_QUIET
    ERROR_QUIET
)
file(REMOVE ${CMAKE_CURRENT_BINARY_DIR}/udk_cpio_test)
if (NOT test_result)
    list(APPEND udk_cpio_options "--reproducible")
endif()

#
# The template file should reside with this cmake file.
# Use NO_CMAKE_FIND_ROOT_PATH to override the configuration
# in the default toolchain.cmake by yocto.
#
find_file(udk_driver_template
    udk_driver_drv.c.in
    PATHS "${CMAKE_CURRENT_LIST_DIR}"
    NO_CMAKE_FIND_ROOT_PATH
)

function(udk_driver_bundle_append drv bundle file tmpdir)
    get_filename_component(file_abs "${file}" DIRECTORY)
    get_filename_component(file_name "${file}" NAME)

    add_custom_command(TARGET "${drv}"
        COMMAND
        ${CMAKE_COMMAND} -E copy ${file} ${tmpdir}
        COMMAND
        ${CMAKE_COMMAND} -E chdir ${tmpdir} chmod 0444 ${file_name}
        COMMAND
        ${CMAKE_COMMAND} -E chdir ${tmpdir} touch -d @0.0 ${file_name}
        COMMAND
        ${CMAKE_COMMAND} -E chdir ${tmpdir}
        sh -c 'echo ${file_name} | cpio --append ${udk_cpio_options} --file ${bundle}'
        DEPENDS
        ${bundle}
    )
endfunction()

#
# Set common flags for udk driver targets
#
function(udk_driver_common_setup target)
    target_compile_definitions(${target} PRIVATE UDK_MODULE)
    set_property(TARGET ${target} PROPERTY PREFIX "")
endfunction()

#
# Generate signature of dev driver file
#
function(udk_driver_bundle_gen_sigature drv file)
    get_filename_component(file_abs "${file}" DIRECTORY)
    get_filename_component(drv_name "$<TARGET_LINKER_FILE_NAME:${drv}>" NAME)
    # sha256
    add_custom_command(TARGET "${drv}" POST_BUILD
        COMMAND
        echo sha256sum > ${file} &&
        sha256sum ${drv_name} | cut -d ' ' -f 1 >> ${file}
    )
endfunction()

#
# Method: udk_driver_make_bundle([tgt...])
#
# Specify one or more targets of UDK drivers to process.
# This method will append a generated C source to compilation.
# Afterwards, a zip file will be generated to contain the object
# file and manifest files.
#
function(udk_driver_make_bundle)
    set(drivers ${ARGN})
    list(LENGTH drivers num_drivers)
    if (num_drivers GREATER 0)
        foreach(drv ${drivers})
            set(drv_helper ${CMAKE_CURRENT_BINARY_DIR}/${drv}.drv.c)
            set(bundle_tmp_dir "${CMAKE_CURRENT_BINARY_DIR}/${drv}_bundle_tmp")
            set(bundle_meta_dir "${CMAKE_CURRENT_BINARY_DIR}/${drv}_bundle_meta")

            configure_file("${udk_driver_template}" "${drv_helper}" @ONLY)

            target_sources(${drv} PRIVATE ${drv_helper})

            udk_driver_common_setup(${drv})

            get_target_property(output_name ${drv} OUTPUT_NAME)
            add_custom_command(TARGET ${drv} POST_BUILD
                COMMAND
                mkdir -p ${bundle_tmp_dir}
                COMMAND
                rm -rf ${bundle_tmp_dir}/*
                COMMAND
                ${CMAKE_STRIP} -D ${UDK_STRIP_OPTION} $<TARGET_LINKER_FILE:${drv}> -o ${bundle_tmp_dir}/$<TARGET_LINKER_FILE_NAME:${drv}>
                COMMAND
                ${CMAKE_COMMAND} -E chdir ${bundle_tmp_dir} chmod 0444 $<TARGET_LINKER_FILE_NAME:${drv}>
                COMMAND
                ${CMAKE_COMMAND} -E chdir ${bundle_tmp_dir} touch -d @0.0 $<TARGET_LINKER_FILE_NAME:${drv}>
                COMMAND
                ${CMAKE_COMMAND} -E chdir ${bundle_tmp_dir}
                sh -c 'echo $<TARGET_LINKER_FILE_NAME:${drv}> | cpio ${udk_cpio_options} --file ${CMAKE_CURRENT_BINARY_DIR}/${output_name}.udk'
                COMMAND
                mkdir -p ${bundle_meta_dir}
                COMMAND
                rm -rf ${bundle_meta_dir}/*
                COMMAND
                touch ${bundle_meta_dir}/compatible &&
                bash -c '${CMAKE_OBJCOPY} --dump-section ".udk_module_compat=${bundle_meta_dir}/compatible" $<TARGET_LINKER_FILE:${drv}> || true' &&
                sed -i 's/\\x00\\+/\\x00/g' ${bundle_meta_dir}/compatible
                COMMAND
                touch ${bundle_meta_dir}/dependencies &&
                bash -c '${CMAKE_OBJCOPY} --dump-section ".udk_module_deps=${bundle_meta_dir}/dependencies" $<TARGET_LINKER_FILE:${drv}> || true' &&
                sed -i 's/\\x00\\+/\\x00/g' ${bundle_meta_dir}/dependencies
                BYPRODUCTS
                "${output_name}.udk"
                "${drv}_bundle_meta"
                "${drv}_bundle_tmp"
            )
            udk_driver_bundle_gen_sigature(
                "${drv}"
                "${bundle_meta_dir}/$<TARGET_LINKER_FILE_NAME:${drv}>.sig"
            )
            udk_driver_bundle_append(
                "${drv}"
                "${CMAKE_CURRENT_BINARY_DIR}/${output_name}.udk"
                "${bundle_meta_dir}/compatible"
                "${bundle_tmp_dir}"
            )
            udk_driver_bundle_append(
                "${drv}"
                "${CMAKE_CURRENT_BINARY_DIR}/${output_name}.udk"
                "${bundle_meta_dir}/dependencies"
                "${bundle_tmp_dir}"
            )
            udk_driver_bundle_append(
                "${drv}"
                "${CMAKE_CURRENT_BINARY_DIR}/${output_name}.udk"
                "${bundle_meta_dir}/$<TARGET_LINKER_FILE_NAME:${drv}>.sig"
                "${bundle_tmp_dir}"
            )
            if (UDK_LIBS_INSTALL_PATH)
                install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${output_name}.udk"
                    DESTINATION ${UDK_LIBS_INSTALL_PATH}
                    PERMISSIONS OWNER_READ OWNER_WRITE
                    GROUP_READ
                    WORLD_READ
                )
            elseif (CONFIG_UDK_LIBS_INSTALL_PATH)
                install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${output_name}.udk"
                    DESTINATION ${CONFIG_UDK_LIBS_INSTALL_PATH}
                    PERMISSIONS OWNER_READ OWNER_WRITE
                    GROUP_READ
                    WORLD_READ
                )
            endif()
        endforeach()
    endif()
endfunction()
