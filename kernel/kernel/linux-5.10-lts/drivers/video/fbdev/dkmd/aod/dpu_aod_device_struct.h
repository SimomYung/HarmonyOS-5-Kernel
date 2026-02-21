#ifndef __DPU_AOD_DEVICE_STRUCT_H_
#define __DPU_AOD_DEVICE_STRUCT_H_

#include "dpu_aod_device.h"

struct buffer_info {
	uint32_t used;
	uint32_t addr;
	uint32_t size;
};

struct buffer_list_t {
	uint32_t count;
	struct buffer_info buffer[0];
};

typedef struct aod_render_buffer_data {
	uint32_t size;
	uint32_t sub_cmd;
	struct buffer_list_t buffer_list;
} aod_render_buffer_data_t;

typedef struct aod_render_buffer_data_mcu {
	uint32_t sub_cmd;
	struct buffer_list_t buffer_list;
} aod_render_buffer_data_mcu_t;

struct aod_common_data {
	uint32_t size;
	uint32_t cmd_type;
	uint32_t data[0];
};

struct aod_general_data {
	uint32_t size;
	uint32_t data[0];
};

struct aod_general_ioctl_data {
	uint32_t sub_cmd;
	uint32_t result_status;
	uint32_t ioctl_data[0];
};

struct aod_ioctl_data_screen_status {
	uint32_t aod_status;
	uint32_t screen_status;
};

struct aod_ioctl_data_last_backlight {
	uint32_t bl;
	uint32_t index_offset;
};

struct aod_multi_gmp_data {
	uint32_t size;
	uint32_t index;
	uint32_t data[0];
};

/* aod start */
typedef struct aod_notif {
	size_t size;
	unsigned int aod_events;
} aod_notif_t;

/* AP struct */
typedef struct aod_start_config_ap {
	size_t size;
	aod_display_pos_t aod_pos;
	uint32_t intelli_switching;
	uint32_t fb_offset;
	uint32_t bitmaps_offset;
	int pixel_format;
	uint32_t fp_offset;
	uint32_t fp_count;
	uint32_t aod_type;
	uint32_t fp_mode;
	uint32_t utc_cnt; // s
} aod_start_config_ap_t;

typedef struct aod_pause_pos_data {
	size_t size;
	aod_display_pos_t aod_pos;
} aod_pause_pos_data_t;

/* finger down status */
typedef struct aod_notify_finger_down {
	size_t size;
	uint32_t finger_down_status;
} aod_notify_finger_down_t;

typedef struct aod_start_updating_pos_data {
	size_t size;
	aod_display_pos_t aod_pos;
} aod_start_updating_pos_data_t;

typedef struct aod_end_updating_pos_data {
	size_t size;
	aod_display_pos_t aod_pos;
	uint32_t aod_type;
	uint32_t utc_cnt; 
} aod_end_updating_pos_data_t;

typedef struct aod_set_max_and_min_backlight_data {
	size_t size;
	int max_backlight;
	int min_backlight;
} aod_set_max_and_min_backlight_data_t;

typedef struct aod_end_updating_pos_data_to_sensorhub {
	aod_display_pos_t aod_pos;
	uint32_t aod_type;
	uint32_t utc_cnt;
} aod_end_updating_pos_data_to_sensorhub_t;

typedef struct aod_stop_pos_data {
	size_t size;
	aod_display_pos_t aod_pos;
} aod_stop_pos_data_t;

typedef struct aod_resume_config {
	size_t size;
	aod_display_pos_t aod_pos;
	int intelli_switching;
	uint32_t aod_type;
	uint32_t fp_mode;
	uint32_t utc_cnt; // s
} aod_resume_config_t;

typedef struct aod_time_config_ap {
	size_t size;
	uint64_t curr_time;
	int32_t time_zone;
	int32_t sec_time_zone;
	int32_t time_format;
} aod_time_config_ap_t;

typedef struct aod_bitmaps_size_ap {
	size_t size;
	int bitmap_type_count;
	aod_bitmap_size_t bitmap_size[MAX_BIT_MAP_SIZE];
} aod_bitmaps_size_ap_t;

typedef struct aod_display_spaces_ap {
	size_t size;
	unsigned char dual_clocks;
	unsigned char display_space_count;
	uint16_t pd_logo_final_pos_y;
	aod_display_space_t display_spaces[MAX_DISPLAY_SPACE_COUNT];
} aod_display_spaces_ap_t;

typedef struct aod_display_spaces_ap_temp {
	size_t size;
	uint32_t dual_clocks;
	uint32_t display_type;
	uint32_t display_space_count;
	aod_display_space_t display_spaces[MAX_DISPLAY_SPACE_COUNT + DIFF_NUMBER];
} aod_display_spaces_ap_temp_t;

