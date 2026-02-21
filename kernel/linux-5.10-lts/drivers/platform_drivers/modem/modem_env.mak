GBB_CUST_NAME ?= $(gbb_cust_name)

ifeq ($(chip_type),)
    GBB_CHIP_TYPE := cs
else
    GBB_CHIP_TYPE := $(chip_type)
endif
$(warning "[titan]==cust_config")
$(warning $(cust_config))
ifeq ($(GBB_CUST_NAME),)
    ifneq ($(filter $(TARGET_BUILD_VARIANT)_$(OBB_PRODUCT_NAME),user_nashville),)
        tmp_software_type := $(TARGET_BUILD_VARIANT)
    else ifeq ($(cust_config),)
        tmp_software_type := eng
    else ifeq ($(cust_config),cust_modem_user)
        ifneq ($(filter $(OBB_PRODUCT_NAME),nashville atherton),)
            tmp_software_type := user
        else
            tmp_software_type := eng
        endif
$(warning "[titan]==tmp_software_type")
$(warning $(tmp_software_type),[titan])
$(warning "[titan]==OBB_PRODUCT_NAME")
$(warning $(OBB_PRODUCT_NAME),[titan])
    else ifeq ($(cust_config),cust_modem_asan)
        tmp_software_type := asan
    else
        $(error cust_config error)
    endif

    GBB_CUST_NAME := $(GBB_CHIP_TYPE)_$(tmp_software_type)
    tmp_software_type :=
endif

export CUST_NAME := $(GBB_CUST_NAME)
export GBB_CUST_NAME
export GBB_CHIP_TYPE
export GBB_PRODUCT_NAME := $(OBB_PRODUCT_NAME)

$(if $(GBB_PRODUCT_NAME),,$(error OBB_PRODUCT_NAME is NULL))
