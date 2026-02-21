
/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: seplat
 * Create : 2024/03/12
 */
#ifndef __SEPLAT_COMMON_H_
#define __SEPLAT_COMMON_H_

#ifndef unused
#define unused(x) (void)(x)
#endif

#define ROOTID              0
#define SEPLAT_CHIP_TYPE    "ca11"
#define TEE_SERVICE_NAME    "seplat"
#define SEPLAT_DEVICE_NAME  "seplat"
#define SEPLAT_DESENSITIVE_NAME "T27290" /* Desensitive name for HiSES */

#define TEEC_PARAM_INDEX_0                 0
#define TEEC_PARAM_INDEX_1                 1
#define TEEC_PARAM_INDEX_2                 2
#define TEEC_PARAM_INDEX_3                 3

#define seplat_chk_err_return_len(ret_len) \
	do {                                   \
		if ((ret_len) < 0) {               \
			tloge("[%s]:%d:%x\n", __func__, __LINE__, (ret_len)); \
			return (ret_len);                                     \
		}                                                         \
	} while (0)

/* uuid to SE TA: 78a354fa-7843-487b-a69e-0da325a828aa */
#define UUID_TEE_SERVICE_SEPLAT                        \
{                                                      \
	0x78a354fa, 0x7843, 0x487b,                        \
	{                                                  \
		0xa6, 0x9e, 0x0d, 0xa3, 0x25, 0xa8, 0x28, 0xaa \
	}                                                  \
}

/* Make sure same with definition in tee. */
enum seplat_cmd_id {
    SEPLAT_TA_CMD_ID_SHUTDOWN_POWER_OFF = 11,
    SEPLAT_TA_CMD_ID_GET_LOG = 12,
    SEPLAT_TA_CMD_ID_TPM_CONNECT = 13,
    SEPLAT_TA_CMD_ID_TPM_DISCONNECT = 14,
    SEPLAT_TA_CMD_ID_TPM_DATA_TRANS = 15,
    SEPLAT_TA_CMD_ID_FLASH_TEST = 16,
    SEPLAT_TA_CMD_ID_GET_DIEID_INFO = 19,
    SEPLAT_TA_CMD_ID_MAX,
};

enum seplat_errno {
    SEPLAT_KERNEL_OK       = 0,
    /* seplat basic errcode, 'S''E'00XX */
    SEPLAT_ERR_BASIC_BASIC = 0x53450000,
    SEPLAT_ERR_BASIC_NULL,
    SEPLAT_ERR_BASIC_PARAMS,
    SEPLAT_ERR_BASIC_INVALID,
    SEPLAT_ERR_BASIC_NOFOUND,
    SEPLAT_ERR_BASIC_MEMORY,
    SEPLAT_ERR_BASIC_VERIFY,
    SEPLAT_ERR_BASIC_READ,
    SEPLAT_ERR_BASIC_WRITE,
    SEPLAT_ERR_BASIC_REQUEST,
    SEPLAT_ERR_BASIC_ALARM,
    SEPLAT_ERR_BASIC_UNSUPPORT,
    SEPLAT_ERR_BASIC_ATTACK,
    SEPLAT_ERR_BASIC_BUSY,
    SEPLAT_ERR_BASIC_EMPTY,

    /* seplat factory errcode, 'S''E'01XX */
    SEPLAT_ERR_FT = 0x53450100,
    SEPLAT_ERR_FT_NORESPONSE,
    SEPLAT_ERR_FT_NOT_READY,
    SEPLAT_ERR_FT_RESULT_DEFAULT,
    SEPLAT_ERR_FT_RESULT_FAIL,
    SEPLAT_ERR_FT_STATUS_WRONG,
    SEPLAT_ERR_FT_STATUS_BUSY,
    SEPLAT_ERR_FT_THREAD_START_FAIL,
    SEPLAT_ERR_FT_THREAD_RUN_FAIL,
    SEPLAT_ERR_FT_FILE_CREATE = 0x53450109,
    SEPLAT_ERR_FT_FILE_OPEN_FAIL = 0x5345010A,
    SEPLAT_ERR_FT_FILE_LEN_INVALID = 0x5345010B,
    SEPLAT_ERR_FT_FILE_WRITE_OUT = 0x5345010C,
    SEPLAT_ERR_FT_FILE_READ_IN = 0x5345010D,
    SEPLAT_ERR_HDI_FILE_OPEN_FAIL = 0x5345010E,
    SEPLAT_ERR_HDI_FILE_LEN_INVALID = 0x5345010F,
    SEPLAT_ERR_FLASH_1BIT_ECC = 0x53450110,
    SEPLAT_ERR_FLASH_XBIT_ECC = 0x53450111,
    SEPLAT_ERR_DSM_REPORT_ERR = 0x53450112,
    SEPLAT_ERR_DSM_CLIENT_BUSY = 0x53450113,
    SEPLAT_ERR_DSM_UNSUPPORT = 0x53450114,
    SEPLAT_ERR_FT_DIEID_LEN_INVALID = 0x53450115,

    /* seplat cmd errcode, 'S''E'02XX */
    SEPLAT_ERR_CMD = 0x53450200,
    SEPLAT_ERR_CMD_PARAM_INVALID,
    SEPLAT_ERR_CMD_DATA_INVALID,
    SEPLAT_ERR_CMD_PARAM_TYPE_INVALID,
    SEPLAT_ERR_CMD_PARAM_DATA_INVALID,
    SEPLAT_ERR_CMD_TIMEOUT,
    SEPLAT_ERR_CMD_INVOKE_FAIL,
    SEPLAT_ERR_CMD_INIT_CONTEXT_FAIL,
    SEPLAT_ERR_CMD_OPENSESSION_FAIL,
};

#endif /* __SEPLAT_COMMON_H_ */
