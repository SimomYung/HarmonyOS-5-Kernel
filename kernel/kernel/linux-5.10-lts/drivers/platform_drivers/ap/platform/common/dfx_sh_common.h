#ifndef __DFX_SH_COMMON_H__
#define __DFX_SH_COMMON_H__

#define FREQ_MAX_NUM_CPU_LIT	20
#define FREQ_MAX_NUM_CPU_MID	20
#define FREQ_MAX_NUM_CPU_BIG	20
#define FREQ_MAX_NUM_L3		20
#define FREQ_MAX_NUM_PERI	10

#define SH_CFG_INFO		513
#define SH_CFG_PV		514
#define CPU_SELF_HEALING_MAGIC 0x12889641
#define CPU_SH_MAX_CLUSTER_NUM 15
#define SH_SW_CNT_MAX_NUM	7
#define SH_NV_SIZE	104

#define SH_VALUES_NOT_NEEDED  0
#define SH_UNSUPPORTED	0
#define SH_CFG_DISABLE  0
#define SH_CFG_ENABLE   1
#define SH_PV_ON   1
#define SH_PV_OFF  0
#define SH_STATE_CHANGE     1
#define SH_STATE_NO_CHANGE  0

#define SH_CFG_INFO_TYPE_MAX   16
#define SH_CFG_INFO_MAIN_RSV   13
#define SH_CFG_INFO_SUB_RSV   32

#define SH_CPU_ECC_CE 0
#define SH_CPU_ECC_DE 1
#define SH_CPU_ECC_UE 2
/*
 * 00-19: sh_type
 * 20-39: acpu dfv (sh_type + 20)
 * 40-59: sh type outside the framework
 * 60-99: reserved
 */
#define SH_TYPE_DFV_START   20
#define SH_TYPE_GPU         40
#define SH_TYPE_NPU         41
#define SH_TYPE_LU          42
#define SH_TYPE_VDEC        43
#define SH_TYPE_HFRC        44

struct sh_sw_cnt {
	unsigned char count:3;
	unsigned char threshold:3;
	unsigned char pv_flag:1;
	unsigned char enable:1;
};

struct sh_lu_cfg {
	unsigned char enable : 1;
	unsigned char flag : 1;
	unsigned char reserved : 6;
};

struct sh_dfv_cfg {
	unsigned char enable : 1;
	unsigned char threshold : 3;
	unsigned short flag : 12;
};

struct sh_hfrc_cfg {
	unsigned char enable : 1;
	unsigned char reserved : 1;
	unsigned char threshold : 3;
	unsigned char count : 3;
};

struct sh_dss_cfg {
	unsigned char enable : 1;
	unsigned char flag : 1;
	unsigned char reserved : 6;
};

struct sh_vdec_cfg {
	unsigned char enable : 1;
	unsigned char flag : 1;
	unsigned char reserved : 6;
};

#define DDR_DFS_EXCE_MAGICNUM 0x5A5A4A4A

struct sh_ddr_nv_info {
	unsigned char dfs_channel;
	unsigned char dfs_cnts : 7;
	unsigned char dfs_enable : 1;
};

typedef struct ddr_dfs_fail_info {
	unsigned int magic_num_start;
	struct sh_ddr_nv_info ddr_info;
	unsigned int magic_num_end;
} UEFI_DDR_NV_INFO;

/*
 * sh config data includes 134 bytes, while one nve item supports only 104 bytes,
 * so sh config data need to be stored in two nve items. we read or write these
 * two nve item through structure sh_cfg_info_main and sh_cfg_info_sub.
 */
struct sh_cfg_info_main {
	struct sh_sw_cnt sw_cnt[SH_CFG_INFO_TYPE_MAX];
	unsigned int panic_time[SH_CFG_INFO_TYPE_MAX];
	unsigned char reserved[SH_CFG_INFO_MAIN_RSV];
	struct sh_lu_cfg lu_cfg;
	struct sh_dfv_cfg dfv_cfg;
	unsigned int version_rtc_time;
	unsigned short clear_data_enable;
	unsigned short version;
}__attribute__((aligned(1)));

struct sh_cfg_info_sub {
	unsigned int clear_interval[SH_CFG_INFO_TYPE_MAX];
	unsigned short clear_interval_enable;
	unsigned char reserved[SH_CFG_INFO_SUB_RSV];
	struct sh_hfrc_cfg hfrc_cfg;
	struct sh_dss_cfg dss_cfg;
	struct sh_vdec_cfg vdec_cfg;
	struct sh_ddr_nv_info ddr_nv_info;
	unsigned char never_use;
}__attribute__((aligned(1)));

#ifdef CONFIG_DFX_SH
void cpu_ecc_do_sh(int ecc_type, int cluster);
int get_lu_sh_flag(int cluster);
int early_get_vdec_sh_flag(void);
int early_set_vdec_sh_flag(int state);
void gpu_exc_do_peri_sh(void);
int is_sh_vdec_exc(void);
int early_get_dss_sh_flag(void);
int early_set_dss_sh_flag(int state);
#else
static inline void cpu_ecc_do_sh(int ecc_type, int cluster) { (void)(ecc_type); (void)(cluster); }
static inline int get_lu_sh_flag(int cluster) { (void)(cluster); return SH_UNSUPPORTED; }
static inline int early_get_vdec_sh_flag(void) { return SH_PV_OFF; }
static inline int early_set_vdec_sh_flag(int state) { (void)(state); return SH_UNSUPPORTED;}
static inline void gpu_exc_do_peri_sh(void) { return; }
static inline int is_sh_vdec_exc(void) { return 0; };  /*  NO VDEC EXCEPTION */
static inline int early_get_dss_sh_flag(void) { return SH_PV_OFF; }
static inline int early_set_dss_sh_flag(int state) { (void)(state); return SH_UNSUPPORTED;}
#endif

#endif /* __DFX_COMMON_H__ */
