#ifndef __COLLEC_AUDIO_INFO__
#define __COLLEC_AUDIO_INFO__

#include <linux/mutex.h>
#include <linux/device.h>
#include "../audio_misc.h"

#define GET_INFO_IOC_MAGIC  'B'
#define SEND_UNDERPATH_W    _IOW(GET_INFO_IOC_MAGIC, 0x1a, struct underpath_info)
#define SEND_PASTATE_W      _IOW(GET_INFO_IOC_MAGIC, 0x1b, int)
#define SEND_MUTESTATE_W    _IOW(GET_INFO_IOC_MAGIC, 0x1c, int)
#define SEND_TDDSTATE_W     _IOW(GET_INFO_IOC_MAGIC, 0x1d, int)

#define MAX_UNDERPATH       48
#define MAX_MANUFACTURER    24

struct underpath_info {
    char input_path[MAX_UNDERPATH];
    char output_path[MAX_UNDERPATH];
};


struct from_cust_platform {
    struct underpath_info path;
    int pastate;
    int mutestate;
    int tddstate;
};

struct from_audio_driver {
    int pa_interrupt_times;
    int write_exception_times;
    char pa_manufacturer[MAX_MANUFACTURER];
};

struct received_info {
    struct from_cust_platform from_platform_info;
    struct from_audio_driver from_driver_info;
};

struct collec_audio_info_priv {
    struct mutex do_ioctl_lock;
    struct mutex pa_interrupt_times_lock;
    struct mutex write_exception_times_lock;
    struct mutex pa_manufacturer_lock;
    struct received_info all_received_info;
    bool pa_interrupt_times_lock_state;
    bool write_exception_times_lock_state;
    bool pa_manufacturer_lock_state;
};

#if IS_ENABLED(CONFIG_COLLEC_AUDIO_INFO)
void get_collected_audio_info(struct received_info *info);
#else
static inline void get_collected_audio_info(struct received_info *info)
{
    return;
}
#endif

void send_painterrupt_to_collec_audio_info(void);
void send_pa_manufacturer_to_collec_audio_info(const char *manufacturer);
void send_write_exception_to_collec_audio_info(void);

#ifdef CONFIG_COLLEC_AUDIO_INFO
int collec_audio_info_probe(struct device *dev, struct device_node *np);
#else
static inline int collec_audio_info_probe(struct device *dev, struct device_node *np)
{
	return 0;
}
#endif /*CONFIG_COLLEC_AUDIO_INFO*/
#endif /*__COLLEC_AUDIO_INFO__*/