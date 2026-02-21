/**
 * @defgroup    bsl_errno   bsl_errno.h
 * @ingroup     bsl
 * @copyright   Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * @brief       BSL module function return value
 */

#ifndef BSL_ERRNO_H
#define BSL_ERRNO_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup bsl_errno
 * @brief   Return success
 */
#define BSL_SUCCESS 0

/**
 * @ingroup bsl_errno
 *
 * Return values of the BSL module range from 0x03000000 to 0x03ffffff.
 */
enum BSL_ERROR {
    /* Common return value start from 0x03000000. */
    BSL_NULL_INPUT = 0x03000000,
    BSL_INTERNAL_EXCEPTION,
    BSL_MALLOC_FAIL,
    BSL_MEMCPY_FAIL,
    BSL_MEMMOVE_FAIL,
    BSL_ERROR_INPUT,
    BSL_INVALID_ARG,

    /* The return value of the SAL submodule starts from 0x03010001. */
    BSL_SAL_ERR_UNKNOWN = 0x03010001,        /**< Unknown error. */
    BSL_SAL_ERR_NO_MEMORY,                   /**< Failed to apply for memory. */
    BSL_SAL_ERR_BAD_PARAM,                   /**< Parameter incorrect. */
    BSL_SAL_ERR_FILE_OPEN,                   /**< Open file error. */
    BSL_SAL_ERR_FILE_READ,                   /**< File reading error. */
    BSL_SAL_ERR_FILE_WRITE,                  /**< File writing error. */
    BSL_SAL_ERR_FILE_LENGTH,                 /**< Obtaining the file length error. */
    BSL_SAL_ERR_FILE_TELL,                   /**< Error in obtaining the file pointer offset. */
    BSL_SAL_ERR_FILE_SEEK,                   /**< Failed to set pointer position of file. */
    BSL_SAL_ERR_FILE_SET_ATTR,               /**< Setting file attribute is incorrect. */
    BSL_SAL_ERR_FILE_GET_ATTR,               /**< Error in obtaining file attributes. */
    BSL_SAL_NOT_FILE_EOF,
    BSL_SAL_MEM_NO_REG_FUNC,
    BSL_SAL_THREAD_LOCK_NO_REG_FUNC,
    BSL_SAL_FILE_NO_REG_FUNC,

    /* The return value of the LOG submodule starts from 0x03020001. */
    BSL_LOG_ERR_REG_FUNC_FAIL = 0x03020001,  /**< Failed to register the log output function. */
    BSL_LOG_ERR_BAD_PARAM,                   /**< Parameter error. */
    BSL_LOG_ERR_MEMCPY_FAIL,                 /**< Memory Copy Failure. */

    /* The return value of the TLV submodule starts from 0x03030001. */
    BSL_TLV_ERR_BAD_PARAM = 0x03030001,      /**< Incorrect parameter. */
    BSL_TLV_ERR_MEMCPY_FAIL,                 /**< Memory Copy Failure. */
    BSL_TLV_ERR_NO_WANT_TYPE,                /**< No TLV found. */

    /* The return value of the ERR submodule starts from 0x03040001. */
    BSL_ERR_ERR_ACQUIRE_READ_LOCK_FAIL = 0x03040001,  /**< Failed to obtain the read lock. */
    BSL_ERR_ERR_ACQUIRE_WRITE_LOCK_FAIL,
    BSL_ERR_ERR_NO_STACK,
    BSL_ERR_ERR_NO_ERROR,
    BSL_ERR_ERR_NO_MARK,

    /* The return value of the TIME submodule starts from 0x03050001. */
    BSL_SAL_TIME_BAD_PARAM = 0x03050001,
    BSL_SAL_TIME_NO_REG_FUNC,                /** The time-related function is not registered. */

    /* The return value of the UIO submodule starts from 0x03060001. */
    BSL_UIO_FAIL = 0x03060001,
    BSL_UIO_CB_NOT_SET,
    BSL_UIO_IO_EXCEPTION,
    BSL_UIO_IO_BUSY,
    BSL_UIO_CTRL_INVALID_PARAM,
    BSL_UIO_FILE_OPEN_FAIL,
    BSL_UIO_MEM_GROW_FAIL,
    BSL_UIO_REF_MAX,
    BSL_UIO_MEM_ALLOC_FAIL,
    BSL_UIO_IO_EOF,
    BSL_UIO_WRITE_NOT_ALLOWED,
    BSL_UIO_UNINITIALIZED,
    BSL_UIO_MEM_NOT_NULL,
    BSL_UIO_EXIST_CONTEXT_NOT_RELEASED,
    BSL_UIO_BUF_TOO_LONG,
    BSL_UIO_TTO_EWOULDBLOCK,

    /* The return value of the LIST submodule starts from 0x03070001. */
    BSL_LIST_INVALID_LIST_CURRENT = 0x03070001,
    BSL_LIST_MALLOC_FAIL,
    BSL_LIST_DATA_NOT_AVAILABLE,
    BSL_LIST_FULL,

