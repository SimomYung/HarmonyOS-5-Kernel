/**
 * @defgroup    bsl_ui    bsl_ui.h
 * @ingroup     bsl
 * @copyright   Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * @brief       UI module header file
 */
#ifndef BSL_UI_H
#define BSL_UI_H

#include <stdint.h>
#include <stdbool.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    BSL_UIM_NONE = 0,
    BSL_UIM_OPEN,
    BSL_UIM_WRITE,
    BSL_UIM_READ,
    BSL_UIM_CLOSE
} BSL_UI_MethodTypes;

typedef enum {
    BSL_UI_DATA_NONE = 0,
    BSL_UI_DATA_READ,
    BSL_UI_DATA_WRITE
} BSL_UI_DataTypes;

typedef enum {
    BSL_UI_DATA_FLAG_NONE = 0, // Setting the Echo Display 01, follow with 010 0100
    BSL_UI_DATA_FLAG_ECHO = 0x1,
    BSL_UI_DATA_FLAG_USER = 0x10000
} BSL_UI_DataFlags;

typedef struct {
    uint32_t flags;
    char *buff;
    uint32_t buffLen;
    char *verifyBuff;
} BSL_UI_CtrlRGetParam;

typedef struct {
    const char *desc;
    const char *name;
    bool verify;
} BSL_UI_ReadPwdParam;

typedef struct UI_Control BSL_UI;
typedef struct UI_ControlMethod BSL_UI_Method;
typedef struct UI_ControlDataPack BSL_UI_DataPack;

typedef int32_t (*BSL_UI_Open) (BSL_UI *ui);
typedef int32_t (*BSL_UI_Write) (BSL_UI *ui, BSL_UI_DataPack *data);
typedef int32_t (*BSL_UI_Read) (BSL_UI *ui, BSL_UI_DataPack *data);
typedef int32_t (*BSL_UI_Close) (BSL_UI *ui);
typedef int32_t (*BSL_UI_CheckDataCallBack) (BSL_UI *ui, char *buff, uint32_t buffLen, void *callBackData);
/**
  * @ingroup bsl_ui
  * @brief The method function is NULL. UI of common default processing functions.
  *        Otherwise, use user-defined functions to create UIs.
  *
  * @param method [IN] UI function
  *
  * @return If success, BSL_SUCCESS is returned.
  *         If fails, NULL is returned.
  */
BSL_UI *BSL_UI_New(const BSL_UI_Method *method);

/**
  * @ingroup bsl_ui
  * @brief Release the UI
  *
  * @param ui [IN] UI object
  *
  * @return   None
  */
void BSL_UI_Free(BSL_UI *ui);

/**
  * @ingroup bsl_ui
  * @brief Create a BSL_UI_Method
  *
  * @return  If success, the BSL_UI_Method object is returned;
  *          If fails, NULL is returned.
  */
BSL_UI_Method *BSL_UI_MethodNew(void);

/**
  * @ingroup bsl_ui
  * @brief Release the BSL_UI_Method
  *
  * @param method [IN] BSL_UI_Method Object
  *
  * @return  None
  */
void BSL_UI_MethodFree(BSL_UI_Method *method);

/**
  * @ingroup bsl_ui
  * @brief If the ui parameter is NULL, obtain the default UI processing function.
  *        Otherwise, obtain the UI processing function.
  *
  * @param ui [IN] UI object
  *
  * @return  If success, the BSL_UI_Method object is returned;
  *          If fails, NULL is returned.
  */
const BSL_UI_Method *BSL_UI_GetOperMethod(const BSL_UI *ui);

/**
  * @ingroup bsl_ui
  * @brief Set the BSL UI method.
  *
  * @param method [IN] BSL_UI_Method Object
  * @param type [IN] Method type. For details, see BSL_UI_MethodTypes.
  * @param func [IN] Method to be set. For details, see the callback prototype.
  *
  * @return If success, the BSL_SUCCESS is returned.
  *         If fail, other values are returned.
  */
int32_t BSL_UI_SetMethod(BSL_UI_Method *method, uint8_t type, void *func);

