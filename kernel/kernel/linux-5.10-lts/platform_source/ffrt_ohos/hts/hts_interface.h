#ifndef __HTS_INTERFACE_H__
#define __HTS_INTERFACE_H__
#include <linux/types.h>
#define HTS_INTERFACE_MAX_EVENT_BUFF             100

struct dev_event_info {
	uint8_t dev_id;
	uint16_t uid;
	uint16_t events[HTS_INTERFACE_MAX_EVENT_BUFF];
	uint16_t event_count;
};

struct dev_pid_info {
	uint32_t dev_id;
	int32_t pid;
};

enum UDK_HTS_CMD {
	UDK_SD_PROXY_MINOR_CMD_FFRT_HTS_BLOCK_DEV = 0,
	UDK_SD_PROXY_MINOR_CMD_FFRT_HTS_BLOCK_EVENT,
	UDK_SD_PROXY_MINOR_CMD_FFRT_HTS_GET_UID_EVENT,
	UDK_SD_PROXY_MINOR_CMD_FFRT_HTS_OUTBUF_ENABLE,
	UDK_SD_PROXY_MINOR_CMD_FFRT_HTS_OUTBUF_DISABLE,
	UDK_SD_PROXY_MINOR_CMD_FFRT_HTS_OUTBUF_ENABLE_BY_DEV,
	UDK_SD_PROXY_MINOR_CMD_FFRT_HTS_OUTBUF_DISABLE_BY_DEV,
	UDK_SD_PROXY_MINOR_CMD_FFRT_HTS_BLOCK_PROC,
	UDK_SD_PROXY_MINOR_CMD_FFRT_HTS_CONNECT,
	UDK_SD_PROXY_MINOR_CMD_FFRT_HTS_DISCONNECT,
};

int hts_block_dev(uint32_t dev_id);
int hts_block_event(uint16_t event);
int hts_block_proc(struct dev_pid_info info);
int ip_get_hts_uid_event(struct dev_event_info *info);
int hts_event_outbuff_enable(uint16_t *event_id, uint16_t event_num);
int hts_event_outbuff_disable(uint16_t *event_id, uint16_t event_num);
int hts_event_outbuff_enable_by_dev(uint32_t dev_id);
int hts_event_outbuff_disable_by_dev(uint32_t dev_id);
int hts_connect(uint32_t dev_id);
int hts_disconnect(uint32_t dev_id);

#endif