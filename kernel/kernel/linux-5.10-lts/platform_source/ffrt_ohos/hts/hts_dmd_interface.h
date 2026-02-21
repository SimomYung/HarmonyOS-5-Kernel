#ifndef __HTS_DMD_INTERFACE_H__
#define __HTS_DMD_INTERFACE_H__
#include <linux/types.h>

#ifdef CONFIG_HISYSEVENT
typedef struct error_no_map
{
	int32_t error_no;
	char *name;
} error_no_map;
#endif

#define DSM_HTS_BUF_SIZE (1024)
enum HTS_CMD {
	SD_PROXY_MINOR_CMD_FFRT_HTS_DMD_INBUF_OVERFLOW = 0,
	SD_PROXY_MINOR_CMD_FFRT_HTS_DMD_ONBUF_OVERFLOW = 1,
};

#endif