#include "hts_interface.h"

#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <liblinux/pal.h>
#include "sd_proxy.h"
#include "hts_kernel_log.h"

int hts_block_dev(uint32_t dev_id)
{
	int ret;
	ret = liblinux_pal_proxy_udk_invoke(PAL_PROXY_COMBINE_CMD(UDK_HTS_MAJOR_CMD, UDK_SD_PROXY_MINOR_CMD_FFRT_HTS_BLOCK_DEV),
		&dev_id, sizeof(uint32_t), NULL, 0);
	if (ret != 0) {
		hts_drv_err("ioctl failed, ret = %d\n", ret);
		return -1;
	}
	return 0;
}

int hts_block_event(uint16_t event)
{
	int ret;
	ret = liblinux_pal_proxy_udk_invoke(PAL_PROXY_COMBINE_CMD(UDK_HTS_MAJOR_CMD, UDK_SD_PROXY_MINOR_CMD_FFRT_HTS_BLOCK_EVENT),
		&event, sizeof(uint32_t), NULL, 0);
	if (ret != 0) {
		hts_drv_err("ioctl failed, ret = %d\n", ret);
		return -1;
	}
	return 0;
}

int hts_block_proc(struct dev_pid_info info)
{
	int ret;
	ret = liblinux_pal_proxy_udk_invoke(PAL_PROXY_COMBINE_CMD(UDK_HTS_MAJOR_CMD, UDK_SD_PROXY_MINOR_CMD_FFRT_HTS_BLOCK_PROC),
		&info, sizeof(struct dev_pid_info), NULL, 0);
	if (ret != 0) {
		hts_drv_err("ioctl failed, ret = %d\n", ret);
		return -1;
	}
	return 0;
}

#ifdef HTS_DEBUG_EN
uint16_t ffrt_get_uniform_id_test(uint32_t pid)
{
	int ret;
	uint16_t uid = 0;

	ret = liblinux_pal_proxy_udk_invoke(PAL_PROXY_COMBINE_CMD(3, 0), &pid, sizeof(uint32_t), &uid, sizeof(uint16_t));
	if (ret != 0) {
		hts_drv_err("ioctl get uid failed, ret = %d\n", ret);
		return 65535;
	}
	return uid;
}

int ip_get_hts_uid_event_test(uint8_t dev_id, int32_t pid)
{
	struct dev_event_info info = {0};
	uint16_t i;
	int ret = -1;

	info.dev_id = dev_id;
	info.uid = ffrt_get_uniform_id_test(pid);
	ret = ip_get_hts_uid_event(&info);
	for(i = 0; i < info.event_count; i++) {
		hts_drv_err("eventid=%d\n", info.events[i]);
	}
	return ret;
}

int hts_event_outbuff_enable_test(uint16_t start_event_id, uint16_t end_event_id)
{
	uint16_t event_id[100] = {0};
	uint16_t i;
	uint16_t num = 0;
	int ret = -1;

	for(i = start_event_id; i <= end_event_id; i++) {
		event_id[num++] = i;
		hts_drv_err("eventid=%d\n", i);
	}
	ret = hts_event_outbuff_enable(event_id, num);
	return ret;
}

int hts_event_outbuff_disable_test(uint16_t start_event_id, uint16_t end_event_id)
{
	uint16_t event_id[100] = {0};
	uint16_t i;
	uint16_t num = 0;
	int ret = -1;

	for(i = start_event_id; i <= end_event_id; i++) {
		event_id[num++] = i;
		hts_drv_err("eventid=%d\n", i);
	}
	ret = hts_event_outbuff_disable(event_id, num);
	return ret;
}
#endif

