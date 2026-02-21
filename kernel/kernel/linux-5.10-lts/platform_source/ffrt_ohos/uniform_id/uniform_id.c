#include "uniform_id.h"
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <liblinux/pal.h>
#include "sd_proxy.h"


#define drv_printk(level, module, fmt, ...) \
	printk(level " [%s, tgid:%d] [%s %d] " fmt, module, current->tgid, \
		__func__, __LINE__, ## __VA_ARGS__)
 
#define MODULE_DEVDRV   "FFRT_UID"
#define ffrt_uid_err(fmt, ...)  \
	drv_printk(KERN_ERR, MODULE_DEVDRV"] [E",  fmt, ## __VA_ARGS__)
#define ffrt_uid_warn(fmt, ...) \
	drv_printk(KERN_WARNING, MODULE_DEVDRV"] [W",  fmt, ## __VA_ARGS__)
#define ffrt_uid_info(fmt, ...)
#define ffrt_uid_debug(fmt, ...)
 
enum UDK_UID_CMD {
	UDK_SD_PROXY_MINOR_CMD_FFRT_GET_UID = 0
};
 
uint16_t ffrt_get_uniform_id(pid_t pid)
{
	int ret;
	uint16_t uid = 0;
	ffrt_uid_info("ffrt_ohos ldk call get uid.\n");
	ret = liblinux_pal_proxy_udk_invoke(PAL_PROXY_COMBINE_CMD(UDK_UNIFORM_ID_MAJOR_CMD,
		UDK_SD_PROXY_MINOR_CMD_FFRT_GET_UID), &pid, sizeof(pid_t), &uid, sizeof(uint16_t));
	if (ret != 0) {
		ffrt_uid_err("get uid proxy udk failed, ret = %d\n", ret);
		return FFRT_INVALID_UNIFORM_ID;
	}
	return uid;
}
EXPORT_SYMBOL(ffrt_get_uniform_id);