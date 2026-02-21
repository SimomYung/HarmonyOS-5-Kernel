include(CheckCCompilerFlag)

# Parse kconfig file
function(parse_kconfig kconfig_config prefix)
    file(
        STRINGS
        ${kconfig_config}
        kconf
        REGEX "^[^#]"
        )

    foreach (kv ${kconf})
        string(GENEX_STRIP ${kv} kv_stripped)
        string(REGEX MATCH "^[^= \t\r\n]+" conf_name ${kv_stripped})
        string(REGEX MATCH "[^= \t\r\n]+$" conf_value ${kv_stripped})
        if (DEFINED conf_value AND
                NOT conf_value MATCHES "\"[ \t\r\n]*\"")
            string(REGEX MATCH "[^\"]+" conf_value_raw ${conf_value})
            set("${prefix}${conf_name}" "${conf_value_raw}" PARENT_SCOPE)
        else()
            set("${prefix}${conf_name}" NOTFOUND PARENT_SCOPE)
        endif()
    endforeach()
endfunction()
