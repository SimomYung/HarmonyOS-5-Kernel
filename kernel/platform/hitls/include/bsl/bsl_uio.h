/**
 * @defgroup  bsl bsl_uio.h
 * @ingroup   bsl
 * @copyright Copyright (c) Huawei Technologies Co., Ltd. 2022-2023. All rights reserved.
 * @brief     UIO external interfaces
 */

#ifndef BSL_UIO_H
#define BSL_UIO_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup bsl
 *
 * @brief   UIO module control structure
 */
typedef struct UIO_ControlBlock BSL_UIO;

typedef int32_t BSL_UIO_InfoCb(BSL_UIO *, int32_t, int32_t);

/* BSL_UIO_Method方法提供 */
typedef int32_t (*BslUioWriteCb)(BSL_UIO *uio, const void *buf, uint32_t len, uint32_t *writeLen); /**< UIO写函数 */
typedef int32_t (*BslUioReadCb)(BSL_UIO *uio, void *buf, uint32_t len, uint32_t *readLen);         /**< UIO读函数 */
typedef int32_t (*BslUioCtrlCb)(BSL_UIO *uio, int32_t cmd, int32_t larg, void *parg);              /**< UIO控制函数 */
typedef int32_t (*BslUioCreateCb)(BSL_UIO *uio);                                                   /**< UIO释放函数 */
typedef int32_t (*BslUioDestroyCb)(BSL_UIO *uio);                                                  /**< UIO创建函数 */
typedef int32_t (*BslUioPutsCb)(BSL_UIO *uio, const char *buf, uint32_t *writeLen);
typedef int32_t (*BslUioGetsCb)(BSL_UIO *uio, char *buf, uint32_t *readLen);
typedef int32_t (*BslUioCbCtrlCb)(BSL_UIO *uio, int cmd, BSL_UIO_InfoCb *callback);

typedef struct BSL_UIO_MethodStruct BSL_UIO_Method;

/**
 * @ingroup bsl
 *
 * @brief   UIO_Addr structure
 */
typedef union UIO_Address BSL_UIO_Addr;

/**
 * @ingroup bsl
 *
 * @brief   Transmission protocol enumeration
 */
typedef enum {
    BSL_UIO_TCP,
    BSL_UIO_UDP,
    BSL_UIO_SCTP,
    BSL_UIO_MEM,
    BSL_UIO_FILE,
    BSL_UIO_FD,
    BSL_UIO_PAIR,
    BSL_UIO_TTO,    /**< TTO IO */
    BSL_UIO_BUFFER,
    BSL_UIO_BASE64,
    BSL_UIO_NULL,
    BSL_UIO_CONNECT,
    BSL_UIO_SSL,
    BSL_UIO_UNKNOWN, /**< Unknown protocol should not appear */

    BSL_UIO_EXTEND = 10000, /** extension value */
} BSL_UIO_TransportType;

/**
 * @ingroup bsl
 *
 * @brief   Sctp auth key, hitls Use the BSL_UIO_Method.ctrl method to transfer the BSL_UIO_SCTP_ADD_AUTH_SHARED_KEY
 *          instruction to notify the user that the auth key needs to be set.
 */
typedef struct {
    uint16_t shareKeyId;
    uint16_t authKeySize;
    const uint8_t *authKey;
} BSL_UIO_SctpAuthKey;

/**
 * @ingroup bsl
 *
 * @brief   userData release function
 */
typedef void (*BSL_UIO_USERDATA_FREE_FUNC)(void *);

/**
 * @ingroup bsl
 *
 * @brief   BSL_UIO_CtrlParameter controls the I/O callback function. Hitls notifies the
 *          user of the function to be implemented
 */
