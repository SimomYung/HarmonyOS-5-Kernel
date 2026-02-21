/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2024. All rights reserved.
 * Description: mlps channel header file
 * Author: jiangzimu <jiangzimu@huawei.com>
 * Create: 2022-02-16
 */

#ifndef __MLPS_CHANNEL_H__
#define __MLPS_CHANNEL_H__

#define MLPSHBIO                         0xB1
#define MLPS_IOCTL_MLPS_START          _IOW(MLPSHBIO, 0x01, short)
#define MLPS_IOCTL_MLPS_STOP           _IOW(MLPSHBIO, 0x02, short)
#define MLPS_IOCTL_MLPS_ATTR_START     _IOW(MLPSHBIO, 0x03, short)
#define MLPS_IOCTL_MLPS_ATTR_STOP      _IOW(MLPSHBIO, 0x04, short)
#define MLPS_IOCTL_MLPS_INTERVAL_SET   _IOW(MLPSHBIO, 0x05, short)
#define MLPS_IOCTL_GET_APP_VERSION     _IOW(MLPSHBIO, 0x06, short)
#ifndef SUCCESS
#define SUCCESS (0)
#endif
#ifndef FAIL
#define FAIL (-1)
#endif
#define MIN_LEN                  (12)
#define MAX_LEN                  (2048)
#define NORMAL_LEN               (8)
#define BIT_1                    (1)
#define MLPS_BIT_MAP_SIZE        (32)
#define MLPS_FD_DEFAULT          (-1)
#define MLPS_RECOVERY_LOW        0XFF
#define MLPS_RECOVERY_HIGH       0XFF
#define MLPS_RECOVERY_DATA_LEN   8
#define MLPS_RECOVERY_LEN        4
#define FILE_LIMIT_GNSS          0660
#define MLPS_SET_GNSS_ON         1
#define MLPS_SET_GNSS_CLOSE      0
#define MLPS_PRIVACY             0XFFFF
#define IPV4_ADDR_LEN            4
#define IPV6_ADDR_LEN            16
#define MAC_ADDR_LEN             6
#define PUHS_OPEN_GNSS_SUC       0X0E00
#define PUHS_OPEN_GNSS_FAIL      0X0E01
#define PUSH_CLOSE_SUC           0X0E02
#define PUSH_IPV6_ADDR           0X0E03
#define PUSH_MLPS_ERR            0X0E04
#define PUSH_STOP_CNF            0X9
#define PUSH_PARAM_CNF           2
#define PUSH_START_PROXY_REQ     6
#define DATA_LEN_16              16
#define DATA_LEN_12              12
#define DATA_LEN_8               8
#define DATA_LEN_4               4
#define MAC_HDR_LEN              14
#define IPV4_RETRY_TIME          2
#define IPV6_RETRY_TIME          5
#define IPV6_RETRY_MAX           1
#define MLPS_WAKEUP_MSEC         50
#define PUSH_WARN_IND            18
#define MLPS_BLOCK               1
#define SESSION_START_DEFAULT    1
#define MLPS_NOTIFY_TA           5
#define NOTIFY_HAL_READY_CMD_ID  8
#define MLPS_COMPATIBLE_NAME     "hisilicon,mlps"

typedef enum {
	MLPS_MPXX,
	MAX_CONN_FUNCTION_NUM,
} mlps_connectivity_tag_list;

typedef enum {
	MLPS_WAIT_GNSS_OPERATION = -3,
	MLPS_NOT_SUPPORT_GNSS_OPERATION,
	MLPS_OPERATE_GNSS_FAIL,
	MLPS_OPERATE_GNSS_SUCCESS,
	MLPS_OPERATE_GNSS_OPEN_SUCCESS = 0,
	MLPS_OPERATE_GNSS_CLOSE_SUCCESS,
} mlps_operate_gnss_status_list;

typedef enum {
	INSIDE_APP_START = 0,
	SOFTBUS_HEARTBEAT_ALIVE = INSIDE_APP_START,
	SOFTBUS_DECISION_CENTER,
	HMSCORE_FIND_NETWORK,
	HMSCORE_NEARBY_BEACON,
	PUSHPROXY_WIFI_HEARTBEAT_AGENT,
	INSIDE_APP_END,
	OUTSIDE_APP_START = 0x18,
	SPACE_AUDIO = OUTSIDE_APP_START,
	BASTET,
	MODULE_END, // max 31
} mlps_app_t;

