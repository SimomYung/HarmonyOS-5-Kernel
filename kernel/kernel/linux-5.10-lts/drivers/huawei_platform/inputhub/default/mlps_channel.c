/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2024. All rights reserved.
 * Description: mlps channel source file
 * Author: jiangzimu <jiangzimu@huawei.com>
 * Create: 2022-02-16
 */

#include "mlps_channel.h"

#include <linux/fs.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>
#include <linux/timer.h>

#include "contexthub_boot.h"
#include "contexthub_ext_log.h"
#include "contexthub_recovery.h"
#include "contexthub_route.h"
#include "sensor_info.h"
#include <securec.h>
#include <platform_include/smart/linux/base/ap/protocol.h>

#include "teek_client_api.h"

#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/netfilter_ipv6.h>
#include <linux/skbuff.h>
#include <linux/net.h>
#include <linux/netdevice.h>
#include <linux/inetdevice.h>
#include <linux/netlink.h>
#include <linux/skbuff.h>
#include <linux/inet.h>
#include <linux/if_ether.h>
#include <linux/string.h>
#include <linux/pm_wakeup.h>

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
#define PUSH_START_PROXY_REQ     6
#define DATA_LEN_16              16
#define DATA_LEN_12              12
#define DATA_LEN_8               8
#define DATA_LEN_4               4
#define MAC_HDR_LEN              14
#define IPV6_RETRY_MAX           3
#define MLPS_WAKEUP_MSEC         50
#define MLPS_COMPATIBLE_NAME     "hisilicon,mlps"

#define MLPS_TA_UUID { 0x53933180, 0xc8b0, 0x4a83, \
	{ 0x88, 0xd7, 0x8c, 0x76, 0xd2, 0x11, 0x7d, 0x19 } }

#define SESSION_START_DEFAULT    1
#define MLPS_NOTIFY_TA           5
#define NOTIFY_HAL_READY_CMD_ID  8
int32_t(*mlps_ops_conn_function[MAX_CONN_FUNCTION_NUM])(int32_t uart);
static struct workqueue_struct *g_mlps_gnss_wq = NULL;
static struct workqueue_struct *g_mlps_wifi_wq = NULL;
static struct workqueue_struct *g_mlps_wifi_send_wq = NULL;
static struct workqueue_struct *g_mlps_privacy_ta_wq = NULL;
struct mlps_gnss_work {
	struct work_struct worker;
	int cmd;
	int service;
};
struct mlps_privacy_ta_work {
	struct work_struct worker;
};
static bool g_support_conn_operation = false;
static int g_gnss_status = MLPS_WAIT_GNSS_OPERATION;
static const uint8_t g_dev_name[] = "wlan0";
typedef enum {
	APP_START = 0,
	APP_HMSCORE,
	APP_SOFTBUS,
	APP_PUSHPROXY,
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
	ERRNO_START,
	SEND_RAW_SOCK_ERR,
	// add new errno here
	ERRNO_MAX,
} push_mlps_errno_t;

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

struct mlps_wifi_work {
	struct work_struct worker;
	int len;
	pkt_wifi_detail_data_t *data;
};

struct mlps_cmd_map {
	int mlps_ioctl_app_cmd;
	int tag;
	enum obj_cmd cmd;
};

struct mlps_sucmd_map {
	mlps_app_type_t type;
	enum obj_sub_cmd subcmd;
};

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

struct mlps_wifi_send_work {
	struct work_struct worker;
	push_wf_send_data_t pg;
};

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

static int64_t g_mlps_ref_cnt;
static unsigned int g_mlps_app_bit_map = 0;
static unsigned int g_outside_app_bit_map = 0;
static bool g_privacy_mode = false;
static bool g_palace_mode = false;
static unsigned int g_privacy_timer_retry = 0;
static unsigned int g_palace_timer_retry = 0;
static unsigned int g_get_ipv6_retry_cnt = 0;
static TEEC_Context g_mlps_context = {0};
static TEEC_Session g_mlps_session = {0};
static const u8 *g_mlps_package_name = "/dev/mlpskernel_ca";
static u32 g_uid;
struct timer_list privacy_gnss_timer;
struct timer_list palace_museum_gnss_timer;
struct timer_list get_ipv6_gnss_timer;
static struct wakeup_source *mlps_wl;
static uint8_t g_push_gnss_status = ST_PUSH_DEFAULT;
static uint8_t g_push_link_status[2] = {0}; // 0  ipv4  1 ipv6
static spinlock_t link_hook_lock;
static push_wf_hook_v4_filter g_v4_f[PUSH_LINK_MAX - PUSH_LINK_1] = { 0 };
static push_wf_hook_v6_filter g_v6_f[PUSH_LINK_MAX - PUSH_LINK_1] = { 0 };
static push_version_t g_push_version = {
	.app_type           = APP_PUSHPROXY,
	.push_proxy_version = 0,
	.push_cloud_version = 0,
};
static softbus_version_t g_softbus_version = {
	.app_type           = APP_SOFTBUS,
	.softbus_version    = 0,
	.rsv                = 0
};
static const struct mlps_cmd_map mlps_cmd_map_tab[] = {
	{ MLPS_IOCTL_MLPS_START, TAG_MLPS, CMD_CMN_OPEN_REQ },
	{ MLPS_IOCTL_MLPS_STOP, TAG_MLPS, CMD_CMN_CLOSE_REQ },
	{ MLPS_IOCTL_MLPS_ATTR_START, TAG_MLPS, CMD_CMN_CONFIG_REQ },
	{ MLPS_IOCTL_MLPS_ATTR_STOP, TAG_MLPS, CMD_CMN_CONFIG_REQ },
	{ MLPS_IOCTL_MLPS_INTERVAL_SET, TAG_MLPS, CMD_CMN_INTERVAL_REQ },
};

static int send_mlps_cmd_ext(void *data, ssize_t len,
	struct read_info *pkg_mcu, bool use_lock);
static void mlps_wifi_handle(struct work_struct *work);
static ssize_t upload_mlps_data(char *data, ssize_t len);

static unsigned int bit_check(unsigned int *map, unsigned int bit)
{
	return ((*map) & ((unsigned int)BIT_1 << ((bit) % MLPS_BIT_MAP_SIZE)));
}

static void bit_set(unsigned int *map, unsigned int bit)
{
	*map |= ((unsigned int)BIT_1 << ((bit) % MLPS_BIT_MAP_SIZE));
	return;
}

static void bit_clear(unsigned int *map, unsigned int bit)
{
	*map &= ~((unsigned int)BIT_1 << ((bit) % MLPS_BIT_MAP_SIZE));
	return;
}

int get_mlps_operate_gnss_status(void)
{
	return g_gnss_status;
}

static int mlps_ops_conn_function_echo(int uart)
{
	hwlog_info("%s enter\n", __func__);
	return MLPS_OPERATE_GNSS_SUCCESS;
}

static void mlps_ops_conn_function_init(void)
{
	int i;

	for (i = 0; i < MAX_CONN_FUNCTION_NUM; i++)
		mlps_ops_conn_function[i] = mlps_ops_conn_function_echo;
}

int mlps_ops_conn_function_register(unsigned int tag, connectivity_callback func)
{
	hwlog_info("%s enter\n", __func__);
	if (func == NULL  || tag >= MAX_CONN_FUNCTION_NUM) {
		hwlog_err("%s,register function is null or tag is error\n", __func__);
		return FAIL;
	}

	if (mlps_ops_conn_function[tag] != mlps_ops_conn_function_echo) {
		hwlog_err("CallbackExist:%u\n", tag);
		return FAIL;
	}

	mlps_ops_conn_function[tag] = func;
	hwlog_info("%s success\n", __func__);
	return SUCCESS;
}

static int mlps_privacy_open_ta_session(void)
{
	TEEC_Result result;
	TEEC_Operation op;
	TEEC_UUID uuid = MLPS_TA_UUID;
/* 1. initialize context */
	result = TEEK_InitializeContext(NULL, &g_mlps_context);
	if (result != TEEC_SUCCESS) {
		hwlog_err("%s teec initial failed", __func__);
		return FAIL;
	}
	hwlog_info("%s teec initial done", __func__);

/* 2. open session */
	u32 err_origin = 0;
	if (memset_s(&op, sizeof(op), 0, sizeof(op)) != EOK) {
		hwlog_err("%s memset_s failed", __func__);
		TEEK_FinalizeContext(&g_mlps_context);
		return FAIL;
	}

	// prepare operation parameters
	op.started = SESSION_START_DEFAULT;
	// params[2] is TEEC_MEMREF_TEMP_INPUT
	op.params[TEE_PARAM_2].tmpref.buffer = (void *)&g_uid;
	op.params[TEE_PARAM_2].tmpref.size = sizeof(g_uid);
	// params[3] is TEEC_MEMREF_TEMP_INPUT
	op.params[TEE_PARAM_3].tmpref.buffer = (void *)g_mlps_package_name;
	op.params[TEE_PARAM_3].tmpref.size = strlen(g_mlps_package_name) + 1;
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_NONE, TEEC_NONE,
		TEEC_MEMREF_TEMP_INPUT, TEEC_MEMREF_TEMP_INPUT);
	// Open a session to the TA
	result = TEEK_OpenSession(&g_mlps_context, &g_mlps_session, &uuid,
			TEEC_LOGIN_IDENTIFY, NULL, &op, &err_origin);
	if (result != TEEC_SUCCESS) {
		hwlog_err("%s open session=%d and err_origin is %u",
			__func__, result, err_origin);
		TEEK_FinalizeContext(&g_mlps_context);
		return FAIL;
	}

	return SUCCESS;
}