    /* The return value of the UI submodule starts from 0x03080001. */
    BSL_UI_WRITE_ERROR = 0x03080001,
    BSL_UI_FGETS_ERROR,
    BSL_UI_STDIN_END_ERROR,
    BSL_UI_OPERATION_ERROR,
    BSL_UI_READ_LEN_TOO_SHORT,
    BSL_UI_READ_BUFF_TOO_LONG,
    BSL_UI_METHOD_INVALID_TYPE,
    BSL_UI_CONSTRUCT_PROMPT_ERROR,
    BSL_UI_CREATE_OBJECT_ERROR,
    BSL_UI_OUTPUT_BUFF_TOO_SHORT,
    BSL_UI_INVALID_DATA_TYPE,
    BSL_UI_INVALID_DATA_ARG,
    BSL_UI_INVALID_DATA_RESULT,
    BSL_UI_VERIFY_BUFF_FAILED,
    BSL_UI_MEM_ALLOC_FAIL,

    /* The return value of the OID submodule starts from 0x03090001. */
    BSL_OID_INVALID = 0x03090001,
    BSL_OID_TYPE_CODE_INVALID,
    BSL_OID_NAME_LENGTH_INVALID,
    BSL_OID_DUPLICATE,
    BSL_OID_OBJ_INTERNAL_USE,

    /* The return value of the BASE64 submodule starts from 0x030a0001. */
    BSL_BASE64_INVALID = 0x030a0001,
    BSL_BASE64_BUF_NOT_ENOUGH,
    BSL_BASE64_DATA_NOT_ENOUGH,
    BSL_BASE64_WRITE_FAILED,
    BSL_BASE64_READ_FAILED,
    BSL_BASE64_DATA_AFTER_PADDING,
    BSL_BASE64_ILLEGALLY_MODIFIED,
    BSL_BASE64_ENCODE_FAILED,
    BSL_BASE64_DECODE_FAILED,
    BSL_BASE64_HEADER,
    BSL_BASE64_INVALID_CHARACTER,
    BSL_BASE64_INVALID_ENCODE,

    /* The return value of the NET submodule starts from 0x030b0001. */
    BSL_SAL_ERR_NET_NOBLOCK = 0x030b0001,
    BSL_SAL_ERR_NET_SOCKCLOSE,
    BSL_SAL_ERR_NET_SETSOCKOPT,
    BSL_SAL_ERR_NET_GETSOCKOPT,
    BSL_SAL_ERR_NET_LISTEN,
    BSL_SAL_ERR_NET_BIND,
    BSL_SAL_ERR_NET_CONNECT,
    BSL_SAL_ERR_NET_IOCTL,
    BSL_SAL_ERR_NET_ILLEGALFD,
    BSL_SAL_NET_NO_REG_FUNC,

    /* The return value of the CONF submodule starts from 0x03110001. */
    BSL_CONF_FAIL = 0x030c0001,
    BSL_CONF_INIT_FAIL,
    BSL_CONF_LOAD_FAIL,
    BSL_CONF_MEM_ALLOC_FAIL,
    BSL_CONF_FREE_FAIL,
    BSL_CONF_NOT_NUM,
    BSL_CONF_PARSE_FAIL,
    BSL_CONF_COPY_ARGS_FAILED,
    BSL_CONF_CONTEXT_ERR,
    BSL_CONF_GET_FAIL,

    /* The return value of the DB submodule starts from 0x03120001. */
    BSL_DB_INSERT_FAIL = 0x030d0001,
    BSL_DB_CREATEINDEX_FAIL,
    BSL_DB_STORE_FAIL,
    BSL_DB_INSERT_DUPLICATE_DATA,
    BSL_DB_HASHLIST_REPEAT,
    BSL_DB_CREATEHASH_FAIL,
    BSL_DB_LOAD_FAIL,
    BSL_DB_MALLOC_FAIL,
    BSL_DB_ERR_INPUT,

    /* The return value of the ASYNC submodule starts from 0x03200001. */
    BSL_ASYNC_ERR = 0x030e0001,
    BSL_ASYNC_NO_JOBS,
    BSL_ASYNC_PAUSE,
    BSL_ASYNC_FINISH,
    BSL_ASYNC_FREE_FAIL,
    BSL_ASYNC_SCHEDULE_NOT_INIT,
    BSL_ASYNC_JOB_NOT_EXIST,
    BSL_ASYNC_UCONTEXT_NOT_WORK,
    BSL_ASYNC_UCONTEXT_SWAP_FAIL,
    BSL_ASYNC_SET_GUARD_PAGE_FAIL,
    BSL_ASYNC_INIT_BY_SSL_FAIL,
    BSL_ASYNC_NO_REG_FUNC,
};

#ifdef __cplusplus
}
#endif

#endif