typedef enum {
	MLPS_CMD_OPEN,  //  ap -> sensorhub
	MLPS_CMD_CLOSE,
	MLPS_CMD_CONFIG,
	MLPS_CMD_UPLOAD,  // sensorhub -> ap
	MLPS_CMD_ACK,
} mlps_cmd_list;

typedef enum {
	APP_START = 0,
	APP_HMSCORE,
	APP_SOFTBUS,
	APP_PUSHPROXY,
	APP_VDR_HIGEO,
	APP_MAX,
	GNSS_POWER_FAIL = 0XFFFC,
	APP_PALACE_MUSEUM = 0XFFFD,
	APP_PRIVACY = 0XFFFE,
	MLPS_SENSORHUB_RECOVERY = 0XFFFF,
} mlps_app_type_t;

typedef enum {
	SOFT_MODULE_START = 0,
	HEARTBEAT_ALIVE,
	DECISION_CENTER,
	SOFTBUS_MODULE_MAX,
} softbus_module_type_t;

typedef enum {
	HMS_CORE_MODULE_START = 0,
	MODULE_NEARBY_BEACON = 0x06,
	MODULE_FIND_NETWORK = 0x07,
	HMSCORE_MODULE_MAX,
} hmscore_module_type_t;

typedef enum {
	TO_SH_DEVICE_INFO = 0,
	TO_SH_TIMESTAMP_SYNC,
	TO_AP_DEVICE_INFO_LIST = 0x0F,
	TO_AP_DEVICE_INFO,
	TO_AP_BLE_CONFIG,
	TO_AP_BLE_SCAN_RESULT,
	TO_AP_DEVICE_STATUS,
	TO_AP_DECISION_DATA,
} ap_and_sh_type;

typedef enum {
	TEE_PARAM_0 = 0,
	TEE_PARAM_1,
	TEE_PARAM_2,
	TEE_PARAM_3,
} tee_param_t;

typedef enum {
	ST_PUSH_FAIL = -1,
	ST_PUSH_DEFAULT = 0,
	ST_PUSH_OPEN_WAIT,
	ST_PUSH_OPEN_SUC,
	ST_PUSH_CLOSE,
} push_gnss_status_t;

typedef enum {
	NORMAL_PROCESS,
	SKIP_PROCESS,
} upload_process_t;

typedef enum {
	PUSH_SEND_RAW = 0x0F00,
	PUSH_LINK_START = PUSH_SEND_RAW,
	PUSH_LINK_1 = 0x0F01,
	PUSH_LINK_2,
	// add new link here
	PUSH_LINK_MAX,
} push_large_traffic_cmd_t;

typedef enum {
	ERRNO_START = 10000,
	RAW_SOCK_SEND_ERR = ERRNO_START,
	RAW_SOCK_CREATE_ERR,
	RAW_SOCK_OP_ERR,
	NETFILTER_V4_REGISTER_ERR,
	NETFILTER_V6_REGISTER_ERR,
	NETFILTER_HOOK_DATA_ERR,
	NETFILTER_UPDATE_FILTER_ERR, // del
	NO_NETWORK_ADAPTER,
	GET_IPV4_MAC_ERR,
	GET_IPV4_ADDR_ERR,
	GET_IPV4_GATEWAY_ERR,
	GET_IPV4_NO_IFA,
	GET_IPV6_MAC_ERR,
	GET_IPV6_ADDR_ERR,
	GET_IPV6_GATEWAY_ERR,
	// add new errno here
	ERRNO_MAX,
} push_mlps_err_event_t;

typedef struct {
	unsigned int subcmd;
	unsigned char value[];
} pkt_mlps_sub_header_t;

typedef struct {
	unsigned short mlps_app_type;
	unsigned short app_len;
	unsigned short mlps_module_type;
	unsigned short module_len;
	unsigned short mlps_cmd;
	unsigned short cmd_len;
	unsigned char value[];
} pkt_mlps_detail_data_t;

typedef struct {
	unsigned short cmd;
	unsigned short cmd_len;
	unsigned char value[];
} pkt_mlps_cmd_data_t;