static void mlps_privacy_invoke_ta(void)
{
	TEEC_Result result;
	TEEC_Operation op_invoke;
	u32 err_origin = 0;
/* 3. set in buffer and out buffer */
	if (memset_s(&op_invoke, sizeof(op_invoke), 0, sizeof(op_invoke)) != EOK) {
		hwlog_err("%s memset_s failed", __func__);
		return;
	}
	op_invoke.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
		TEEC_VALUE_OUTPUT, TEEC_NONE, TEEC_NONE);
	op_invoke.started = SESSION_START_DEFAULT;
	op_invoke.params[TEE_PARAM_0].value.a = MLPS_NOTIFY_TA;
/* 4. invoke command to ta */
	result = TEEK_InvokeCommand(&g_mlps_session,
		NOTIFY_HAL_READY_CMD_ID, &op_invoke, &err_origin);
	if (result != TEEC_SUCCESS) {
		hwlog_err("%s TEEK_InvokeCommand failed res=0x%x orig=0x%x \n",
			__func__, result, err_origin);
		return;
	}
	hwlog_info("%s TEEK_InvokeCommand called successfully %d",
		__func__, op_invoke.params[TEE_PARAM_1].value.a);
}

static void mlps_privacy_op_ta(void)
{
	int ret = mlps_privacy_open_ta_session();
	if (ret == FAIL)
		return;

	mlps_privacy_invoke_ta();

	TEEK_CloseSession(&g_mlps_session);
	TEEK_FinalizeContext(&g_mlps_context);
}

static void mlps_privacy_ta_handle(struct work_struct *work)
{
	struct mlps_privacy_ta_work *p = container_of(work, struct mlps_privacy_ta_work, worker);
	mlps_privacy_op_ta();
	kfree(p);

	return;
}

static void mlps_privacy_notify_ta(void)
{
	struct mlps_privacy_ta_work *ta_work = NULL;
	ta_work = kzalloc(sizeof(struct mlps_privacy_ta_work), GFP_ATOMIC);
	if (ZERO_OR_NULL_PTR((unsigned long)(uintptr_t)ta_work)) {
		pr_err("[%s] alloc gnss_work failed.\n", __func__);
		return;
	}

	INIT_WORK(&ta_work->worker, mlps_privacy_ta_handle);
	queue_work(g_mlps_privacy_ta_wq, &ta_work->worker);
}

static void mlps_notify_pushproxy_open_gnss(unsigned short op_ret)
{
	char *data = (char *)kzalloc(DATA_LEN_16 + SUBCMD_LEN, GFP_KERNEL);
	if (data == NULL) {
		hwlog_err("%s data kzalloc fail\n", __func__);
		return;
	}

	pkt_mlps_sub_header_t *temp = (pkt_mlps_sub_header_t *)data;
	temp->subcmd = SUB_CMD_MLPS_CONFIG_REQ;
	pkt_mlps_detail_data_t *tmp = (pkt_mlps_detail_data_t *)temp->value;
	tmp->mlps_app_type = APP_PUSHPROXY;
	tmp->app_len = DATA_LEN_12;
	tmp->mlps_module_type = 0;
	tmp->module_len = DATA_LEN_8;
	tmp->mlps_cmd = MLPS_CMD_CONFIG;
	tmp->cmd_len = DATA_LEN_4;
	pkt_mlps_cmd_data_t *value = (pkt_mlps_cmd_data_t *)tmp->value;
	value->cmd = op_ret;
	value->cmd_len = 0;
	hwlog_info("%s send opt 0x%x\n", __func__, op_ret);
	int ret = send_mlps_cmd_ext(data, DATA_LEN_16, NULL, true);

	if (data != NULL) {
		kfree(data);
		data = NULL;
	}
}

static void mlps_notify_pushproxy_opt_gnss(unsigned int cmd)
{
	if (cmd == MLPS_SET_GNSS_ON) {
		g_push_gnss_status = ST_PUSH_OPEN_SUC;
		mlps_notify_pushproxy_open_gnss(PUHS_OPEN_GNSS_SUC);
	} else if (cmd == MLPS_SET_GNSS_CLOSE) {
		g_push_gnss_status = ST_PUSH_CLOSE;
	} else {
		hwlog_err("%s no support cmd\n", __func__);
	}
}

static void mlps_update_gnss_power_fail(void)
{
	unsigned short mlps_upload_data[DATA_LEN_4] = {GNSS_POWER_FAIL, MLPS_RECOVERY_LEN, 0, 0};
	upload_mlps_data((char *)mlps_upload_data, MLPS_RECOVERY_DATA_LEN);
}

static int mlps_operate_gnss(unsigned int cmd)
{
	if ((g_privacy_mode || g_palace_mode) && cmd == MLPS_SET_GNSS_ON) {
		hwlog_info("%s is in privacy or palace\n", __func__);
		return MLPS_OPERATE_GNSS_FAIL;
	}

	hwlog_info("%s enter\n", __func__);
	int ret = 0;
	int i = 0;
	for (i = 0; i < MAX_CONN_FUNCTION_NUM; i++)
		ret |= mlps_ops_conn_function[i](cmd);

	if (ret != 0) {
		hwlog_err("%s failed, ret is %d\n", __func__, ret);
		return MLPS_OPERATE_GNSS_FAIL;
	}

	hwlog_info("%s, cmd: %d success\n", __func__, cmd);
	return MLPS_OPERATE_GNSS_SUCCESS;
}

static bool mlps_check_bit_map(void)
{
	int i = 0;
	for (i = 0; i < MODULE_END; i++) {
		if (bit_check(&g_mlps_app_bit_map, i))
			return true;
	}

	return false;
}

static int mlps_open_gnss_power(unsigned int service)
{
	int ret = MLPS_OPERATE_GNSS_SUCCESS;
	hwlog_info("%s enter, service: %u, g_mlps_app_bit_map: %u\n", __func__, service, g_mlps_app_bit_map);
	if (bit_check(&g_mlps_app_bit_map, service)) {
		hwlog_warn("%s service %d has opened\n", __func__, service);
		if (service == PUSHPROXY_WIFI_HEARTBEAT_AGENT)
			mlps_notify_pushproxy_opt_gnss(MLPS_SET_GNSS_ON);
		return ret;
	}

	if (mlps_check_bit_map() == false) {
		ret = mlps_operate_gnss(MLPS_SET_GNSS_ON);
		hwlog_warn("%s service %d open success ret = %d\n", __func__, service, ret);
	}

	if (ret == MLPS_OPERATE_GNSS_FAIL) {
		mlps_update_gnss_power_fail();
		if (service == PUSHPROXY_WIFI_HEARTBEAT_AGENT)
			g_push_gnss_status = ST_PUSH_FAIL;
		return ret;
	}

	bit_set(&g_mlps_app_bit_map, service);
	if (service == PUSHPROXY_WIFI_HEARTBEAT_AGENT)
		mlps_notify_pushproxy_opt_gnss(MLPS_SET_GNSS_ON);

	return ret;
}

static int mlps_close_gnss_power(unsigned int service)
{
	int ret = MLPS_OPERATE_GNSS_SUCCESS;
	hwlog_info("%s enter, service: %u, g_mlps_app_bit_map: %u\n", __func__, service, g_mlps_app_bit_map);
	if (!bit_check(&g_mlps_app_bit_map, service)) {
		hwlog_warn("%s service %d has closed\n", __func__, service);
		if (service == PUSHPROXY_WIFI_HEARTBEAT_AGENT)
			mlps_notify_pushproxy_opt_gnss(MLPS_SET_GNSS_CLOSE);
		return ret;
	}

	bit_clear(&g_mlps_app_bit_map, service);

	if (mlps_check_bit_map() == false) {
		ret = mlps_operate_gnss(MLPS_SET_GNSS_CLOSE);
		hwlog_warn("%s close success\n", __func__);
	}

	if (service == PUSHPROXY_WIFI_HEARTBEAT_AGENT)
		mlps_notify_pushproxy_opt_gnss(MLPS_SET_GNSS_CLOSE);

	return ret;
}

static void mlps_gnss_handle(struct work_struct *work)
{
	int ret = MLPS_OPERATE_GNSS_SUCCESS;
	struct mlps_gnss_work *p = container_of(work, struct mlps_gnss_work, worker);
	if (p->cmd == MLPS_CMD_OPEN) {
		ret = mlps_open_gnss_power(p->service);
	} else if (p->cmd == MLPS_CMD_CLOSE) {
		ret = mlps_close_gnss_power(p->service);
	} else {
		ret = MLPS_NOT_SUPPORT_GNSS_OPERATION;
		hwlog_warn("%s unkonwn cmd %u\n", __func__, p->cmd);
	}
	if (ret == MLPS_OPERATE_GNSS_SUCCESS) {
		g_gnss_status = MLPS_OPERATE_GNSS_OPEN_SUCCESS;
		if (p->cmd == MLPS_CMD_CLOSE)
			g_gnss_status = MLPS_OPERATE_GNSS_CLOSE_SUCCESS;
	} else {
		g_gnss_status = ret;
	}
	kfree(p);
	return;
}

