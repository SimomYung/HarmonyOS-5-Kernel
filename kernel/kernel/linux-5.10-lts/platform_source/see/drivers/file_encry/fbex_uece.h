

 
#ifndef __HISI_FBEX_UECE_H_
#define __HISI_FBEX_UECE_H_
 
#include <linux/types.h>
#include <platform_include/see/fbe_ctrl.h>
 
typedef uint32_t (*fbex_uece_func)(u32 user_id_double, u32 user_id_single, u32 status, u8 *buf, u32 length);
 
struct fbex_uece_function_table {
	uint32_t cmd_id;
	fbex_uece_func func;
};
 
struct fbex_uece_req {
	u32 user_id_double;
	u32 user_id_single;
	u32 status;
	u32 length;
	u8 buf[KEY_LEN];
};
 
#ifdef CONFIG_FBE_UECE

#define FBEX4_IOC_MAGIC 'f'

#define HISI_FBEX_ADD_UECE _IOWR(FBEX4_IOC_MAGIC, 21, struct fbex_uece_req)
#define HISI_FBEX_READ_UECE _IOWR(FBEX4_IOC_MAGIC, 22, struct fbex_uece_req)
#define HISI_FBEX_WRITE_UECE _IOWR(FBEX4_IOC_MAGIC, 23, struct fbex_uece_req)
#define HISI_FBEX_DEL_USER_PINCODE _IOWR(FBEX4_IOC_MAGIC, 24, struct fbex_uece_req)
#define HISI_FBEX_ADD_APPKEY2 _IOWR(FBEX4_IOC_MAGIC, 25, struct fbex_uece_req)
#define HISI_FBEX_CHANGE_PINCODE _IOWR(FBEX4_IOC_MAGIC, 26, struct fbex_uece_req)
#define HISI_FBEX_LOCK_UECE _IOWR(FBEX4_IOC_MAGIC, 27, struct fbex_uece_req)

#ifdef CONFIG_FBE_DE_DOUBLE_TO_SINGLE
#define HISI_FBEX_ADD_DE_CHANGE_USER_ID _IOWR(FBEX4_IOC_MAGIC, 20, struct fbex_uece_req)
#endif

int fbex_init_uece_dev(void);
void fbex_cleanup_uece_dev(void);
#else
static inline int fbex_init_uece_dev(void) {return 0;}
static inline void fbex_cleanup_uece_dev(void) {}
#endif
 
#endif /* __HISI_FBEX_UECE_H_ */