typedef struct aod_fold_info_config_mcu {
	uint32_t panel_id;
	uint32_t fold_state;
	uint32_t display_region;
} aod_fold_info_config_mcu_t;

typedef struct aod_fold_info_data {
	uint32_t size;
	uint32_t panel_id;
	uint32_t fold_state;
	uint32_t display_region;
} aod_fold_info_data_t;

typedef struct aod_lcd_status_data {
	uint32_t size;
	uint32_t sub_cmd;
	uint32_t lcd_on;
} aod_lcd_status_data_t;

typedef struct aod_lcd_status_data_mcu {
	uint32_t sub_cmd;
	uint32_t lcd_on;
} aod_lcd_status_data_mcu_t;

// from user: must have head with size
typedef struct aod_display_param {
	uint32_t size;
	uint32_t sub_cmd;
	uint32_t litehdm_color_mode;
	uint32_t reserved;
} aod_display_param_t;

// to mcu: do not have head with size
typedef struct aod_display_param_mcu {
	uint32_t sub_cmd;
	uint32_t litehdm_color_mode;
	uint32_t reserved;
} aod_display_param_mcu_t;

typedef struct aod_set_vivobus_level_data {
	uint32_t size;
	uint32_t sub_cmd;
	uint32_t vivobus_level;
} aod_set_vivobus_level_data_t;

typedef struct aod_set_vivobus_level_data_mcu {
	uint32_t sub_cmd;
	uint32_t vivobus_level;
} aod_set_vivobus_level_data_mcu_t;

typedef struct aod_template_data {
	uint32_t size;
	uint32_t sub_cmd;
} aod_template_data_t;

// add
typedef struct aod_dss_ctrl_ap_status {
	struct pkt_header hd;
	uint32_t sub_cmd;
} aod_dss_ctrl_status_ap_t;

/* sensorhub struct */
typedef struct aod_dss_ctrl_sh_status {
	struct pkt_header hd;
	uint32_t sub_cmd;
	uint32_t dss_on; // 1 - dss on, 0 - dss off
	uint32_t success; // 0 - on/off success, non-zero - on/off fail
} aod_dss_ctrl_status_sh_t;

typedef struct aod_start_config_mcu {
#if SHMEM_START_CONFIG
	uint32_t cmd_sub_type; // 1:start msg
#endif
	aod_display_pos_t aod_pos;
	int32_t intelli_switching;
	int32_t aod_type;
	int32_t fp_mode;
	uint32_t dynamic_fb_count;
	uint32_t dynamic_ext_fb_count;
	uint32_t face_id_fb_count;
	uint32_t pd_logo_fb_count;
	uint32_t utc_cnt;
#if SHMEM_START_CONFIG
	uint32_t digital_clock_count;
	uint32_t analog_clock_count;
	uint32_t pattern_clock_count;
	uint32_t dynamic_reserve_count;
#endif
	uint32_t dynamic_fb_addr[MAX_DYNAMIC_ALLOC_FB_COUNT];
} aod_start_config_mcu_t;

typedef struct aod_info_mcu {
	struct pkt_header_resp head;
	aod_display_pos_t aod_pos;
} aod_info_mcu_t;

typedef struct aod_time_config_mcu {
	uint64_t curr_time;
	int32_t time_zone;
	int32_t sec_time_zone;
	int32_t time_format;
} aod_time_config_mcu_t;

typedef struct aod_display_spaces_mcu {
	unsigned char dual_clocks;
	unsigned char display_space_count;
	uint16_t pd_logo_final_pos_y;
	aod_display_space_t display_spaces[MAX_DISPLAY_SPACE_COUNT];
} aod_display_spaces_mcu_t;

typedef struct aod_bitmaps_size_mcu {
	int bitmap_type_count;
	aod_bitmap_size_t bitmap_size[MAX_BIT_MAP_SIZE];
} aod_bitmaps_size_mcu_t;

typedef struct aod_config_info_mcu {
	uint32_t aod_fb;
	uint32_t aod_digits_addr;
	aod_screen_info_t screen_info;
	aod_bitmaps_size_mcu_t bitmaps_size;
	int32_t fp_offset;
	int32_t fp_count;
} aod_set_config_mcu_t;
#pragma pack(4)
typedef struct {
	struct pkt_header hd;
	unsigned int subtype;
	union {
		aod_time_config_mcu_t time_param;
		aod_display_spaces_mcu_t display_spaces;
		aod_set_config_mcu_t set_config_param;
		aod_display_pos_t display_pos;
		aod_end_updating_pos_data_to_sensorhub_t end_updating_data_to_sensorhub;
		aod_display_param_mcu_t display_param;
	};
} aod_pkt_t;
#pragma pack()