#ifdef CONFIG_MLPS_OPERATE_CONN
static int mlps_operate_gnss_handle(unsigned short cmd, unsigned int service)
{
	hwlog_info("%s, cmd: %u, service: %u\n", __func__, cmd, service);
	int ret = MLPS_OPERATE_GNSS_SUCCESS;
	g_gnss_status = MLPS_WAIT_GNSS_OPERATION;
	if (g_support_conn_operation  == false) {
		g_gnss_status = MLPS_NOT_SUPPORT_GNSS_OPERATION;
		ret = MLPS_NOT_SUPPORT_GNSS_OPERATION;
		return ret;
	}
	if (service >= MODULE_END) {
		g_gnss_status = MLPS_NOT_SUPPORT_GNSS_OPERATION;
		ret = MLPS_NOT_SUPPORT_GNSS_OPERATION;
		hwlog_warn("%s unkonwn service %u\n", __func__, service);
		return ret;
	}

	struct mlps_gnss_work *gnss_work = NULL;
	gnss_work = kzalloc(sizeof(struct mlps_gnss_work), GFP_ATOMIC);
	if (ZERO_OR_NULL_PTR((unsigned long)(uintptr_t)gnss_work)) {
		pr_err("[%s] alloc gnss_work failed.\n", __func__);
		g_gnss_status = MLPS_OPERATE_GNSS_FAIL;
		ret = MLPS_OPERATE_GNSS_FAIL;
		return ret;
	}
	gnss_work->cmd = cmd;
	gnss_work->service = service;
	INIT_WORK(&gnss_work->worker, mlps_gnss_handle);
	queue_work(g_mlps_gnss_wq, &gnss_work->worker);
	return ret;
}
#else
static int mlps_operate_gnss_handle(unsigned short cmd, unsigned int service)
{
	if (cmd == MLPS_CMD_OPEN)
		g_gnss_status = MLPS_OPERATE_GNSS_OPEN_SUCCESS;
	else
		g_gnss_status = MLPS_OPERATE_GNSS_CLOSE_SUCCESS;

	if (cmd == MLPS_CMD_OPEN && service == PUSHPROXY_WIFI_HEARTBEAT_AGENT) {
		g_push_gnss_status = ST_PUSH_OPEN_SUC;
		mlps_notify_pushproxy_open_gnss(PUHS_OPEN_GNSS_SUC);
	} else if (cmd == MLPS_CMD_CLOSE && service == PUSHPROXY_WIFI_HEARTBEAT_AGENT) {
		g_push_gnss_status = ST_PUSH_CLOSE;
	}

	return MLPS_OPERATE_GNSS_SUCCESS;
}
#endif

static void outside_app_status_record(unsigned short cmd, unsigned int service)
{
	if (cmd == MLPS_CMD_OPEN) {
		bit_set(&g_outside_app_bit_map, service);
	} else if (cmd == MLPS_CMD_CLOSE){
		bit_clear(&g_outside_app_bit_map, service);
	}
}

int mlps_operate_gnss_power(unsigned short cmd, unsigned int service)
{
	hwlog_info("%s, cmd: %u, service: %u\n", __func__, cmd, service);
	if (service >= OUTSIDE_APP_START)
		outside_app_status_record(cmd, service);

	return mlps_operate_gnss_handle(cmd, service);
}

#ifdef CONFIG_MLPS_OPERATE_CONN
static void mlps_parse_softbus_module(pkt_mlps_detail_data_t *data)
{
	switch (data->mlps_module_type) {
	case HEARTBEAT_ALIVE:
		if (data->mlps_cmd == MLPS_CMD_OPEN) {
			hwlog_info("%s SOFTBUS_HEARTBEAT_ALIVE open\n", __func__);
			mlps_operate_gnss_handle(data->mlps_cmd, SOFTBUS_HEARTBEAT_ALIVE);
		} else if (data->mlps_cmd == MLPS_CMD_CLOSE) {
			mlps_operate_gnss_handle(data->mlps_cmd, SOFTBUS_HEARTBEAT_ALIVE);
			hwlog_info("%s SOFTBUS_HEARTBEAT_ALIVE close\n", __func__);
		}
		break;

	default:
		hwlog_warn("%s unkonwn mlps softbus %d\n", __func__, data->mlps_module_type);
		break;
	}
}
#endif

static void mlps_send_ipv6_addr(pkt_wifi_detail_data_t *wf_data)
{
	struct mlps_wifi_work  *wifi_work = NULL;
	wifi_work = kzalloc(sizeof(struct mlps_wifi_work), GFP_ATOMIC);
	if (ZERO_OR_NULL_PTR((unsigned long)(uintptr_t)wifi_work)) {
		pr_err("[%s] alloc wifi_work failed.\n", __func__);
		kfree(wf_data);
		wf_data = NULL;
		return;
	}
	wifi_work->data = wf_data;
	wifi_work->len = IPV6_ADDR_LEN + DATA_LEN_16;
	INIT_WORK(&wifi_work->worker, mlps_wifi_handle);
	queue_work(g_mlps_wifi_wq, &wifi_work->worker);
}

static void mlps_process_ipv6_addr(unsigned char *addr)
{
	char *data = (char *)kzalloc(IPV6_ADDR_LEN + DATA_LEN_16 + SUBCMD_LEN, GFP_KERNEL);
	if (data == NULL) {
		hwlog_err("%s ip addr kzalloc fail!!!\n", __func__);
		return;
	}
	pkt_wifi_detail_data_t *wf_data = (pkt_wifi_detail_data_t *)data;
	(void)memset_s(data, (IPV6_ADDR_LEN + DATA_LEN_16 + SUBCMD_LEN),
		0, (IPV6_ADDR_LEN + DATA_LEN_16 + SUBCMD_LEN));
	if (memcpy_s(wf_data->value, IPV6_ADDR_LEN, addr, IPV6_ADDR_LEN) != EOK) {
		hwlog_err("%s memcpy_s fail\n", __func__);
		kfree(data);
		data = NULL;
		return;
	}
	wf_data->subcmd = SUB_CMD_MLPS_CONFIG_REQ;
	wf_data->mlps_app_type = APP_PUSHPROXY;
	wf_data->app_len = IPV6_ADDR_LEN + DATA_LEN_12;
	wf_data->mlps_module_type = 0;
	wf_data->module_len = IPV6_ADDR_LEN + DATA_LEN_8;
	wf_data->config_type = MLPS_CMD_CONFIG;
	wf_data->config_len = IPV6_ADDR_LEN + DATA_LEN_4;
	wf_data->cmd = PUSH_IPV6_ADDR;
	wf_data->cmd_len = IPV6_ADDR_LEN;

	mlps_send_ipv6_addr(wf_data);
	hwlog_info("%s send to sh sucess\n", __func__);
	return;
}

static void mlps_get_current_ipv6_info(struct net_device *dev,
	struct inet6_ifaddr *ifp, unsigned char *addr, bool is_go)
{
	bool ipv6_done = false;

	if (ifp->scope != 0) {
		hwlog_info("%s ipv6 addr is not global\n", __func__);
	} else {
		if (memcpy_s(addr, IPV6_ADDR_LEN, &ifp->addr.s6_addr, IPV6_ADDR_LEN) != EOK) {
			hwlog_err("%s memcpy v6_addr fail\n", __func__);
			return;
		}
		hwlog_info("%s ipv6 addr is global\n", __func__);
		ipv6_done = true;
		g_get_ipv6_retry_cnt = 0xFF;
	}

	if (ipv6_done) {
		if (is_go == false) {
			hwlog_info("%s retry get\n", __func__);
			mlps_process_ipv6_addr(addr);
		}
		return;
	}

	if (g_get_ipv6_retry_cnt < IPV6_RETRY_MAX) {
		hwlog_info("%s mod_timer retry = %d\n", __func__, g_get_ipv6_retry_cnt);
		mod_timer(&get_ipv6_gnss_timer, jiffies + 10 * HZ);
	}
}

static void mlps_get_current_ipv6_addr(unsigned char *addr, bool is_go)
{
	struct net_device *dev = NULL;

	dev = dev_get_by_name(&init_net, g_dev_name);
	if (dev == NULL) {
		hwlog_err("%s dev_get_by_name fail\n", __func__);
		return;
	}
	hwlog_info("%s find wlan0\n", __func__);

	struct inet6_dev *idev = in6_dev_get(dev);
	if (idev == NULL) {
		dev_put(dev);
		return;
	}

	struct inet6_ifaddr *ifp;
	rcu_read_lock();
	list_for_each_entry(ifp, &idev->addr_list, if_list) {
		mlps_get_current_ipv6_info(dev, ifp, addr, is_go);
	}
	rcu_read_unlock();
	in6_dev_put(idev);
	dev_put(dev);
}

static void mlps_get_ipv4_info(struct net_device *dev, push_proxy_start_req_t *addr)
{
    if (!(dev->flags & IFF_UP))
		return;

	struct in_device *in_dev = in_dev_get(dev);
	if (in_dev == NULL)
		return;

	struct neighbour *n = NULL;
	struct in_ifaddr *ifa;
	rcu_read_lock();
	in_dev_for_each_ifa_rcu(ifa, in_dev) {
		if (ifa->ifa_local == 0)
			continue;

		if (memcpy_s(addr->ipv4_addr, IPV4_ADDR_LEN,
			&ifa->ifa_local, IPV4_ADDR_LEN) != EOK) {
			hwlog_err("%s memcpy v4_addr fail\n", __func__);
			rcu_read_unlock();
			in_dev_put(in_dev);
			return;
		}
		uint8_t ip_addr[IPV4_ADDR_LEN] = { 0 };
		if (memcpy_s(ip_addr, IPV4_ADDR_LEN, addr->next_hop_ip, IPV4_ADDR_LEN) != EOK) {
			hwlog_err("%s memcpy v4_next_addr fail\n", __func__);
			rcu_read_unlock();
			in_dev_put(in_dev);
			return;
		}
		n = neigh_lookup(&arp_tbl, ip_addr, dev);  // ipv6 table nd_tbl
		if (!n) {
			hwlog_err("%s neigh_lookup fail\n", __func__);
			rcu_read_unlock();
			in_dev_put(in_dev);
			return;
		}
		if (n->nud_state & NUD_VALID) {
			hwlog_info("%s find neigher\n", __func__);
			if (memcpy_s(addr->next_hop_mac, MAC_ADDR_LEN, n->ha, MAC_ADDR_LEN) != EOK)
				hwlog_err("%s memcpy v4_next_hop_mac fail\n", __func__);
		}
		neigh_release(n);
	}
	rcu_read_unlock();
	in_dev_put(in_dev);
}

