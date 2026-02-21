
 
#ifndef __HISI_FBEX_UECE_ENHANCE_H_
#define __HISI_FBEX_UECE_ENHANCE_H_
 
#include <linux/types.h>
#include <platform_include/see/fbe_ctrl.h>
 
typedef uint32_t (*fbex_uece_enhance_func)(u32 user_id_double, u32 user_id_single, u32 status, u8 *buf, u32 length);


struct fbex_uece_enhance_function_table {
	uint32_t cmd_id;
	fbex_uece_enhance_func func;
};
 
struct fbex_uece_enhance_req {
	u32 user_id_double;
	u32 user_id_single;
	u32 status;
	u32 length;
	u8 buf[KEY_LEN];
};

#ifdef CONFIG_FBE_UECE_ENHANCE

#define FBEX4_IOC_MAGIC 'f'

#define HISI_FBEX_GET_CURRENT_UECE_AVAIL_STATUS _IOW(FBEX4_IOC_MAGIC, 28, struct fbex_uece_enhance_req)
#define HISI_FBEX_DELETE_UECE_AUTHTOKEN _IOWR(FBEX4_IOC_MAGIC, 29, struct fbex_uece_enhance_req)

int fbex_init_uece_enhance_dev(void);
void fbex_cleanup_uece_enhance_dev(void);
#else
static inline int fbex_init_uece_enhance_dev(void) {return 0;}
static inline void fbex_cleanup_uece_enhance_dev(void) {}
#endif /* CONFIG_FBE_UECE_ENHANCE */

#endif /* __HISI_FBEX_UECE_H_ */