/**
  * @ingroup bsl_ui
  * @brief Obtain the BSL UI method.
  *
  * @param method [IN] BSL_UI_Method Object
  * @param type [IN] Method type. For details, see BSL_UI_MethodTypes.
  * @param func [OUT] Pointer to the obtained method function.
  *
  * @return If success, the BSL_SUCCESS is returned.
  *         If fail, other values are returned.
  */
int32_t BSL_UI_GetMethod(const BSL_UI_Method *method, uint8_t type, void **func);

/**
  * @ingroup bsl_ui
  * @brief Read the pwd tool function of the user input
  *
  * @param param [IN] The user need input parameter marked as "BSL_UI_ReadPwdParam" that required by pwd tool function.
                      desc: indicates the string description, name: indicates the string object name, and verify
                      indicates whether the verification is required.
  * @param buff [OUT] Indicates the obtained PWD buff.
  * @param buffLen [IN/OUT] Indicates the obtained PWD buff length.
  * @param checkDataCallBack [IN] BSL_UI_CheckDataCallBack checks the input string callback, the value NULL
  * indicates that the check is not required.
  * @param callBackData [IN] BSL_UI_CheckDataCallBack, check the user data of the input string callback.
  *
  * @return  If success, the BSL_SUCCESS is returned.
  *          If fail, other values are returned.
  */
int32_t BSL_UI_ReadPwdUtil(BSL_UI_ReadPwdParam *param, char *buff, uint32_t *buffLen,
    const BSL_UI_CheckDataCallBack checkDataCallBack, void *callBackData);

/**
  * @ingroup bsl_ui
  * @brief Create a DataPack object
  *
  * @return  If success, the BSL_UI_DataPack object is returned.
  *          If fail, other values are returned.
  */
BSL_UI_DataPack *BSL_UI_DataPackNew(void);

/**
  * @ingroup bsl_ui
  * @brief Release the BSL_UI_DataPack
  *
  * @param data [IN] BSL_UI_DataPack Object
  *
  * @return  None
  */
void BSL_UI_DataPackFree(BSL_UI_DataPack *data);

/**
  * @ingroup bsl_ui
  * @brief Set the BSL_UI_DataPack data by type
  *
  * @param data [IN] BSL_UI_DataPack Object
  * @param type [IN] Different types perform different operations on the data object
  * @param parg [IN] Depends on the type. For details, see the following sections:
  * @param larg [IN] Depends on the type. For details, see the following sections:
  *
  * @brief Set the BSL_UI_DataPack data of the read type
  * @param data [IN] BSL_UI_DataPack Object
  * @param type [IN] BSL_UI_DATA_READ
  * @param parg [IN] BSL_UI_CtrlRGetParam *, include：
  *             flags   [IN] BSL_UI_DATA_FLAG_ECHO/BSL_UI_DATA_FLAG_USER and user define
  *             data    [IN] Indicates the buff to be read
  *             dataLen [IN] Data length
  * @param larg [IN] BSL_UI_CtrlRGetParam Length
  *
  * @brief Set the BSL_UI_DataPack data of the write type
  * @param data [IN] BSL_UI_DataPack Object
  * @param type [IN] BSL_UI_DATA_WRITE
  * @param parg [IN] Buffer to be written
  * @param larg [IN] Length of the buffer to be written
  *
  * @return  If success, BSL_SUCCESS is returned.
  *          Else, other values are returned.
  */
int32_t BSL_UI_DataCtrl(BSL_UI_DataPack *data, uint32_t type, void *parg, uint32_t larg);

/**
  * @ingroup bsl_ui
  * @brief Obtaining user data result
  *
  * @param data [IN] Data object to be obtained.
  * @param result [OUT] User data result.
  * @param result_len [OUT] User data result length.
  *
  * @return  If success, BSL_SUCCESS is returned.
  *          Else, other values are returned.
  */
int32_t BSL_UI_GetDataResult(BSL_UI_DataPack *data, char **result, uint32_t *resultLen);

/**
  * @ingroup bsl_ui
  * @brief Construct a prompt message
  *
  * @param objectDesc [IN] Object description in the prompt message.
  * @param objectName [IN] Object name in the prompt message.
  *
  * @return  If success, constructed prompt string is returned.
  *          Else, NULL is returned.
  */
char *BSL_UI_ConstructPrompt(const char *objectDesc, const char *objectName);

#ifdef __cplusplus
}
#endif

#endif