static void mlps_get_current_ipv4_addr(push_proxy_start_req_t *addr)
{
	struct net_device *dev = NULL;
	dev = dev_get_by_name(&init_net, g_dev_name);
	if (dev == NULL) {
		hwlog_err("%s dev_get_by_name fail\n", __func__);
		return;
	}
	hwlog_info("%s find wlan0\n", __func__);

	if (memcpy_s(addr->mac, MAC_ADDR_LEN, dev->dev_addr, MAC_ADDR_LEN) != EOK) {
		hwlog_err("%s memcpy mac_addr fail\n", __func__);
		dev_put(dev);
		return;
	}
	mlps_get_ipv4_info(dev, addr);
	dev_put(dev);
}

static void mlps_push_get_addr(push_proxy_start_req_t *addr)
{
	g_get_ipv6_retry_cnt = 0;
	mlps_get_current_ipv4_addr(addr);
	mlps_get_current_ipv6_addr(addr->ipv6_addr, true);
}

static void mlps_parse_pushproxy_module_cmd(pkt_mlps_cmd_data_t *data)
{
	switch (data->cmd) {
	case PUSH_START_PROXY_REQ:
		if (g_push_gnss_status != ST_PUSH_OPEN_WAIT) {
			g_push_gnss_status = ST_PUSH_OPEN_WAIT;
			mlps_operate_gnss_handle(MLPS_CMD_OPEN, PUSHPROXY_WIFI_HEARTBEAT_AGENT);
		}
		mlps_push_get_addr((push_proxy_start_req_t *)data->value);
		break;

	default:
		if (g_get_ipv6_retry_cnt == IPV6_RETRY_MAX) {
			unsigned char ipv6_addr[IPV6_ADDR_LEN] = { 0 };
			mlps_get_current_ipv6_addr(ipv6_addr, false);
		}
		break;
	}
}

static void mlps_parse_pushproxy_module(pkt_mlps_detail_data_t *data)
{
	(void)mlps_parse_pushproxy_module_cmd((pkt_mlps_cmd_data_t *)data->value);
}

static void mlps_parse_app(pkt_mlps_detail_data_t *data)
{
	switch (data->mlps_app_type) {
#ifdef CONFIG_MLPS_OPERATE_CONN
	case APP_SOFTBUS:
		mlps_parse_softbus_module(data);
		break;
#endif

	case APP_PUSHPROXY:
		mlps_parse_pushproxy_module(data);
		break;

	default:
		hwlog_warn("%s unkonwn mlps app %d\n", __func__, data->mlps_app_type);
		break;
	}
}

static void mlps_parse_data(pkt_mlps_detail_data_t *data)
{
	if (data->mlps_app_type <= APP_START || data->mlps_app_type >= APP_MAX) {
		hwlog_err("%s: no support app : %d\n", __func__, data->mlps_app_type);
		return;
	}

	if (data->mlps_cmd < MLPS_CMD_OPEN || data->mlps_cmd >= MLPS_CMD_UPLOAD) {
		hwlog_err("%s: no support cmd : %d\n", __func__, data->mlps_cmd);
		return;
	}

	mlps_parse_app(data);
}

static ssize_t upload_mlps_data(char *data, ssize_t len)
{
	return inputhub_route_write(ROUTE_MLPS_PORT, data, len);
}

static int send_mlps_cmd_internal(int tag, enum obj_cmd cmd,
	enum obj_sub_cmd subcmd, bool use_lock)
{
	interval_param_t interval_param;

	(void)memset_s(&interval_param, sizeof(interval_param), 0,
		sizeof(interval_param));
	if (cmd == CMD_CMN_OPEN_REQ) {
		if (really_do_enable_disable(&g_mlps_ref_cnt, true, 0)) {
			if (use_lock) {
				inputhub_sensor_enable(tag, true, HOST_DEFAULT);
				inputhub_sensor_setdelay(tag, &interval_param, HOST_DEFAULT);
			} else {
				inputhub_sensor_enable_nolock(tag, true, HOST_DEFAULT);
				inputhub_sensor_setdelay_nolock(tag,
					&interval_param, HOST_DEFAULT);
			}
		}
		hwlog_info("send_mlps_cmd open cmd:%d, mlps_ref_cnt= %x\n", cmd, g_mlps_ref_cnt);
	} else if (cmd == CMD_CMN_CLOSE_REQ) {
		if (really_do_enable_disable(&g_mlps_ref_cnt, false, 0)) {
			if (use_lock)
				inputhub_sensor_enable(tag, false, HOST_DEFAULT);
			else
				inputhub_sensor_enable_nolock(tag, false, HOST_DEFAULT);
		}
		hwlog_info("send_mlps_cmd close cmd:%d, mlps_ref_cnt= %x\n", cmd, g_mlps_ref_cnt);
	}
	return 0;
}

static int send_mlps_cmd(enum obj_cmd cmd, unsigned long arg, ssize_t len)
{
	void __user *argp = (void __user *)(uintptr_t)arg;
	int argvalue = 0;
	int i;
	int loop_num;

	loop_num = sizeof(mlps_cmd_map_tab) /
		sizeof(mlps_cmd_map_tab[0]);
	for (i = 0; i < loop_num; i++) {
		if (mlps_cmd_map_tab[i].mlps_ioctl_app_cmd == cmd)
			break;
	}

	if (i == loop_num) {
		hwlog_err("%s unknown cmd %d in mlps\n", __func__, cmd);
		return -EFAULT;
	}

	if (copy_from_user(&argvalue, argp, sizeof(argvalue)))
		hwlog_err("%s copy_from_user fail or arg is NULL\n", __func__);

	return send_mlps_cmd_internal(mlps_cmd_map_tab[i].tag,
		mlps_cmd_map_tab[i].cmd, argvalue, true);
}

static int send_mlps_cmd_ext(void *data, ssize_t len,
	struct read_info *pkg_mcu, bool use_lock)
{
	struct write_info pkg_ap;
	int ret = 0;

	if (!(data) || (len <= 0)) {
		hwlog_err("%s para failed\n", __func__);
		return -1;
	}

	if (memset_s(&pkg_ap, sizeof(pkg_ap), 0, sizeof(pkg_ap))) {
		hwlog_err("%s memset data failed\n", __func__);
		return -1;
	}

	pkg_ap.cmd = CMD_CMN_CONFIG_REQ;
	pkg_ap.tag = TAG_MLPS;
	pkg_ap.wr_buf = data;
	pkg_ap.wr_len = SUBCMD_LEN + len;

	ret = write_customize_cmd(&pkg_ap, NULL, use_lock);
	if (ret < 0) {
		hwlog_err("%s: err. write cmd\n", __func__);
		return -1;
	}
	return ret;
}

static void send_raw_sock_errno_to_mcu(int errno)
{
	// four data is type errno rsv rsv
	int ep[DATA_LEN_4] = { SEND_RAW_SOCK_ERR, errno, 0, 0 };
	char *data = (char *)kzalloc(DATA_LEN_16 + sizeof(ep) + SUBCMD_LEN, GFP_KERNEL);
	if (data == NULL) {
		hwlog_err("%s data kzalloc fail\n", __func__);
		return;
	}

	pkt_mlps_sub_header_t *temp = (pkt_mlps_sub_header_t *)data;
	temp->subcmd = SUB_CMD_MLPS_CONFIG_REQ;
	pkt_mlps_detail_data_t *tmp = (pkt_mlps_detail_data_t *)temp->value;
	tmp->mlps_app_type = APP_PUSHPROXY;
	tmp->app_len = DATA_LEN_12 + sizeof(ep);
	tmp->mlps_module_type = 0;
	tmp->module_len = DATA_LEN_8 + sizeof(ep);
	tmp->mlps_cmd = MLPS_CMD_CONFIG;
	tmp->cmd_len = DATA_LEN_4 + sizeof(ep);
	pkt_mlps_cmd_data_t *value = (pkt_mlps_cmd_data_t *)tmp->value;
	value->cmd = PUSH_MLPS_ERR;
	value->cmd_len = sizeof(ep);
	if (memcpy_s(value->value, DATA_LEN_16, ep, sizeof(ep)) != EOK) {
		hwlog_err("%s memcpy errno fail\n", __func__);
		return;
	}

	hwlog_info("%s send errno %d\n", __func__, errno);
	int ret = send_mlps_cmd_ext(data, DATA_LEN_16 + sizeof(ep), NULL, true);

	if (data != NULL) {
		kfree(data);
		data = NULL;
	}
}

static void mlps_ipv6_msg_update(struct msghdr *msg,
	push_wf_send_data_t *pg, struct sockaddr_in6 *s_addr)
{
	s_addr->sin6_port = 0;
	s_addr->sin6_family = AF_INET6;
	if (memcpy_s(&s_addr->sin6_addr, IPV6_ADDR_LEN, pg->addr, IPV6_ADDR_LEN) != EOK) {
		hwlog_err("%s v6 memcpy_s fail\n", __func__);
		return;
	}

	msg->msg_flags |= MSG_DONTWAIT;
	msg->msg_control = NULL;
	msg->msg_controllen = 0;
	msg->msg_name = (struct sockaddr *)s_addr;
	msg->msg_namelen = sizeof(struct sockaddr_in6);
}

static void mlps_ipv4_msg_update(struct msghdr *msg,
	push_wf_send_data_t *pg, struct sockaddr_in *s_addr)
{
	s_addr->sin_port = pg->port;
	s_addr->sin_family = AF_INET;
	if (memcpy_s(&s_addr->sin_addr, IPV4_ADDR_LEN, pg->addr, IPV4_ADDR_LEN) != EOK) {
		hwlog_err("%s v4 memcpy_s fail\n", __func__);
		return;
	}

	msg->msg_flags |= MSG_DONTWAIT;
	msg->msg_control = NULL;
	msg->msg_controllen = 0;
	msg->msg_name = (struct sockaddr *)s_addr;
	msg->msg_namelen = sizeof(struct sockaddr);
}