typedef enum {
    /* Public use 0x0xx */
    /* larg equals the length of array, parg is a pointer type array */
    BSL_UIO_SET_PEER_IP_ADDR = 0x000,
    /* larg equals the length of BSL_UIO_CtrlGetPeerIpAddrParam structure,
     * parg is a pointer type BSL_UIO_CtrlGetPeerIpAddrParam
     */
    BSL_UIO_GET_PEER_IP_ADDR,
    /* larg equals the length of 32-bit integer, parg is a pointer type 32-bit integer */
    BSL_UIO_SET_FD,
    /* larg equals the length of 32-bit integer, parg is a pointer type 32-bit integer */
    BSL_UIO_GET_FD,
    /* larg equals the length of 64-bit integer, parg is a pointer type 64-bit integer */
    BSL_UIO_PENDING,
    /* not need to set larg and parg. the default values are 0 and null */
    BSL_UIO_FLUSH,
    /* larg equals the length of 64-bit integer, parg is a pointer type 64-bit integer */
    BSL_UIO_WPENDING,
    /* larg equals the length of 64-bit integer,  parg is a pointer type 64-bit integer */
    BSL_UIO_SEEK,
    /* not need to set larg and parg. the default values are 0 and null */
    BSL_UIO_RESET,
    /* larg equals the length of bool,  parg is a pointer type bool */
    BSL_UIO_GET_INIT,
    /* larg equals the length of 64-bit integer, parg is a pointer type 64-bit integer */
    BSL_UIO_GET_WRITE_NUM,
    /* larg equals the length of 64-bit integer, parg is a pointer type 64-bit integer */
    BSL_UIO_GET_READ_NUM,
    /* larg equals the length of 32-bit integer, parg is a pointer type 32-bit integer */
    BSL_UIO_GET_PTO_LEN,
    BSL_UIO_INFO,
    BIO_UIO_SET_CALLBACK,
    BSL_UIO_SET_CONNECT,
    BSL_UIO_GET_CONNECT,
    BSL_UIO_SET_NOBLOCK,
    BSL_UIO_DO_HANDSHAKE,
    BSL_UIO_SET_CTX,
    BSL_UIO_GET_CTX,
    BSL_UIO_SET_SSL_RENEGOTIATE_TIMEOUT,
    BSL_UIO_SET_SSL_RENEGOTIATE_BYTES,
    BSL_UIO_UDP_SET_CONNECTED,
    BSL_UIO_SET_BUFFER_SIZE,

    /* SCTP uses 0x3XX */
    BSL_UIO_SCTP_CHECK_PEER_AUTH = 0x300,     /**< SCTP Check whether the peer supports authentication */
    BSL_UIO_SCTP_ADD_AUTH_SHARED_KEY,         /**< Add a new sctp auth key. The sctp can use the new
                                                   key to receive data */
    BSL_UIO_SCTP_ACTIVE_AUTH_SHARED_KEY,      /**< activates the new sctp auth key. The sctp needs to
                                                   use the new key to send data */
    BSL_UIO_SCTP_DEL_PRE_AUTH_SHARED_KEY,     /**< SCTP Delete the previous shared key */
    BSL_UIO_SCTP_SND_BUFF_IS_EMPTY,           /**< Whether the SCTP send queue is NULL. ctrl.param is the
                                                   pointer of uint8_t *. If the value of ctrl.param is greater than 0,
                                                   it indicates that the queue is NULL.
                                                   Otherwise, it indicates that the queue is not NULL */
    BSL_UIO_SCTP_RECV_BUFF_IS_EMPTY,          /**< check whether the SCTP receive queue is NULL.
                                                   ctrl.param is the pointer of uint8_t *.
                                                   If the value of ctrl.param is greater than 0,
                                                   it indicates that the queue is NULL.
                                                   Otherwise, it indicates that the queue is not NULL */
    BSL_UIO_SCTP_GET_SEND_STREAM_ID,
    /* larg equals the length of unsigned 16-bit integer, parg is a pointer type unsigned 16-bit integer */
    BSL_UIO_SCTP_SET_APP_STREAM_ID,
    /* larg equals the length of bool, parg is a pointer type bool */
    BSL_UIO_SCTP_MASK_APP_MESSAGE,
    /* larg is a func callback type BSL_UIO_METHOD_TYPE,  parg is a pointer type func callback */
    BSL_UIO_SCTP_SET_CALLBACK,

    /* MEM使用0x4XX */
    /* larg equals the length of array, parg is a pointer type array */
    BSL_UIO_MEM_NEW_BUF = 0x400,
    /* larg equals the length of pointer, parg is a pointer's pointer type BSL_BufMem */
    BSL_UIO_MEM_GET_PTR,
    /* larg equals the length of 32-bit integer, parg is a pointer type 32-bit integer */
    BSL_UIO_MEM_SET_EOF,
    /* larg equals the length of 32-bit integer, parg is a pointer type 32-bit integer */
    BSL_UIO_MEM_GET_EOF,
    /* larg equals the length of BSL_UIO_CtrlGetInfoParam,  parg is a pointer type BSL_UIO_CtrlGetInfoParam */
    BSL_UIO_MEM_GET_INFO,

    /* FILE使用0x5xx */
    /* larg is the mode for opening a file, parg is a pointer type string */
    BSL_UIO_FILE_OPEN = 0x500,
    /* larg is of the Boolean type and is used to indicate whether the handle is closed with the release of uio,
     * parg is a pointer type file */
    BSL_UIO_FILE_PTR,
    BSL_UIO_FILE_GET_EOF,

    /* FD使用0x6xx */
    /* larg equals the length of 64-bit integer, parg is a pointer type 64-bit integer */

    BSL_UIO_FD_GET_INFO = 0x600,

    /* Pair uses 0x7xx */
    BSL_UIO_PAIR_MAKE = 0x700,
    BSL_UIO_PAIR_SET_BUFFER_SIZE,
    BSL_UIO_PAIR_GET_BUFFER_SIZE,

    /* TTO uses 0x8xx */
    BSL_UIO_TTO_SET_READ_CB = 0x800,
    BSL_UIO_TTO_SET_WRITE_CB,
    BSL_UIO_TTO_SET_READ_APP_DATA,
    BSL_UIO_TTO_SET_WRITE_APP_DATA,

    /* DTO uses 0x10xx */
    BSL_UIO_DTO_SET_KEY = 0x1001,
    BSL_UIO_DTO_SET_EPOCH_SEQUENCE,
} BSL_UIO_CtrlParameter;