typedef struct {
	struct pkt_header_resp hd;
	uint8_t data[100];
} aod_pkt_resp_t;

typedef struct aod_ion_buf_fb {
	uint32_t buf_size;
	int32_t ion_buf_fb;
} aod_ion_buf_fb_t;
typedef struct aod_dynamic_fb {
	uint32_t dynamic_fb_count;
	uint32_t dynamic_ext_fb_count;
	uint32_t face_id_fb_count;
	uint32_t pd_logo_fb_count;
#if SHMEM_START_CONFIG
	uint32_t digital_clock_count;
	uint32_t analog_clock_count;
	uint32_t pattern_clock_count;
	uint32_t dynamic_reserve_count;
#endif
	aod_ion_buf_fb_t  str_ion_fb[MAX_DYNAMIC_ALLOC_FB_COUNT] ;
} aod_dynamic_fb_space_t;

struct fb_buf {
	uint32_t size;
	uint32_t addr;
	uint32_t is_need_jpegd;
	uint32_t is_via_smmu;
    uint32_t is_need_arsr;
	uint32_t w; // src width of the frame buffer
	uint32_t h; // src height of the frame buffer
};
struct fb_list {
	uint32_t size;
	uint32_t cmd_type;
	uint32_t dynamic_fb_count;
	struct fb_buf fb[0];
};

typedef struct aod_lcd_pre_on_off_data_mcu {
	uint32_t sub_cmd;
	uint32_t blank_mode;
} aod_lcd_pre_on_off_data_mcu_t;

typedef struct aod_tcu_cfg_data_mcu {
	uint32_t sub_cmd;
	uint32_t reserved; // align to sensorhub struct
	struct smmu_init_info smmu_info_jpegd;
	struct smmu_init_info smmu_info;
} aod_tcu_cfg_data_mcu_t;

struct fb_buf_list {
	uint32_t count;
	struct dma_buf *dma[0];
};

struct aod_data_t {
	dev_t devno;
	unsigned long smem_size;
	unsigned long fb_start_addr;
	unsigned long aod_digits_addr;
	uint32_t x_res;
	uint32_t y_res;
	uint32_t bpp;
	uint32_t max_buf_size;
	struct class *aod_dev_class;
	struct device *dev;
	struct device *aod_cdevice;
	struct cdev cdev;
	struct mutex ioctl_lock;
	struct mutex mm_lock;
	bool fb_mem_alloc_flag;
	bool ion_dynamic_alloc_flag;
	bool ion_dynamic_update_flag;
	struct ion_client *ion_client;
	struct ion_handle *ion_dyn_handle[MAX_DYNAMIC_ALLOC_FB_COUNT];
#ifdef ION_ALLOC_BUFFER
	struct ion_client *ion_client;
	struct ion_handle *ion_handle;
#else
	struct device *cma_device;
	char *buff_virt;
	phys_addr_t buff_phy;
#endif
	aod_start_config_mcu_t start_config_mcu;
	aod_set_config_mcu_t set_config_mcu;
	aod_time_config_mcu_t time_config_mcu;
	aod_bitmaps_size_mcu_t bitmaps_size_mcu;
	aod_display_spaces_mcu_t display_spaces_mcu;
	aod_fold_info_config_mcu_t fold_info_config_mcu;
	aod_display_param_mcu_t display_param_mcu;
	aod_lcd_status_data_mcu_t lcd_status_mcu;
	aod_set_vivobus_level_data_mcu_t vivobus_level_mcu;
	aod_lcd_pre_on_off_data_mcu_t lcd_pre_on_off_mcu;
	aod_tcu_cfg_data_mcu_t tcu_cfg_mcu;
	aod_display_pos_t pos_data;
	aod_notif_t aod_notify_data;

	int blank_mode;
	int aod_status;
	uint32_t finger_down_status;
	bool start_req_faster;
	bool no_need_enter_aod;
	struct semaphore aod_status_sem;
	struct mutex aod_lock;
	struct wakeup_source *wlock;
	// true:ap hold the aod lock; false:ap release the aod lock
	bool aod_lock_status;

	atomic_t atomic_v;
};

#pragma pack(4)
typedef struct {
	struct pkt_header hd;
	uint32_t subtype;
	union {
		aod_fold_info_config_mcu_t fold_info_config_param;
	};
}aod_pkt_pack_t;
#pragma pack()

#endif