static void mlps_ip_raw_socket(push_wf_send_data_t *pg)
{
	struct socket *sock = NULL;
	int err;
	struct msghdr msg;
	struct kvec iov[1];
	struct sockaddr_in s_addr;
	struct sockaddr_in6 s_addr6;

	(void)memset_s(&msg, sizeof(msg), 0, sizeof(msg));
	(void)memset_s(&iov, sizeof(iov), 0, sizeof(iov));
	if (pg->addr_len == IPV4_ADDR_LEN) {
		mlps_ipv4_msg_update(&msg, pg, &s_addr);
		err = sock_create_kern(&init_net, AF_INET, SOCK_RAW, IPPROTO_TCP, &sock);
	} else if (pg->addr_len == IPV6_ADDR_LEN) {
		mlps_ipv6_msg_update(&msg, pg, &s_addr6);
		err = sock_create_kern(&init_net, AF_INET6, SOCK_RAW, IPPROTO_RAW, &sock);
	} else {
		hwlog_err("%s is not ipv4 or v6\n", __func__);
		return;
	}
	if (err < 0) {
		hwlog_err("%s create fail\n", __func__);
		return;
	}

	int on = 1;
	if (pg->addr_len == IPV4_ADDR_LEN) {
		err = sock->ops->setsockopt(sock, IPPROTO_IP, IP_HDRINCL, KERNEL_SOCKPTR(&on), sizeof(on));
	} else if (pg->addr_len == IPV6_ADDR_LEN) {
		err = sock->ops->setsockopt(sock, IPPROTO_IPV6, IPV6_HDRINCL, KERNEL_SOCKPTR(&on), sizeof(on));
	} else {
		hwlog_info("%s unknown set sock\n", __func__);
	}
	if (err < 0) {
		hwlog_err("%s kernel_setsockopt fail : %d\n", __func__, err);
		sock_release(sock);
		return;
	}

	iov[0].iov_base = pg->data;
	iov[0].iov_len  = pg->data_len;
	err = kernel_sendmsg(sock, &msg, iov, 1, pg->data_len);
	if (err != (signed)pg->data_len) {
		hwlog_err("%s kernel_sendmsg failed, ret=%d", __func__, err);
		sock_release(sock);
		send_raw_sock_errno_to_mcu(err);
		return;
	}
	sock_release(sock);
}


static void mlps_wifi_send_handle(struct work_struct *work)
{
	struct mlps_wifi_send_work *p = container_of(work, struct mlps_wifi_send_work, worker);
	mlps_ip_raw_socket(&p->pg);
	kfree(p);
	return;
}

static void mlps_wifi_handle(struct work_struct *work)
{
	struct mlps_wifi_work *p = container_of(work, struct mlps_wifi_work, worker);

	int ret = send_mlps_cmd_ext(p->data, p->len, NULL, true);
	if (ret != 0)
		hwlog_err("%s send_mlps_cmd_ext work error\n", __func__);

	if (p->data != NULL) {
		kfree(p->data);
		p->data = NULL;
	}
	kfree(p);
	return;
}

static int mlps_match_ip_hook_data(struct sk_buff *skb, unsigned short link_id)
{
	struct ethhdr *eth_header = (struct ethhdr *)(skb_mac_header(skb));
	char *data = (char *)kzalloc(skb->len + MAC_HDR_LEN + DATA_LEN_16 + SUBCMD_LEN, GFP_ATOMIC);
	if (data == NULL) {
		hwlog_err("%s ip pg kzalloc fail!!!\n", __func__);
		return FAIL;
	}
	pkt_wifi_detail_data_t *wf_data = (pkt_wifi_detail_data_t *)data;
	(void)memset_s(data, (skb->len + MAC_HDR_LEN + DATA_LEN_16 + SUBCMD_LEN),
		0, (skb->len + MAC_HDR_LEN + DATA_LEN_16 + SUBCMD_LEN));
	if (memcpy_s(wf_data->value, MAC_HDR_LEN, eth_header, MAC_HDR_LEN) != EOK) {
		hwlog_err("%s memcpy_s fail\n", __func__);
		return FAIL;
	}
	int ret = skb_copy_bits(skb, 0, wf_data->value + MAC_HDR_LEN, skb->len);
	if (ret != 0) {
		hwlog_info("%s copy error!\n", __func__);
		kfree(data);
		data = NULL;
		return FAIL;
	}
	wf_data->subcmd = SUB_CMD_MLPS_CONFIG_REQ;
	wf_data->mlps_app_type = APP_PUSHPROXY;
	wf_data->app_len = skb->len + MAC_HDR_LEN + DATA_LEN_12;
	wf_data->mlps_module_type = 0;
	wf_data->module_len = skb->len + MAC_HDR_LEN + DATA_LEN_8;
	wf_data->config_type = MLPS_CMD_CONFIG;
	wf_data->config_len = skb->len + MAC_HDR_LEN + DATA_LEN_4;
	wf_data->cmd = link_id;
	wf_data->cmd_len = skb->len + MAC_HDR_LEN;
	struct mlps_wifi_work  *wifi_work = NULL;
	wifi_work = kzalloc(sizeof(struct mlps_wifi_work), GFP_ATOMIC);
	if (ZERO_OR_NULL_PTR((unsigned long)(uintptr_t)wifi_work)) {
		pr_err("[%s] alloc wifi_work failed.\n", __func__);
		kfree(data);
		data = NULL;
		return FAIL;
	}
	wifi_work->data = wf_data;
	wifi_work->len = skb->len + MAC_HDR_LEN + DATA_LEN_16;
	INIT_WORK(&wifi_work->worker, mlps_wifi_handle);
	queue_work(g_mlps_wifi_wq, &wifi_work->worker);
	return SUCCESS;
}

static int mlps_ipv6_hook_in_check_data(struct sk_buff *skb)
{
	struct ethhdr *eth_header;
	struct ipv6hdr *ip_header;
	struct tcphdr *tcp_header;

	eth_header = (struct ethhdr *)(skb_mac_header(skb));
	ip_header = (struct ipv6hdr *)(ipv6_hdr(skb));
	tcp_header = (struct tcphdr *)(tcp_hdr(skb));
	if (eth_header == NULL || ip_header == NULL || tcp_header == NULL) {
		return FAIL;
	}
	for (int i = 0; i < PUSH_LINK_MAX - PUSH_LINK_1; i++) {
		if (g_v6_f[i].flag != 1) {
			return FAIL;
		}
		if (g_v6_f[i].port == ntohs(tcp_header->dest)) {
			if (memcmp(&(ip_header->saddr), g_v6_f[i].addr, IPV6_ADDR_LEN) == 0) {
				hwlog_info("%s ipv6 filter match!!!\n", __func__);
				return mlps_match_ip_hook_data(skb, g_v6_f[i].link_id);
			}
		}
	}

	return FAIL;
}

static unsigned int mlps_ipv6_hook_in(void *priv_data, struct sk_buff *skb, const struct nf_hook_state *state)
{
	if (g_push_gnss_status != ST_PUSH_OPEN_SUC) {
		return NF_ACCEPT;
	}

	if (!skb || !skb->len || !state) {
		return NF_ACCEPT;
	}

	if (ipv6_hdr(skb)->nexthdr != IPPROTO_TCP) {
		return NF_ACCEPT;
	}

	spin_lock_bh(&link_hook_lock);
	if (mlps_ipv6_hook_in_check_data(skb) == FAIL) {
		spin_unlock_bh(&link_hook_lock);
		return NF_ACCEPT;
	}
	spin_unlock_bh(&link_hook_lock);

	return NF_DROP;
}

static int mlps_ipv4_hook_in_check_data(struct sk_buff *skb)
{
	struct ethhdr *eth_header;
	struct iphdr *ip_header;
	struct tcphdr *tcp_header;

	eth_header = (struct ethhdr *)(skb_mac_header(skb));
	ip_header = (struct iphdr *)(skb_network_header(skb));
	tcp_header = (struct tcphdr *)(skb_transport_header(skb));
	if (eth_header == NULL || ip_header == NULL || tcp_header == NULL) {
		return FAIL;
	}

	for (int i = 0; i < PUSH_LINK_MAX - PUSH_LINK_1; i++) {
		if (g_v4_f[i].flag != 1) {
			return FAIL;
		}
		if (g_v4_f[i].port == ntohs(tcp_header->dest)) {
			if (memcmp(&(ip_header->saddr), g_v4_f[i].addr, IPV4_ADDR_LEN) == 0) {
				hwlog_info("%s ipv4 filter match!!!\n", __func__);
				return mlps_match_ip_hook_data(skb, g_v4_f[i].link_id);
			}
		}
	}

	return FAIL;
}

static unsigned int mlps_ipv4_hook_in(void *priv_data, struct sk_buff *skb, const struct nf_hook_state *state)
{
	if (g_push_gnss_status != ST_PUSH_OPEN_SUC) {
		return NF_ACCEPT;
	}

	if (!skb || !skb->len || !state) {
		return NF_ACCEPT;
	}

	if (ip_hdr(skb)->protocol != IPPROTO_TCP) {
		return NF_ACCEPT;
	}

	spin_lock_bh(&link_hook_lock);
	if (mlps_ipv4_hook_in_check_data(skb) == FAIL) {
		spin_unlock_bh(&link_hook_lock);
		return NF_ACCEPT;
	}
	spin_unlock_bh(&link_hook_lock);

	return NF_DROP;
}