#define BSL_UIO_FLAGS_BASE64_NO_NEWLINE  0x20
#define BSL_UIO_FLAGS_BASE64_PEM         0x40

typedef enum {
    BSL_UIO_CREATE_CB,
    BSL_UIO_DESTROY_CB,
    BSL_UIO_WRITE_CB,
    BSL_UIO_READ_CB,
    BSL_UIO_CTRL_CB,
    BSL_UIO_PUTS_CB,
    BSL_UIO_GETS_CB,
} BSL_UIO_METHOD_TYPE;

typedef struct {
    uint8_t *addr;
    uint32_t size;
} BSL_UIO_CtrlGetPeerIpAddrParam;

#define BSL_UIO_FILE_READ             0x02
#define BSL_UIO_FILE_WRITE            0x04
#define BSL_UIO_FILE_APPEND           0x08
#define BSL_UIO_FILE_TEXT             0x10

#define BSL_UIO_FLAGS_READ          0x01
#define BSL_UIO_FLAGS_WRITE         0x02
#define BSL_UIO_FLAGS_IO_SPECIAL    0x04
#define BSL_UIO_FLAGS_RWS (BSL_UIO_FLAGS_READ | BSL_UIO_FLAGS_WRITE | BSL_UIO_FLAGS_IO_SPECIAL)
#define BSL_UIO_FLAGS_SHOULD_RETRY  0x08

#define BSL_UIO_FLAGS_MEM_READ_ONLY      0x10 // This flag can be set only by uio_mem


typedef int32_t (*BSL_TTO_WriteCb)(int32_t fd, const uint8_t *buf, int32_t bufLen, void *appData, int32_t *err);

typedef int32_t (*BSL_TTO_ReadCb)(int32_t fd, uint8_t *buf, int32_t bufLen, void *appData, int32_t *err);


/**
 * @ingroup bsl_uio
 *
 * @brief   创建自定义method结构
 * @return  MEM UIO方法指针
 */
BSL_UIO_Method *BSL_UIO_NewMethod(void);

/**
 * @ingroup bsl_uio
 *
 * @brief   设置自定义method类型
 * @param   meth  [IN] Method结构体
 * @param   type  [IN] 类型
 *
 * @retval BSL_SUCCESS,设置成功
 * @retval BSL_NULL_INPUT,空参
 */
int32_t BSL_UIO_SetMethodType(BSL_UIO_Method *meth, BSL_UIO_TransportType type);

/**
 * @ingroup bsl_uio
 *
 * @brief   设置自定义method回调
 * @param   meth  [IN] Method结构体
 * @param   type  [IN] 回调类型
 * @param   func  [IN] 回调
 *
 * @retval BSL_SUCCESS,设置成功
 * @retval BSL_NULL_INPUT,空参
 * @retval BSL_INVALID_ARG,不合法入参
 */