int ip_get_hts_uid_event(struct dev_event_info *info)
{
	int ret;
	if (info == NULL) {
		hts_drv_err("dev_event_info is NULL\n");
		return -1;
	}
	ret = liblinux_pal_proxy_udk_invoke(PAL_PROXY_COMBINE_CMD(UDK_HTS_MAJOR_CMD, UDK_SD_PROXY_MINOR_CMD_FFRT_HTS_GET_UID_EVENT),
		info, sizeof(struct dev_event_info), NULL, 0);
	if (ret != 0) {
		hts_drv_err("ioctl failed, ret = %d\n", ret);
		return -1;
	}
	return 0;
}

int hts_event_outbuff_enable(uint16_t *event_id, uint16_t event_num)
{
	int ret;
	if (event_id == NULL || event_num == 0) {
		hts_drv_err("event_id is event_num\n");
		return -1;
	}
	ret = liblinux_pal_proxy_udk_invoke(PAL_PROXY_COMBINE_CMD(UDK_HTS_MAJOR_CMD, UDK_SD_PROXY_MINOR_CMD_FFRT_HTS_OUTBUF_ENABLE),
		event_id, sizeof(uint16_t) * event_num, NULL, 0);
	if (ret != 0) {
		hts_drv_err("ioctl failed, ret = %d\n", ret);
		return -1;
	}
	return 0;
}

int hts_event_outbuff_disable(uint16_t *event_id, uint16_t event_num)
{
	int ret;
	if (event_id == NULL || event_num == 0) {
		hts_drv_err("event_id is event_num\n");
		return -1;
	}
	ret = liblinux_pal_proxy_udk_invoke(PAL_PROXY_COMBINE_CMD(UDK_HTS_MAJOR_CMD, UDK_SD_PROXY_MINOR_CMD_FFRT_HTS_OUTBUF_DISABLE),
		event_id, sizeof(uint16_t) * event_num, NULL, 0);
	if (ret != 0) {
		hts_drv_err("ioctl failed, ret = %d\n", ret);
		return -1;
	}
	return 0;
}

int hts_event_outbuff_enable_by_dev(uint32_t dev_id)
{
	int ret;
	ret = liblinux_pal_proxy_udk_invoke(PAL_PROXY_COMBINE_CMD(UDK_HTS_MAJOR_CMD, UDK_SD_PROXY_MINOR_CMD_FFRT_HTS_OUTBUF_ENABLE_BY_DEV),
		&dev_id, sizeof(uint32_t), NULL, 0);
	if (ret != 0) {
		hts_drv_err("ioctl failed, ret = %d\n", ret);
		return -1;
	}
	return 0;
}

int hts_event_outbuff_disable_by_dev(uint32_t dev_id)
{
	int ret;
	ret = liblinux_pal_proxy_udk_invoke(PAL_PROXY_COMBINE_CMD(UDK_HTS_MAJOR_CMD, UDK_SD_PROXY_MINOR_CMD_FFRT_HTS_OUTBUF_DISABLE_BY_DEV),
		&dev_id, sizeof(uint32_t), NULL, 0);
	if (ret != 0) {
		hts_drv_err("ioctl failed, ret = %d\n", ret);
		return -1;
	}
	return 0;
}

int hts_connect(uint32_t dev_id)
{
	int ret;
	ret = liblinux_pal_proxy_udk_invoke(PAL_PROXY_COMBINE_CMD(UDK_HTS_MAJOR_CMD, UDK_SD_PROXY_MINOR_CMD_FFRT_HTS_CONNECT),
		&dev_id, sizeof(uint32_t), NULL, 0);
	if (ret != 0) {
		hts_drv_err("ioctl failed, ret = %d\n", ret);
		return -1;
	}
	return 0;
}

int hts_disconnect(uint32_t dev_id)
{
	int ret;
	ret = liblinux_pal_proxy_udk_invoke(PAL_PROXY_COMBINE_CMD(UDK_HTS_MAJOR_CMD, UDK_SD_PROXY_MINOR_CMD_FFRT_HTS_DISCONNECT),
		&dev_id, sizeof(uint32_t), NULL, 0);
	if (ret != 0) {
		hts_drv_err("ioctl failed, ret = %d\n", ret);
		return -1;
	}
	return 0;
}