#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 1)
static const struct nf_hook_ops mlps_hooks_ipv4[] = {
#else
static struct nf_hook_ops mlps_hooks_ipv4[] = {
#endif
	{
		.hook = mlps_ipv4_hook_in,
		.pf = PF_INET,
		.hooknum = NF_INET_LOCAL_IN,
		.priority = NF_IP_PRI_FILTER + 1,
	},
};

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 1)
static const struct nf_hook_ops mlps_hooks_ipv6[] = {
#else
static struct nf_hook_ops mlps_hooks_ipv6[] = {
#endif
	{
		.hook = mlps_ipv6_hook_in,
		.pf = PF_INET6,
		.hooknum = NF_INET_LOCAL_IN,
		.priority = NF_IP_PRI_FILTER + 1,
	},
};

static void mlps_close_push_link_hooks(void)
{
	if (g_push_link_status[0] == 1) {
		hwlog_info("%s enter v4\n", __func__);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 1)
		nf_unregister_net_hooks(&init_net, mlps_hooks_ipv4, ARRAY_SIZE(mlps_hooks_ipv4));
#else
		nf_unregister_hooks(mlps_hooks_ipv4, ARRAY_SIZE(mlps_hooks_ipv4));
#endif
		g_push_link_status[0] = 0;
	}
	if (g_push_link_status[1] == 1) {
		hwlog_info("%s enter v6\n", __func__);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 1)
		nf_unregister_net_hooks(&init_net, mlps_hooks_ipv6, ARRAY_SIZE(mlps_hooks_ipv6));
#else
		nf_unregister_hooks(mlps_hooks_ipv6, ARRAY_SIZE(mlps_hooks_ipv6));
#endif
		g_push_link_status[1] = 0;
	}
}

static void mlps_start_push_link_v6_hooks(void)
{
	int ret = 0;
	if (g_push_link_status[1] == 0) {
		hwlog_info("%s enter\n", __func__);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 1)
		ret = nf_register_net_hooks(&init_net, mlps_hooks_ipv6, ARRAY_SIZE(mlps_hooks_ipv6));
#else
		ret = nf_register_hooks(mlps_hooks_ipv6, ARRAY_SIZE(mlps_hooks_ipv6));
#endif
	}
	if (ret < 0) {
		hwlog_err("%s fail\n", __func__);
		return;
	}
	g_push_link_status[1] = 1;
}

static void mlps_start_push_link_v4_hooks(void)
{
	int ret = 0;
	if (g_push_link_status[0] == 0) {
		hwlog_info("%s enter\n", __func__);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 1)
		ret = nf_register_net_hooks(&init_net, mlps_hooks_ipv4, ARRAY_SIZE(mlps_hooks_ipv4));
#else
		ret = nf_register_hooks(mlps_hooks_ipv4, ARRAY_SIZE(mlps_hooks_ipv4));
#endif
	}
	if (ret < 0) {
		hwlog_err("%s fail\n", __func__);
		return;
	}
	g_push_link_status[0] = 1;
}

static void mlps_push_close_ops(void)
{
	g_get_ipv6_retry_cnt = 0;
	mlps_close_push_link_hooks();
	mlps_operate_gnss_handle(MLPS_CMD_CLOSE, PUSHPROXY_WIFI_HEARTBEAT_AGENT);
	spin_lock_bh(&link_hook_lock);
	for (int i = 0; i < PUSH_LINK_MAX - PUSH_LINK_1; i++) {
		g_v4_f[i].flag = 0;
		g_v6_f[i].flag = 0;
	}
	spin_unlock_bh(&link_hook_lock);
}

static void mlps_push_send_raw(pkt_mlps_cmd_data_t *raw_data)
{
	if (raw_data->cmd_len > 0) {
		push_wf_send_data_t *pg = (push_wf_send_data_t *)raw_data->value;

		struct mlps_wifi_send_work *ws_work = NULL;
		ws_work = kzalloc(sizeof(struct mlps_wifi_send_work) + pg->data_len, GFP_ATOMIC);
		if (ZERO_OR_NULL_PTR((unsigned long)(uintptr_t)ws_work)) {
			pr_err("[%s] alloc ws_work failed.\n", __func__);
			return;
		}
		if (memcpy_s(&ws_work->pg, raw_data->cmd_len, raw_data->value, raw_data->cmd_len) != EOK) {
			hwlog_err("%s memcpy_s fail\n", __func__);
			return;
		}
		INIT_WORK(&ws_work->worker, mlps_wifi_send_handle);
		queue_work(g_mlps_wifi_send_wq, &ws_work->worker);
	}
}

static void mlps_push_update_v4_filter(uint8_t i, push_wf_hook_data_t *hook_filter)
{
	if (hook_filter->flag == 1) { // open
		hwlog_info("%s ipv4 flag 1\n", __func__);
		g_v4_f[i].port = hook_filter->port;
		if (memcpy_s(g_v4_f[i].addr, IPV4_ADDR_LEN, hook_filter->addr, IPV4_ADDR_LEN) != EOK) {
			hwlog_err("%s memcpy_s fail\n", __func__);
			return;
		}
		g_v4_f[i].flag = 1;
		g_v6_f[i].flag = 0;
	} else if (hook_filter->flag == 0) {
		hwlog_info("%s ipv4 flag 0\n", __func__);
		g_v4_f[i].flag = 0;
		g_v6_f[i].flag = 0;
	} else {
		hwlog_err("%s no suport push v4 hook flag %d\n", __func__, hook_filter->flag);
	}
}

static void mlps_push_update_v6_filter(uint8_t i, push_wf_hook_data_t *hook_filter)
{
	if (hook_filter->flag == 1) { // open
		hwlog_info("%s ipv6 flag 1\n", __func__);
		g_v6_f[i].port = hook_filter->port;
		if (memcpy_s(g_v6_f[i].addr, IPV6_ADDR_LEN, hook_filter->addr, IPV6_ADDR_LEN) != EOK) {
			hwlog_err("%s memcpy_s fail\n", __func__);
			return;
		}
		g_v4_f[i].flag = 0;
		g_v6_f[i].flag = 1;
	} else if (hook_filter->flag == 0) {
		hwlog_info("%s ipv6 flag 0\n", __func__);
		g_v4_f[i].flag = 0;
		g_v6_f[i].flag = 0;
	} else {
		hwlog_err("%s no suport push v6 hook flag %d\n", __func__, hook_filter->flag);
	}
}

static void mlps_push_update_filter(uint8_t i, pkt_mlps_cmd_data_t *temp)
{
	push_wf_hook_data_t *hook_filter = (push_wf_hook_data_t *)temp->value;

	spin_lock_bh(&link_hook_lock);
	if (temp->value[0] == IPV4_ADDR_LEN) {
		mlps_start_push_link_v4_hooks();
		hwlog_info("%s ipv4 enter in\n", __func__);
		mlps_push_update_v4_filter(i, hook_filter);
	} else if (temp->value[0] == IPV6_ADDR_LEN) {
		mlps_start_push_link_v6_hooks();
		hwlog_info("%s ipv6 enter in\n", __func__);
		mlps_push_update_v6_filter(i, hook_filter);
	} else {
		hwlog_err("%s no suport push hook ip addr len = %d\n", __func__, temp->value[0]);
	}
	spin_unlock_bh(&link_hook_lock);
}

static int mlps_parse_push_up_module(pkt_mlps_detail_data_t *data)
{
	pkt_mlps_cmd_data_t *temp = (pkt_mlps_cmd_data_t *)data->value;
	int ret = NORMAL_PROCESS;
	hwlog_info("%s cmd 0x%x\n", __func__, temp->cmd);

	switch (temp->cmd) {
	case PUSH_STOP_CNF:
		g_push_gnss_status = ST_PUSH_DEFAULT;
		break;

	case PUSH_CLOSE_SUC:
		ret = SKIP_PROCESS;
		mlps_push_close_ops();
		break;

	case PUSH_SEND_RAW:
		ret = SKIP_PROCESS;
		mlps_push_send_raw(temp);
		break;

	case PUSH_LINK_1:
	case PUSH_LINK_2:
		if (temp->cmd_len > 0) {
			mlps_push_update_filter(temp->cmd - PUSH_LINK_1, temp);
		}
		ret = SKIP_PROCESS;
		break;

	default:
		hwlog_err("%s no suport push cmd = 0x%x\n", __func__, temp->cmd);
		break;
	}

	return ret;
}

static int mlps_parse_upload_app(pkt_mlps_detail_data_t *data)
{
	int ret = NORMAL_PROCESS;

	switch (data->mlps_app_type) {
	case APP_PUSHPROXY:
		ret = mlps_parse_push_up_module(data);
		break;

	default:
		break;
	}

	return ret;
}

static int process_mlps_data_from_mcu(const struct pkt_header *head)
{
	int ret = NORMAL_PROCESS;
	unsigned char *data = (unsigned char *)head + sizeof(struct pkt_header);
	ret = mlps_parse_upload_app((pkt_mlps_detail_data_t *)data);
	if (ret == NORMAL_PROCESS) {
		__pm_wakeup_event(mlps_wl, MLPS_WAKEUP_MSEC); // wait for mlps service read 50ms
		upload_mlps_data(data, head->length);
		hwlog_info("%s: upload mlps data\n", __func__);
	} else {
		hwlog_info("%s: no need upload mlps data\n", __func__);
	}
	return 0;
}

/* read /dev/mlpshub */
static ssize_t mlps_read(struct file *file, char __user *data, size_t count,
			loff_t *pos)
{
	return inputhub_route_read(ROUTE_MLPS_PORT, data, count);
}