int32_t BSL_UIO_SetMethod(BSL_UIO_Method *meth, int32_t type, void *func);

/**
 * @ingroup bsl_uio
 *
 * @brief   释放自定义 Method方法
 * @param   meth  [IN] Method方法结构体
 * @return  void
 */
void BSL_UIO_FreeMethod(BSL_UIO_Method *meth);

/**
 * @ingroup bsl
 *
 * @brief   obtain the default MEM UIO method.
 * @return  pointer to the MEM UIO method
 */
const BSL_UIO_Method *BSL_UIO_MemMethod(void);

/**
 * @ingroup bsl
 *
 * @brief   obtain the default FILE UIO method
 * @return  pointer to the FILE UIO method
 */
const BSL_UIO_Method *BSL_UIO_FileMethod(void);

/**
 * @ingroup bsl
 *
 * @brief   obtain the default FD UIO method.
 * @return  pointer to the FD UIO method
 */
const BSL_UIO_Method *BSL_UIO_FDMethod(void);

/**
 * @ingroup bsl
 *
 * @brief   obtain the default SCTP UIO
 * @return  pointer to the SCTP UIO method
 */
const BSL_UIO_Method *BSL_UIO_SctpMethod(void);

/**
 * @ingroup bsl
 *
 * @brief   obtain the default TCP UIO method
 * @return  pointer to the TCP UIO method
 */
const BSL_UIO_Method *BSL_UIO_TcpMethod(void);

/**
 * @ingroup bsl
 *
 * @brief   obtain the default UDP UIO method
 * @return  pointer to the UDP UIO method
 */
const BSL_UIO_Method *BSL_UIO_UdpMethod(void);


/**
 * @ingroup bsl
 *
 * @brief   Obtain the default PAIR UIO method.
 * @return  Pointer to the PAIR UIO method
 */
const BSL_UIO_Method *BSL_UIO_PairMethod(void);

/**
 * @ingroup bsl
 *
 * @brief   Obtain the default TTO UIO method
 * @return  Pointer to the TTO UIO method
 */
const BSL_UIO_Method *BSL_UIO_TtoMethod(void);

/**
 * @ingroup bsl
 *
 * @brief   obtain the default buffer UIO
 * @return  pointer to the Buffer UIO method
 */
const BSL_UIO_Method *BSL_UIO_BufferMethod(void);

/**
 * @ingroup bsl
 *
 * @brief   obtain the default BASE64 UIO method.
 * @return  pointer to the BASE64 UIO method
 */
const BSL_UIO_Method *BSL_UIO_Base64Method(void);

/**
 * @ingroup bsl
 *
 * @brief   obtain the default NULL FD method
 * @return  NULL UIO method pointer
 */
const BSL_UIO_Method *BSL_UIO_NULLMethod(void);

/**
 * @ingroup bsl
 *
 * @brief   Obtain the default CONNECT UIO method.
 * @return  CONNECT UIO method pointer
 */
const BSL_UIO_Method *BSL_UIO_ConnMethod(void);

/**
 * @ingroup bsl
 *
 * @brief   Create a UIO object
 * @param   method  [IN] UIO method structure
 * @retval  UIO, created successfully
 * @retval  NULL UIO, creation failure
 */
BSL_UIO *BSL_UIO_New(const BSL_UIO_Method *method);

/**
 * @ingroup bsl
 *
 * @brief   Release the UIO object.
 * @param   uio  [IN] UIO object.
 */
void BSL_UIO_Free(BSL_UIO *uio);

/**
 * @ingroup bsl
 * @brief Write data to the UIO object
 *
 * @param uio  [IN] uio object.
 * @param data  [IN] Data to be written.
 * @param len  [IN] Data length.
 * @param writeLen [OUT] Length of the data that is successfully written.
 *
 * @retval BSL_SUCCESS,indicating that the data is successfully written
 * @retval BSL_INTERNAL_EXCEPTION, an unexpected internal error occurs.
 * @retval BSL_UIO_IO_BUSY,indicating that the underlying I/O is busy.
 * @retval BSL_UIO_IO_EXCEPTION,The I/O is abnormal.
 * @retval BSL_UIO_FAIL,invalid parameter.
 */