typedef struct {
	unsigned int subcmd;
	unsigned short mlps_app_type;
	unsigned short app_len;
	unsigned short mlps_module_type;
	unsigned short module_len;
	unsigned short config_type;
	unsigned short config_len;
	unsigned short cmd;
	unsigned short cmd_len;
	unsigned char value[];
} pkt_wifi_detail_data_t;

#pragma pack(1)
typedef struct push_wf_send_data {
	unsigned char addr_len;
	unsigned char addr[IPV6_ADDR_LEN];
	unsigned short port;
	unsigned int data_len;
	unsigned char data[0];
} push_wf_send_data_t;

typedef struct push_wf_hook_data {
	unsigned char addr_len;
	unsigned char addr[IPV6_ADDR_LEN];
	unsigned short port;
	unsigned char flag; // open 1; close 0
} push_wf_hook_data_t;

typedef struct {
	unsigned short link_id;
	unsigned char addr[IPV4_ADDR_LEN];
	unsigned short port;
	unsigned char flag;
} push_wf_hook_v4_filter;

typedef struct {
	unsigned short link_id;
	unsigned char addr[IPV6_ADDR_LEN];
	unsigned short port;
	unsigned char flag;
} push_wf_hook_v6_filter;
#pragma pack()

#pragma pack(4)
typedef struct {
	unsigned char ipv4_addr[IPV4_ADDR_LEN];
	unsigned char ipv6_addr[IPV6_ADDR_LEN];
	unsigned char mac[MAC_ADDR_LEN];
	unsigned char next_hop_mac[MAC_ADDR_LEN];
	unsigned char next_hop_ip[IPV4_ADDR_LEN];
} push_proxy_start_req_t;
#pragma pack()

typedef struct {
	unsigned short mlps_app_type;
	unsigned short app_len;
	unsigned short mlps_module_type;
	unsigned short module_len;
	unsigned short config_type;
	unsigned short config_len;
	unsigned short cmd;
	unsigned short cmd_len;
	unsigned int warnmsgcnt;
	long long time;
	int event;
	int reason;
	int subreason;
	int len;
	int reasons[44];
} mlps_push_err_t;

typedef struct {
	unsigned int app_type;
	unsigned int version1;
	unsigned int version2;
} app_version_t;

typedef struct {
	unsigned int app_type;
	unsigned int push_proxy_version;
	unsigned int push_cloud_version;
} push_version_t;

typedef struct {
	unsigned int app_type;
	unsigned int softbus_version;
	unsigned int rsv;
} softbus_version_t;

typedef struct {
	unsigned int ipv4_flag : 1;
	unsigned int ipv6_flag : 1;
	unsigned int azIndex : 4;
	unsigned int resv : 26;
	unsigned short v4_port;
	unsigned short v6_port;
	unsigned char ipv4_addr[IPV4_ADDR_LEN];
	unsigned char ipv6_addr[IPV6_ADDR_LEN];	
} push_proxy_dest_ip_addr;

typedef struct {
	unsigned int ip_addr_num;
	push_proxy_dest_ip_addr addr_info[DATA_LEN_4];
} push_proxy_ip_info;

typedef int (*connectivity_callback)(int para);
#ifdef CONFIG_MLPS_OPERATE_CONN
/*
 * Function    : mlps_ops_conn_function_register
 * Description : register function to make mlps operate connectivity function
 * Input       : [tag] tag for different connectivities
 *             : [func] connectivity function
 * Output      : none
 * Return      : 0:successs, -1:fail
 */
int mlps_ops_conn_function_register(unsigned int tag, connectivity_callback func);

/*
 * Function    : mlps_operate_gnss_power
 * Description : mlps operate gnss cpu power on/off for open/close the i3c route in c
 * Input       : [cmd] open : 0, close : 1 (mlps_cmd_list only support open/close)
 *             : [service] service : service of mlps (mlps_app_t)
 * Output      : none
 * Return      : 0:successs, -1:fail, -2:unsupported
 */
int mlps_operate_gnss_power(unsigned short cmd, unsigned int service);

/*
 * Function    : get_mlps_operate_gnss_status
 * Description : show whether mlps operates gnss success
 * Input       : none
 * Output      : none
 * Return      : 0:successs, -1:fail, -2:unsupported, -3:waiting operation
 */
int get_mlps_operate_gnss_status(void);
#endif
#endif