static int check_mlps_privacy_mode(pkt_mlps_detail_data_t *data)
{
	int ret = 0;

	if (data->app_len != NORMAL_LEN || data->module_len != MLPS_RECOVERY_LEN)
		return ret;

	if (data->mlps_cmd == MLPS_CMD_OPEN) {
		hwlog_info("%s privacy mode in\n", __func__);
		g_privacy_mode = true;
		if (mlps_check_bit_map() == false) {
			hwlog_info("%s check bit map suc\n", __func__);
			mlps_privacy_notify_ta();
			return ret;
		}
		hwlog_info("%s check bit map fail : 0x%x\n", __func__, g_mlps_app_bit_map);
		for (int i = OUTSIDE_APP_START; i < MODULE_END; i++) {
			if (bit_check(&g_mlps_app_bit_map, i))
				(void)mlps_operate_gnss_power(MLPS_CMD_CLOSE, i);
		}
		g_privacy_timer_retry = 0;
		mod_timer(&privacy_gnss_timer, jiffies + 1 * HZ);
	} else if (data->mlps_cmd == MLPS_CMD_CLOSE) {
		hwlog_info("%s privacy mode out\n", __func__);
		g_privacy_timer_retry = 0;
		g_privacy_mode = false;
		for (int i = OUTSIDE_APP_START; i < MODULE_END; i++) {
			if (bit_check(&g_outside_app_bit_map, i))
				(void)mlps_operate_gnss_power(MLPS_CMD_OPEN, i);
		}
	}

	return ret;
}

static int check_mlps_palace_museum_mode(pkt_mlps_detail_data_t *data)
{
	int ret = 0;

	if (data->app_len != NORMAL_LEN)
		return ret;

	if (data->mlps_cmd == MLPS_CMD_OPEN) {
		hwlog_info("%s in\n", __func__);
		g_palace_mode = true;
		if (mlps_check_bit_map() == false) {
			hwlog_info("%s check bit map suc\n", __func__);
			unsigned short mlps_upload_data[DATA_LEN_4] = {APP_PALACE_MUSEUM, MLPS_RECOVERY_LEN, 0, 0};
			upload_mlps_data((char *)mlps_upload_data, MLPS_RECOVERY_DATA_LEN);
			return ret;
		}
		hwlog_info("%s check bit map fail : 0x%x\n", __func__, g_mlps_app_bit_map);
		for (int i = OUTSIDE_APP_START; i < MODULE_END; i++) {
			if (bit_check(&g_mlps_app_bit_map, i))
				(void)mlps_operate_gnss_power(MLPS_CMD_CLOSE, i);
		}
		g_palace_timer_retry = 0;
		mod_timer(&palace_museum_gnss_timer, jiffies + 1 * HZ);
	} else if (data->mlps_cmd == MLPS_CMD_CLOSE) {
		hwlog_info("%s out\n", __func__);
		g_palace_timer_retry = 0;
		g_palace_mode = false;
		for (int i = OUTSIDE_APP_START; i < MODULE_END; i++) {
			if (bit_check(&g_outside_app_bit_map, i))
				(void)mlps_operate_gnss_power(MLPS_CMD_OPEN, i);
		}
	}

	return ret;
}

static int check_mlps_data(pkt_mlps_detail_data_t *data)
{
	if (data->mlps_app_type < APP_PALACE_MUSEUM || data->mlps_app_type >= MLPS_SENSORHUB_RECOVERY) {
		hwlog_info("%s: app %d no need check\n", __func__, data->mlps_app_type);
		return -1;
	}

	if (data->mlps_cmd < MLPS_CMD_OPEN || data->mlps_cmd >= MLPS_CMD_UPLOAD) {
		hwlog_err("%s: cmd fail\n", __func__);
		return -1;
	}

	int ret = 0;
	switch (data->mlps_app_type) {
	case APP_PALACE_MUSEUM:
		ret = check_mlps_palace_museum_mode(data);
		break;

	case APP_PRIVACY:
		ret = check_mlps_privacy_mode(data);
		break;

	default:
		hwlog_info("%s app %d no need check\n", __func__, data->mlps_app_type);
		break;
	}

	return ret;
}

/* write to /dev/motionhub, when need tansfer big data ,use this channel */
static ssize_t mlps_write(struct file *file, const char __user *user_data,
			 size_t len, loff_t *ppos)
{
	if (!(user_data) || (len < MIN_LEN) || (len > MAX_LEN)) {
		hwlog_err("%s para failed\n", __func__);
		return len;
	}

	char *data = (char *)kzalloc(len + SUBCMD_LEN, GFP_KERNEL);
	if (data == NULL) {
		hwlog_err("%s user data kzalloc fail\n", __func__);
		return len;
	}

	pkt_mlps_sub_header_t *temp = (pkt_mlps_sub_header_t *)data;
	temp->subcmd = SUB_CMD_MLPS_CONFIG_REQ;
	if (copy_from_user(temp->value, user_data, len)) {
		hwlog_err("%s copy from user fail\n", __func__);
		if (data != NULL) {
			kfree(data);
		}
		return len;
	}

	int ret = check_mlps_data((pkt_mlps_detail_data_t *)temp->value);
	if (ret != 0) {
		mlps_parse_data((pkt_mlps_detail_data_t *)temp->value);
		ret = send_mlps_cmd_ext(data, len, NULL, true);
		if (ret != 0)
			hwlog_err("mlps_write work error\n");
	}

	if (data != NULL) {
		kfree(data);
		data = NULL;
	}

	return len;
}