int32_t BSL_UIO_Write(BSL_UIO *uio, const void *data, uint32_t len, uint32_t *writeLen);

/**
 * @ingroup bsl
 * @brief   Read data from the UIO object.
 *
 * @param uio  [IN] uio object.
 * @param data  [IN] Buffer for receiving data
 * @param len  [IN] Length of the received data buffer.
 * @param readLen [OUT] Length of the received data.
 *
 * @retval BSL_SUCCESS,The data is read successfully(Determined based on the actual receive length,
 * if the length is 0 means no data is read.)
 * @retval BSL_INTERNAL_EXCEPTION, an unexpected internal error occurs.
 * @retval BSL_UIO_FAIL, invalid parameter.
 * @retval BSL_UIO_IO_EXCEPTION,IO is abnormal.
 */
int32_t BSL_UIO_Read(BSL_UIO *uio, void *data, uint32_t len, uint32_t *readLen);

/**
 * @ingroup bsl
 *
 * @brief Process specific UIO implementations by cmd
 *
 * @param uio [IN] UIO object
 * @param cmd [IN] Different cmd processes perform different operations on UIO objects.
 * @param larg [IN] Determined by cmd. For details, see the following
 * @param parg [IN/OUT] Determined by cmd. For details, see the following
 * @retval BSL_SUCCESS
 * @retval Non-BSL_SUCCESS, for details, see bsl_errno.h.

 */
int32_t BSL_UIO_Ctrl(BSL_UIO *uio, int32_t cmd, int32_t larg, void *parg);

/**
 * @ingroup bsl
 * @brief   Write a string to the UIO object.
 *
 * @param uio  [IN] uio object.
 * @param buf  [IN] A null-terminated string to be written.
 * @param writeLen [OUT] Length of the data that is successfully written.
 *
 * @retval BSL_SUCCESS, Writing succeeded.
 * @retval BSL_INTERNAL_EXCEPTION, an unexpected internal error occurs.
 * @retval BSL_UIO_IO_BUSY, indicating that the underlying I/O is busy.
 * @retval BSL_UIO_IO_EXCEPTION,IO abnormal.
 * @retval BSL_UIO_FAIL, invalid parameter.
 */
int32_t BSL_UIO_Puts(BSL_UIO *uio, const char *buf, uint32_t *writeLen);

/**
 * @ingroup bsl
 * @brief   Reads a string from the UIO object
 *
 * @param uio  [IN] uio object.
 * @param buf  [IN] Buffer that accepts a line of strings
 * @param readLen [IN/OUT] Length of the buffer for receiving data/Length of the data that is successfully read
 * @retval BSL_SUCCESS (Determine the value based on the actual receive length.
 * if the length is 0 means no data is read.)
 * @retval BSL_INTERNAL_EXCEPTION, an unexpected internal error occurs.
 * @retval BSL_UIO_FAIL, invalid parameter.
 * @retval BSL_UIO_IO_EXCEPTION,IO abnormal.
 */
int32_t BSL_UIO_Gets(BSL_UIO *uio, char *buf, uint32_t *readLen);

/**
 * @ingroup bsl
 * @brief Set the UIO init.
 *
 * @param uio [IN] UIO object
 * @param init [IN] init value
 */
void BSL_UIO_SetInit(BSL_UIO *uio, bool init);

/**
 * @ingroup bsl
 *
 * @brief   Obtain the UIO transmission protocol type
 * @param   uio  [IN] UIO object.
 * @return  protocol type
 */
BSL_UIO_TransportType BSL_UIO_GetTransportType(const BSL_UIO *uio);

/**
 * @ingroup bsl
 *
 * @brief   Obtain the UIO transmission protocol type
 * @param   uio  [IN] UIO object.
 * @param   uioType [IN] Type of the protocol to be obtained.
 * @return  TRUE，Succeeded in obtaining the UIO type.
 * @return  FALSE，Failed to obtain the UIO type.
 */
bool BSL_UIO_GetUioChainTransportType(BSL_UIO *uio, const BSL_UIO_TransportType uioType);

/**
 * @ingroup bsl
 *
 * @brief   Set the user data in the UIO object
 * @details UIO will not modify the user data, user can add some information
 *      for the UIO, and get the information by use BSL_UIO_GetUserData function; After you set user data by calling
 *      BSL_UIO_SetUserData, you need to call BSL_UIO_SetUserData again before calling BSL_UIO_Free to set
 *      user data to null to ensure that all memory is released.
 * @param   uio   [IN] UIO object。
 * @param   data  [IN] User data pointer
 * @retval  BSL_SUCCESS, success.
 * @retval  BSL_NULL_INPUT, invalid null pointer.
 */
int32_t BSL_UIO_SetUserData(BSL_UIO *uio, void *data);

/**
 * @ingroup bsl
 *
 * @brief   Release the user data set in the UIO object.
 * @details Free uio->userData at BSL_UIO_Free.
 * @param   uio   [IN] UIO object
 * @param   data  [IN] Pointer to the function for releasing user data
 * @retval  BSL_SUCCESS, success.
 * @retval  BSL_NULL_INPUT, invalid null pointer.
 */
int32_t BSL_UIO_SetUserDataFreeFunc(BSL_UIO *uio, BSL_UIO_USERDATA_FREE_FUNC userDataFreeFunc);

/**
 * @ingroup bsl
 *
 * @brief   Obtain the user data in the UIO object.
 * @details The user data comes from users, and tls will not change any thing
 *          for user data, user can add some customize information.
 * @param   uio   [IN] UIO object.
 * @retval  Succeeded in obtaining the data structure pointer stored by the user.
 * @retval  NULL, the obtained data does not exist.
 */
void *BSL_UIO_GetUserData(const BSL_UIO *uio);

/**
 * @ingroup bsl
 * @brief Obtains whether resources associated with the UIO are closed by the UIO.
 *
 * @param uio [OUT] UIO object
 *
 * @retval ture The resources associated with the UIO are closed by the UIO.
 * @retval false The resources associated with the UIO are not closed by the UIO.
 */
bool BSL_UIO_GetIsUnderlyingClosedByUio(const BSL_UIO *uio);

/**
 * @ingroup bsl
 * @brief Set whether resources associated with the UIO are closed by the UIO.
 *
 * @param uio [IN/OUT] UIO object
 * @param close [IN] true UIO-associated resources are closed by the UIO.
 *                   false The resources associated with the UIO are not closed by the UIO.
 */
void BSL_UIO_SetIsUnderlyingClosedByUio(BSL_UIO *uio, bool close);

/**
 * @ingroup bsl
 * @brief Method for obtaining the UIO
 *
 * @param uio [IN/OUT] UIO object
 *
 * @return UIO method
 */
const BSL_UIO_Method *BSL_UIO_GetMethod(const BSL_UIO *uio);

/**
 * @ingroup bsl
 * @brief Method for obtaining the UIO
 *
 * @param uio [IN/OUT] UIO object
 *
 * @return UIO method
 */
BSL_UIO_Method *BSL_UIO_GetMethodWithoutConst(BSL_UIO *uio);

/**
 * @ingroup bsl
 * @brief Obtain the implementation-related context.
 * @param uio [IN] UIO object
 * @return Implementation-related context pointer
 */
void *BSL_UIO_GetCtx(const BSL_UIO *uio);

/**
 * @ingroup bsl
 * @brief Set the implementation-related context.
 * @param uio [IN] UIO object
 * @param ctx [IN] Implement the relevant context pointer.
 */
void BSL_UIO_SetCtx(BSL_UIO *uio, void *ctx);

/**
 * @ingroup bsl
 *
 * @brief   Set the ctxlen of the UIO object
 * @param   uio [IN] UIO object
 * @param   len [IN] length
 */
void BSL_UIO_SetCtxLen(BSL_UIO *uio, uint32_t len);

/**
 * @ingroup bsl
 *
 * @brief   Set the FD of the UIO object
 * @param   uio [IN] UIO object
 * @param   fd [IN] File Descriptor fd
 */
void BSL_UIO_SetFD(BSL_UIO *uio, int fd);

/**
 * @ingroup bsl
 * @brief Set the UIO object flag.
 * @param uio [IN] UIO object
 * @param flags [IN] flag
 * @return BSL_SUCCESS, succeeded.
 * @return Other reference: bsl_errno.h.
 */
int32_t BSL_UIO_SetFlags(BSL_UIO *uio, uint32_t flags);