static void privacy_or_palace_ops_gnss(void)
{
	for (int i = INSIDE_APP_START; i < MODULE_END; i++) {
		if (bit_check(&g_mlps_app_bit_map, i))
			(void)mlps_operate_gnss_power(MLPS_CMD_CLOSE, i);
	}
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(4,19,0)
static void privacy_gnss_timeout(unsigned long data)
#else
static void privacy_gnss_timeout(struct timer_list *data)
#endif
{
	hwlog_info("%s enter in\n", __func__);
	if (g_privacy_mode == false)
		return;

	if (mlps_check_bit_map() == true) {
		if (g_privacy_timer_retry == 0) {
			g_privacy_timer_retry++;
			privacy_or_palace_ops_gnss();
			mod_timer(&privacy_gnss_timer, jiffies + 1 * HZ);
		} else {
			g_privacy_timer_retry = 0;
		}
		hwlog_info("%s mlps_check_bit_map fail 0x%x\n", __func__, g_mlps_app_bit_map);
		return;
	}

	g_privacy_timer_retry = 0;
	mlps_privacy_notify_ta();

	return;
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(4,19,0)
static void palace_museum_gnss_timeout(unsigned long data)
#else
static void palace_museum_gnss_timeout(struct timer_list *data)
#endif
{
	hwlog_info("%s enter in\n", __func__);
	if (g_palace_mode == false)
		return;

	if (mlps_check_bit_map() == true) {
		if (g_palace_timer_retry == 0) {
			g_palace_timer_retry++;
			privacy_or_palace_ops_gnss();
			mod_timer(&palace_museum_gnss_timer, jiffies + msecs_to_jiffies(500));
		} else {
			g_palace_timer_retry = 0;
		}
		hwlog_info("%s mlps_check_bit_map fail 0x%x\n", __func__, g_mlps_app_bit_map);
		return;
	}

	g_palace_timer_retry = 0;
	unsigned short mlps_upload_data[DATA_LEN_4] = {APP_PALACE_MUSEUM, MLPS_RECOVERY_LEN, 0, 0};
	upload_mlps_data((char *)mlps_upload_data, MLPS_RECOVERY_DATA_LEN);

	return;
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(4,19,0)
static void get_ipv6_gnss_timeout(unsigned long data)
#else
static void get_ipv6_gnss_timeout(struct timer_list *data)
#endif
{
	hwlog_info("%s is enter\n", __func__);

	if (g_push_gnss_status != ST_PUSH_OPEN_SUC) {
		hwlog_warn("%s g_push_gnss_status is not open\n", __func__);
		return;
	}

	if (g_get_ipv6_retry_cnt >=  IPV6_RETRY_MAX) {
		return;
	} else {
		g_get_ipv6_retry_cnt++;
	}

	hwlog_info("%s retry_cnt = %d\n", __func__, g_get_ipv6_retry_cnt);
	unsigned char ipv6_addr[IPV6_ADDR_LEN] = { 0 };
	mlps_get_current_ipv6_addr(ipv6_addr, false);
	return;
}

static void notify_ap_mlps_recovery(void)
{
	g_push_gnss_status = ST_PUSH_CLOSE;
	char mlps_recovery_data[MLPS_RECOVERY_DATA_LEN] = {MLPS_RECOVERY_HIGH, MLPS_RECOVERY_LOW,
		MLPS_RECOVERY_LEN, 0, 0, 0, 0, 0};
	upload_mlps_data((char *)mlps_recovery_data, MLPS_RECOVERY_DATA_LEN);
}

static int mlps_ioc_get_app_version(void __user *argp)
{
	int rc = 0;
	app_version_t temp;

	if (copy_from_user(&temp, argp, sizeof(app_version_t)))
		hwlog_err("%s copy_from_user fail or arg is NULL\n", __func__);

	switch (temp.app_type) {
	case APP_SOFTBUS:
		if (copy_to_user(argp, &g_softbus_version, sizeof(softbus_version_t)))
			rc = -EFAULT;
		break;

	case APP_PUSHPROXY:
		if (copy_to_user(argp, &g_push_version, sizeof(push_version_t)))
			rc = -EFAULT;
		break;

	default:
		rc = -EFAULT;
		break;
	}

	return rc;
}

/*
 * Description: ioctl function to /dev/mlpshub, do open, do config/debug
 *              cmd to mlpshub
 * Return:      result of ioctrl command, 0 successed, -ENOTTY failed
 */
static long mlps_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	void __user *argp = (void __user *)(uintptr_t)arg;

	switch (cmd) {
	case MLPS_IOCTL_MLPS_START:
		hwlog_info("mlps: %s  cmd: MLPS_IOCTL_MLPS_START\n", __func__);
		break;

	case MLPS_IOCTL_MLPS_STOP:
		hwlog_info("mlps: %s  cmd: MLPS_IOCTL_MLPS_STOP\n", __func__);
		break;

	case MLPS_IOCTL_MLPS_ATTR_START:
		hwlog_info("mlps: %s  cmd: MLPS_IOCTL_MLPS_ATTR_START\n", __func__);
		break;

	case MLPS_IOCTL_MLPS_ATTR_STOP:
		hwlog_info("mlps: %s  cmd: MLPS_IOCTL_MLPS_ATTR_STOP\n", __func__);
		break;

	case MLPS_IOCTL_MLPS_INTERVAL_SET:
		hwlog_info("mlps: %s  cmd: MLPS_IOCTL_MLPS_INTERVAL_SET\n", __func__);
		break;

	case MLPS_IOCTL_GET_APP_VERSION:
		hwlog_info("mlps: %s  cmd: MLPS_IOCTL_GET_APP_VERSION\n", __func__);
		return mlps_ioc_get_app_version(argp);

	default:
		hwlog_err("mlps: %s unknown cmd : %u\n", __func__, cmd);
		return -ENOTTY;
	}

	return send_mlps_cmd(cmd, arg, sizeof(arg));
}

/* open to /dev/mlpshub, do nothing now */
static int mlps_open(struct inode *inode, struct file *file)
{
	hwlog_info("mlps: enter %s\n", __func__);
	return 0;
}

/* release to /dev/mlpshub, do nothing now */
static int mlps_release(struct inode *inode, struct file *file)
{
	hwlog_info("mlps: %s ok\n", __func__);
	return 0;
}

static void enable_mlps_when_recovery_iom3(void)
{
	g_mlps_ref_cnt = 0;

	send_mlps_cmd_internal(TAG_MLPS, CMD_CMN_OPEN_REQ,
		SUB_CMD_NULL_REQ, false);
}

static int mlps_recovery_notify(struct notifier_block *nb,
	unsigned long foo, void *bar)
{
	/* prevent access the emmc now: */
	hwlog_info("%s (%lu) +\n", __func__, foo);
	switch (foo) {
	case IOM3_RECOVERY_IDLE:
		notify_ap_mlps_recovery();
		break;
	case IOM3_RECOVERY_START:
	case IOM3_RECOVERY_MINISYS:
	case IOM3_RECOVERY_3RD_DOING:
	case IOM3_RECOVERY_FAILED:
		break;
	case IOM3_RECOVERY_DOING:
		save_step_count();
		enable_mlps_when_recovery_iom3();
		break;
	default:
		hwlog_err("%s -unknow state %ld\n", __func__, foo);
		break;
	}
	hwlog_info("%s -\n", __func__);
	return 0;
}

static struct notifier_block g_mlps_recovery_notify = {
	.notifier_call = mlps_recovery_notify,
	.priority = -1,
};

/* file_operations to mlps */
static const struct file_operations mlps_fops = {
	.owner = THIS_MODULE,
	.llseek = no_llseek,
	.read = mlps_read,
	.write = mlps_write,
	.unlocked_ioctl = mlps_ioctl,
#ifdef CONFIG_COMPAT
	.compat_ioctl = mlps_ioctl,
#endif
	.open = mlps_open,
	.release = mlps_release,
};

/* miscdevice to motion */
static struct miscdevice mlpshub_miscdev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "mlpshub",
	.fops = &mlps_fops,
};

static int is_mlps_supported(void)
{
	int len = 0;
	struct device_node *mlps_node = NULL;
	const char *mlps_status = NULL;
	const char *mlps_operate_conn = NULL;
	static int ret = SUCCESS;

	mlps_node = of_find_compatible_node(NULL, NULL, MLPS_COMPATIBLE_NAME);
	if (!mlps_node) {
		hwlog_err("%s, can not find node mlps_status\n", __func__);
		return FAIL;
	}

	mlps_status = of_get_property(mlps_node, "status", &len);
	if (!mlps_status) {
		hwlog_err("%s, can't find property status\n", __func__);
		return FAIL;
	}

	if (strstr(mlps_status, "ok")) {
		hwlog_info("%s, mlps enabled!\n", __func__);
		ret = SUCCESS;
		if (of_property_read_string_index(mlps_node, "operate_conn", 0, &mlps_operate_conn) != 0)
			hwlog_err("%s, fail to read mlps operate_conn\n", __func__);
		if (strstr(mlps_operate_conn, "ok")) {
			g_support_conn_operation = true;
			hwlog_info("%s, mlps needs to operate conn!\n", __func__);
		}
		uint32_t temp = 0;
		if (of_property_read_u32(mlps_node, "push_proxy_version", &temp)) {
			hwlog_err("%s:read push_proxy_version fail\n", __func__);
		} else {
			g_push_version.push_proxy_version = (uint32_t)temp;
			hwlog_info("mlps: push_proxy_version : 0x%x\n", g_push_version.push_proxy_version);
		}
		if (of_property_read_u32(mlps_node, "push_cloud_version", &temp)) {
			hwlog_err("%s:read push_cloud_version fail\n", __func__);
		} else {
			g_push_version.push_cloud_version = (uint32_t)temp;
			hwlog_info("mlps: push_cloud_version : 0x%x\n", g_push_version.push_cloud_version);
		}
		if (of_property_read_u32(mlps_node, "softbus_version", &temp)) {
			hwlog_err("%s:read softbus_version fail\n", __func__);
		} else {
			g_softbus_version.softbus_version = (uint32_t)temp;
			hwlog_info("mlps: softbus_version : 0x%x\n", g_softbus_version.softbus_version);
		}
	} else {
		hwlog_err("%s, mlps disabled!\n", __func__);
		ret = FAIL;
	}
	return ret;
}

static int mlps_workqueue_create(void)
{
	g_mlps_gnss_wq = create_singlethread_workqueue("mlps_gnss_workqueue");
	if (g_mlps_gnss_wq == NULL) {
		hwlog_err("create_gnss_wq_singlethread_workqueue fail\n");
		return -EFAULT;
	}
	g_mlps_wifi_wq = create_singlethread_workqueue("mlps_wifi_workqueue");
	if (g_mlps_wifi_wq == NULL) {
		hwlog_err("create_wifi_wq_singlethread_workqueue fail\n");
		return -EFAULT;
	}
	g_mlps_wifi_send_wq = create_singlethread_workqueue("mlps_wifi_send_workqueue");
	if (g_mlps_wifi_send_wq == NULL) {
		hwlog_err("create_wifi_send_wq_singlethread_workqueue fail\n");
		return -EFAULT;
	}
	g_mlps_privacy_ta_wq = create_singlethread_workqueue("mlps_privacy_ta_workqueue");
	if (g_mlps_privacy_ta_wq == NULL) {
		hwlog_err("create_privacy_ta_wq_singlethread_workqueue fail\n");
		return -EFAULT;
	}

	spin_lock_init(&link_hook_lock);

	for (int i = 0; i < PUSH_LINK_MAX - PUSH_LINK_1; i++) {
		g_v4_f[i].link_id = PUSH_LINK_1 + i;
		g_v6_f[i].link_id = PUSH_LINK_1 + i;
	}

	return 0;
}
/*
* Description:	apply kernel buffer, register mlps_miscdev
* Return: 		result of function, 0 successed, else false
*/
static int __init mlpshub_init(void)
{
	int ret;

	if (is_sensorhub_disabled())
		return -1;

	ret = is_mlps_supported();
	if (ret != 0)
	{
		hwlog_info("%s, not support mlps!\n", __func__);
		return ret;
	}

	mlps_ops_conn_function_init();

	ret = mlps_workqueue_create();
	if (ret == -EFAULT) {
		hwlog_err("mlps: %s mlps_workqueue_create err=%d\n", __func__, ret);
		return ret;
	}

	ret = inputhub_route_open(ROUTE_MLPS_PORT);
	if (ret != 0) {
		hwlog_err("mlps: %s cannot open inputhub route err=%d\n",
			__func__, ret);
		return ret;
	}

	ret = misc_register(&mlpshub_miscdev);
	if (ret != 0) {
		hwlog_err("%s cannot register miscdev err=%d\n", __func__, ret);
		inputhub_route_close(ROUTE_MLPS_PORT);
		return ret;
	}
	register_mcu_event_notifier(TAG_MLPS, CMD_DATA_REQ, process_mlps_data_from_mcu);
	register_iom3_recovery_notifier(REC_USR_MLPS, &g_mlps_recovery_notify);

#if LINUX_VERSION_CODE < KERNEL_VERSION(4,19,0)
	setup_timer(&privacy_gnss_timer, privacy_gnss_timeout, 0);
	setup_timer(&palace_museum_gnss_timer, palace_museum_gnss_timeout, 0);
	setup_timer(&get_ipv6_gnss_timer, get_ipv6_gnss_timeout, 0);
#else
	timer_setup(&privacy_gnss_timer, privacy_gnss_timeout, 0);
	timer_setup(&palace_museum_gnss_timer, palace_museum_gnss_timeout, 0);
	timer_setup(&get_ipv6_gnss_timer, get_ipv6_gnss_timeout, 0);
#endif

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5, 4, 0))
	mlps_wl = wakeup_source_register(NULL, "mlps_sensorhub");
#else
	mlps_wl = wakeup_source_register("mlps_sensorhub");
#endif

	hwlog_info("%s ok\n", __func__);
	return ret;
}

/* release kernel buffer, deregister mlpshub_ud_miscdev */
static void __exit mlpshub_exit(void)
{
	inputhub_route_close(ROUTE_MLPS_PORT);
	wakeup_source_unregister(mlps_wl);
	mlps_wl = NULL;
	misc_deregister(&mlpshub_miscdev);
	hwlog_info("exit %s\n", __func__);
}

late_initcall_sync(mlpshub_init);
module_exit(mlpshub_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("MLPSHub driver");
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");