/**
 * @ingroup bsl
 * @brief Clear the UIO object flag
 * @param uio [IN] UIO object
 * @param flags [IN] flag
 * @return BSL_SUCCESS, succeeded.
 * @return Other reference: bsl_errno.h.
 */
int32_t BSL_UIO_ClearFlags(BSL_UIO *uio, uint32_t flags);

/**
 * @ingroup bsl
 * @brief Check the UIO object flag
 * @param uio [IN] UIO object
 * @param flags [IN] To-be-checked flag
 * @param out [OUT] Mark the detection result
 * @return BSL_SUCCESS, succeeded.
 * @return Other reference: bsl_errno.h
 */
uint32_t BSL_UIO_TestFlags(const BSL_UIO *uio, uint32_t flags, uint32_t *out);

/**
 * @ingroup bsl
 * @brief    Set the value of uio reference counting to 1
 *
 * @attention Call BSL_UIO_Free to decrease the value of reference counting by 1
 * @param   uio [IN] uio object
 *
 * @retval BSL_SUCCESS, the setting is successful.
 * @retval BSL_INTERNAL_EXCEPTION, an unexpected internal error occurs.
 * @retval BSL_UIO_REF_MAX, The number of UIO objects has reached the maximum.
 */
int32_t BSL_UIO_UpRef(BSL_UIO *uio);

/**
 * @ingroup bsl
 * @brief   Add a UIO object to the tail of the chain.
 *
 * @attention The reference counting of the added UIO object will not increase by 1.
 * @param   uio [IN] uio object
 * @param   tail [IN] UIO object added to the tail
 *
 * @retval BSL_SUCCESS, success.
 * @retval Non-BSL_SUCCESS, failure. For details, see bsl_errno.h.
 */
int32_t BSL_UIO_Append(BSL_UIO *uio, BSL_UIO *tail);

/**
 * @ingroup bsl
 * @brief   Pop UIO object from the chain.
 *
 * @attention The reference counting of the added UIO object does not decrease by 1.
 * @param   uio [IN] UIO object of the pop-up link.
 *
 * @return The next UIO object in the chain.
 */
BSL_UIO *BSL_UIO_PopCurrent(BSL_UIO *uio);

/**
 * @ingroup bsl
 * @brief   Release UIO object b and its subsequent chains.
 *
 * @attention: The release starts from b.
 * If the reference counting of a UIO object in the chain is greater than or equal to 1, the release stops
 * @param   uio [IN] First UIO object in the UIO object chain to be released
 */
void BSL_UIO_FreeChain(BSL_UIO *uio);

/**
 * @ingroup bsl
 * @brief   Obtain the next UIO object in the chain.
 *
 * @param   uio [IN] UIO object
 *
 * @return Next UIO object in the chain.
 */
BSL_UIO *BSL_UIO_Next(BSL_UIO *uio);

/**
 * @ingroup bsl
 * @brief   Obtain the flags of the next UIO object in the chain and set the flags to the current UIO object.
 *
 * @param   uio [IN] UIO object
 *
 * @return Return value of BSL_UIO_SetFlags
 */
int32_t BSL_UIO_SetFlagsFromNext(BSL_UIO *uio);

int32_t BSL_UIO_CallbackCtrl(BSL_UIO *uio, int32_t cmd, BSL_UIO_InfoCb *callback);

/**
 * @ingroup bsl
 * @brief   Set the reason why UIO read and write operations need to be retried.
 * @param   uio [IN] UIO object
 * @param   reason [IN] UIO_Addr object
 *
 * @return None
 */
void BSL_UIO_SetRetryReason(BSL_UIO *uio, int32_t reason);

/**
 * @ingroup bsl
 * @brief   Obtain the reason why UIO read and write operations need to be retried.
 * @param   uio [IN] UIO object
 *
 * @return reason
 */
int32_t BSL_UIO_GetRetryReason(BSL_UIO *uio);

/**
 * @ingroup bsl
 * @brief Obtain the socket error code based on the returned value and determine whether to retry.
 *
 * @param result  [IN] Socket link establishment return value.
 *
 * @retval BSL_SUCCESS, you can try again.
 *         BSL_UIO_FAIL, fails and cannot be retried.
 */
int32_t BSL_UIO_SockShouldRetry(int32_t result);

#ifdef __cplusplus
}
#endif

#endif /* HITLS_H */
