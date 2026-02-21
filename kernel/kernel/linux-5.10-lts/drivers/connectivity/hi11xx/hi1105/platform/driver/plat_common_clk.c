/**
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 *
 * Description: Common clock drive
 * Author: @CompanyNameTag
 */

/*****************************************************************************
  1 头文件包含
*****************************************************************************/
#include "hisi_ini.h"
#include "plat_debug.h"
#include "chr_user.h"
#include "board.h"
#include "oal_util.h"

#ifdef HISI_NVRAM_SUPPORT
#include "nve_info_interface.h"
#endif /* end for HISI_NVRAM_SUPPORT */

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 35))
#ifndef _PRE_NO_HISI_NVRAM
#define HISI_NVRAM_SUPPORT
#endif
#endif

#include "plat_common_clk.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_PLATFORM_COMMON_CLK_C

/** 时钟参数配置 **/
#define INI_BUF_SIZE         16
#define INI_TCXO_DCXO_MODE  "tcxo_dcxo_mode"
#define INI_SUB_CLK_MODE    "sub_clk_mode"
#define INI_THIRD_CLK_MODE  "third_clk_mode"

typedef struct {
    int8_t *label;
    uint32_t label_value;
} clk_label_str;

/* PMU 参数配置 */
#define INI_PMU_CHIP_TYPE "pmu_version"
#define INI_6555V300_STR  "Hi6555V300"
#define INI_6421V700_STR  "Hi6421V700"
#define INI_6555V200_STR  "Hi6555V200"
#define INI_6421BV100_STR "Hi6421BV100"

// PMU 6555V300
#define PMU_6555V300_CALI_END  0x3A0
#define PMU_6555V300_CALI_AVE0 0x3A1
#define PMU_6555V300_CALI_AVE1 0x3A2
#define PMU_6555V300_CALI_AVE2 0x3A3
#define PMU_6555V300_CALI_AVE3 0x3A4
#define PMU_6555V300_ANA_EN    0x3AA
#define PMU_6555V300_STATE     0x3AC
#define PMU_6555V300_LOW_BYTE  0x3AD
#define PMU_6555V300_HIGH_BYTE 0x3AE

// PMU 6421V700
#define PMU_6421V700_CALI_END  0x3A0
#define PMU_6421V700_CALI_AVE0 0x3A1
#define PMU_6421V700_CALI_AVE1 0x3A2
#define PMU_6421V700_CALI_AVE2 0x3A3
#define PMU_6421V700_CALI_AVE3 0x3A4
#define PMU_6421V700_ANA_EN    0x3AA
#define PMU_6421V700_STATE     0x3AC
#define PMU_6421V700_LOW_BYTE  0x3AD
#define PMU_6421V700_HIGH_BYTE 0x3AE

// PMU 6555V200
#define PMU_6555V200_CALI_END  0x254
#define PMU_6555V200_CALI_AVE0 0x255
#define PMU_6555V200_CALI_AVE1 0x256
#define PMU_6555V200_CALI_AVE2 0x257
#define PMU_6555V200_CALI_AVE3 0x258
#define PMU_6555V200_ANA_EN    0x25E
#define PMU_6555V200_STATE     0x260
#define PMU_6555V200_LOW_BYTE  0x261
#define PMU_6555V200_HIGH_BYTE 0x262

// PMU 6421BV100
#define PMU_6421BV100_CALI_END  0x3A0
#define PMU_6421BV100_CALI_AVE0 0x3A1
#define PMU_6421BV100_CALI_AVE1 0x3A2
#define PMU_6421BV100_CALI_AVE2 0x3A3
#define PMU_6421BV100_CALI_AVE3 0x3A4
#define PMU_6421BV100_ANA_EN    0x3B5
#define PMU_6421BV100_STATE     0x3B7
#define PMU_6421BV100_LOW_BYTE  0x3B8
#define PMU_6421BV100_HIGH_BYTE 0x3B9

enum pmu_type_enum {
    PMU_HI655V300   = 0,
    PMU_HI6421V700  = 1,
    PMU_HI6555V200  = 2,
    PMU_HI6421BV100 = 3,
    PMU_TYPE_BUFF
};

// io dts attribute
#define DTS_PROP_MPXX_GPIO_TCXO_FREQ_DETECT  "hi110x,gpio_tcxo_freq_detect"
#define PROC_NAME_MPXX_GPIO_TCXO_FREQ "hi110x_tcxo_freq_detect"

#define DTS_PROP_MPXX_POWER_PREPARE "hi110x,power_prepare"
#define DTS_PROP_GPIO_TCXO_LEVEL      "hi110x,tcxo_gpio_level"

/* 以下是DCXO 读取的NV中的参数 */
#define MALLOC_LEN            104
#define NV_GNSS_ID            236
#define NV_GNSS_SIZE          32
#define NV_GNSS_NAME          "XOCOE"
#define NV_FAC_ID             235
#define NV_FAC_SIZE           88
#define NV_FAC_NAME           "XOA1A0"
#define PLAT_DEF_ID           (-1)
#define PLAT_DEF_NAME         "PLAT_DEF_COEFF"
#define NVRAM_READ            1
#define NV_CHK_PAIR           2
#define RSV_NUM               2
#define DCXO_NV_CHK_OK        1
#define T0_BIT_WIDTH_10       0
#define T0_WIDTH10_TO_WIDTH16 (1 << 6)
#define DCXO_PARA_READ_OK     0x01
#define DCXO_PARA_T0_30       0x1E0000
#define FAC_NV_CRC_MASK       0xFFFF

// 默认下发给device的参数
#define DEFAULT_T0       0x1D0000    // 29*2^16
#define DEFAULT_A0       0
#define DEFAULT_A1       0xFFFc0000  // -0.25*2^20
#define DEFAULT_A2       0
#define DEFAULT_A3       0x68db8b  // 0.0001*2^36
#define DEFAULT_MODE     0
#define DCXO_CHECK_MAGIC 0xA0A0A0A0

// 默认a0~t0的min max参数
#define DEF_A0_MIN (-3145728)  // -12*2^18
#define DEF_A0_MAX 3145728     // 12*2^18
#define DEF_A1_MIN (-419430)   // -0.4*2^20
#define DEF_A1_MAX (-104857)   // -0.1*2^20
#define DEF_A2_MIN (-30923764) // -0.00045*2^36
#define DEF_A2_MAX 30923764    // 0.00045*2^36
#define DEF_A3_MIN 5841155     // 0.000085*2^36
#define DEF_A3_MAX 7902739     // 0.000115*2^36
#define DEF_T0_MIN 1638400     // 25*2^16
#define DEF_T0_MAX 2293760     // 35*2^16

// a0~t0 min max 异常范围检测参数
#define A0_EXCEP_PROT_MIN (-13107200)   // -50*2^18
#define A0_EXCEP_PROT_MAX 13107200      // 50*2^18
#define A1_EXCEP_PROT_MIN (-1048576)    // -1*2^20
#define A1_EXCEP_PROT_MAX 0             // 0*2^20
#define A2_EXCEP_PROT_MIN (-1374389535) // -0.02*2^36
#define A2_EXCEP_PROT_MAX 1374389535    // 0.02*2^36
#define A3_EXCEP_PROT_MIN 0             // 0*2^36
#define A3_EXCEP_PROT_MAX 1374389535    // 0.02*2^36
#define T0_EXCEP_PROT_MIN 1310720       // 20*2^16
#define T0_EXCEP_PROT_MAX 2621440       // 40*2^16

enum coeff_type_enum {
    INIT_TYPE_MIN = 0,
    INIT_TYPE_MAX = 1,
    COEFF_TYPE_BUFF
};

typedef struct {
    int32_t valid;                            // 判断当期nv是否有效， 1:有效 0:无效
    int32_t coeff_para[COEFF_NUM];            // 存放a0, a1, a2, a3, t0
    uint32_t coeff_time;                      // gnss相关参数，平台未使用
    int32_t reserve0;                         // 保留
    int32_t coeff_th[COEFF_NUM][NV_CHK_PAIR]; // a0~t0阈值范围放到ini文件中配置，此处已经不再使用
    int32_t t0_bit_width_fix;                 // 0: t0存储的是10位，需要乘以2^6修正, 1:t0存储的是16位，不需要修正
    int32_t tempr_expand;                     // gnss相关参数，平台未使用
    int32_t reserve1;                         // 保留
    uint32_t crc_relt;                        // crc校验结果
} dcxo_nv_info_stru;

typedef struct {
    int8_t *pc_name;     // ini 文件中对应的名字
    int8_t c_index;      // 使用的是第几组参数a0，第一组， a1,第二组...
    int32_t l_type;      // 数组最大值/最小值的index
    int32_t l_prot_min;  // 读取值的下限保护
    int32_t l_prot_max;  // 读取值的上限保护
    int32_t l_def_val;   // 存放默认数据的结果
} ini_file_str;

// 由于devcie划定的存储空间限制，这里需要做判断
oal_static_assert(((sizeof(dcxo_dl_para_stru)) <= DCXO_DEVICE_MAX_BUF_LEN), device_mem_must_big_than_host);

typedef struct {
    char *label_name;
    clk_mode_enum mode;
    clk_freq_enum freq;
} clk_label_stru;

typedef struct {
    int32_t part_id;     // nv号
    int32_t total_size;  // nv项大小
    int32_t read_size;   // 存储实际有效内容的大小
    int32_t used;        // 当前是否在使用
    char *name;
    int32_t (*process_cali)(uint8_t *pc_buffer, dcxo_dl_para_stru *pst_dl_para);  // 处理函数
} nv_part_str;

#define PLAT_MAX_REPORT_CNT       10
#define PLAT_CHR_ERRNO_DCXO_NV_RD 910800034
// 从nv中读取的参数异常
typedef struct {
    int32_t l_struct_len;       // 结构体的大小
    int32_t l_para[COEFF_NUM];  // 存储依次为 a0, a1, a2, a3, t0
} plat_chr_nv_rd_excep_stru;

typedef struct {
    int32_t use_part_id;
    uint32_t chr_nv_read_err;
    long nv_init_flag;
    plat_clk_mode_str mode;
    clk_det_mode clk_det;
    int32_t clk_io_nr;                  // clk io number in dts
    uint8_t clk_io_level[CLK_BUFF];      // clk io level in dts
    uint32_t pmu_chip_type;
    int32_t check_value[COEFF_NUM][NV_CHK_PAIR];  // 最大值/最小值判断
} dcxo_manage_stru;

/*****************************************************************************
  3 函数声明
*****************************************************************************/
OAL_STATIC  int32_t process_plat_cali(uint8_t *pc_buffer, dcxo_dl_para_stru *pst_dl_para);
OAL_STATIC int32_t process_factory_cali(uint8_t *pc_buffer, dcxo_dl_para_stru *pst_dl_para);
OAL_STATIC int32_t process_gnss_self_study(uint8_t *pc_buffer, dcxo_dl_para_stru *pst_dl_para);

/*****************************************************************************
  4 全局变量定义
*****************************************************************************/
dcxo_manage_stru g_dcxo_info = {0};

/* 最多支持4路, 参考 plat_clk_mode_str结构体 */
clk_label_stru g_clk_label[] = {
    /* 主路时钟, 默认tcxo */
    [0] = {
        .label_name = INI_TCXO_DCXO_MODE,
        .mode = CLK_MODE_TCXO,
        .freq = CLK_NONE,
    },
    /* 次路时钟 */
    [1] = {
        .label_name = INI_SUB_CLK_MODE,
        .mode = CLK_MODE_ABSENT,
        .freq = CLK_NONE
    },
    /* 第三路时钟 */
    [2] = {
        .label_name = INI_THIRD_CLK_MODE,
        .mode = CLK_MODE_ABSENT,
        .freq = CLK_NONE
    },
    /* 第四路时钟(reserve) */
    [3] = {
        .label_name = NULL,
        .mode = CLK_MODE_ABSENT,
        .freq = CLK_NONE
    }
};

OAL_STATIC uint8_t *g_dcxo_data_buf = NULL;  // 存储从NV中读取的参数信息，解析cfg文件时赋值一次，后续GNSS业务需要，也会更新该变量
OAL_STATIC dcxo_nv_info_stru g_dcxo_factory = {0};

OAL_STATIC int32_t g_default_coeff[COEFF_NUM] = {
    DEFAULT_A0,
    DEFAULT_A1,
    DEFAULT_A2,
    DEFAULT_A3,
    DEFAULT_T0
};

OAL_STATIC dcxo_pmu_addr_stru g_pmu_addr[PMU_TYPE_BUFF] = {

    {   PMU_6555V300_CALI_END, PMU_6555V300_CALI_AVE0, PMU_6555V300_CALI_AVE1,
        PMU_6555V300_CALI_AVE2, PMU_6555V300_CALI_AVE3, PMU_6555V300_ANA_EN,
        PMU_6555V300_STATE, PMU_6555V300_LOW_BYTE, PMU_6555V300_HIGH_BYTE,
        PMU_HI655V300
    },

    {   PMU_6421V700_CALI_END, PMU_6421V700_CALI_AVE0, PMU_6421V700_CALI_AVE1,
        PMU_6421V700_CALI_AVE2, PMU_6421V700_CALI_AVE3, PMU_6421V700_ANA_EN,
        PMU_6421V700_STATE, PMU_6421V700_LOW_BYTE, PMU_6421V700_HIGH_BYTE,
        PMU_HI6421V700
    },
    {
        PMU_6555V200_CALI_END, PMU_6555V200_CALI_AVE0, PMU_6555V200_CALI_AVE1,
        PMU_6555V200_CALI_AVE2, PMU_6555V200_CALI_AVE3, PMU_6555V200_ANA_EN,
        PMU_6555V200_STATE, PMU_6555V200_LOW_BYTE, PMU_6555V200_HIGH_BYTE,
        PMU_HI6555V200
    },
    {
        PMU_6421BV100_CALI_END, PMU_6421BV100_CALI_AVE0, PMU_6421BV100_CALI_AVE1,
        PMU_6421BV100_CALI_AVE2, PMU_6421BV100_CALI_AVE3, PMU_6421BV100_ANA_EN,
        PMU_6421BV100_STATE, PMU_6421BV100_LOW_BYTE, PMU_6421BV100_HIGH_BYTE,
        PMU_HI6421BV100
    }
};

/* 先读平台默认值，其次工厂刷入的nv，最后读自学习的nv，顺序不能反 */
OAL_STATIC nv_part_str g_dcxo_nv_part[] = {
    {   PLAT_DEF_ID, sizeof(g_default_coeff), sizeof(g_default_coeff),
        OAL_TRUE, PLAT_DEF_NAME, process_plat_cali
    },
    {   NV_FAC_ID, NV_FAC_SIZE, sizeof(dcxo_nv_info_stru),
        OAL_TRUE, NV_FAC_NAME, process_factory_cali
    },
    /* GNSS 自学习分区的大小是valid标志位 + a0~t0五个系数 */
    {   NV_GNSS_ID, NV_GNSS_SIZE, sizeof(int32_t) + sizeof(uint32_t)*COEFF_NUM,
        OAL_TRUE, NV_GNSS_NAME, process_gnss_self_study
    }
};

/* 通过该顺序来读取ini文件， 其中的顺序不能打乱，原因看deal_min_max_val 注释的策略1 */
OAL_STATIC ini_file_str g_dcxo_ini_para[] = {
    { "a0_min", COEFF_A0, INIT_TYPE_MIN, A0_EXCEP_PROT_MIN, A0_EXCEP_PROT_MAX, DEF_A0_MIN },
    { "a0_max", COEFF_A0, INIT_TYPE_MAX, A0_EXCEP_PROT_MIN, A0_EXCEP_PROT_MAX, DEF_A0_MAX },
    { "a1_min", COEFF_A1, INIT_TYPE_MIN, A1_EXCEP_PROT_MIN, A1_EXCEP_PROT_MAX, DEF_A1_MIN },
    { "a1_max", COEFF_A1, INIT_TYPE_MAX, A1_EXCEP_PROT_MIN, A1_EXCEP_PROT_MAX, DEF_A1_MAX },
    { "a2_min", COEFF_A2, INIT_TYPE_MIN, A2_EXCEP_PROT_MIN, A2_EXCEP_PROT_MAX, DEF_A2_MIN },
    { "a2_max", COEFF_A2, INIT_TYPE_MAX, A2_EXCEP_PROT_MIN, A2_EXCEP_PROT_MAX, DEF_A2_MAX },
    { "a3_min", COEFF_A3, INIT_TYPE_MIN, A3_EXCEP_PROT_MIN, A3_EXCEP_PROT_MAX, DEF_A3_MIN },
    { "a3_max", COEFF_A3, INIT_TYPE_MAX, A3_EXCEP_PROT_MIN, A3_EXCEP_PROT_MAX, DEF_A3_MAX },
    { "t0_min", COEFF_T0, INIT_TYPE_MIN, T0_EXCEP_PROT_MIN, T0_EXCEP_PROT_MAX, DEF_T0_MIN },
    { "t0_max", COEFF_T0, INIT_TYPE_MAX, T0_EXCEP_PROT_MIN, T0_EXCEP_PROT_MAX, DEF_T0_MAX }
};

OAL_STATIC clk_label_str g_clk_mode_label[] = {
    {"tcxo", CLK_MODE_TCXO}, {"dcxo", CLK_MODE_DCXO},
    {"absent", CLK_MODE_ABSENT}, {"xo", CLK_MODE_XO}
};

OAL_STATIC clk_label_str g_freq_label[] = {
    {"26M", CLK_26M},    {"38P4M", CLK_38P4}, {"48M", CLK_48M},
    {"76P8M", CLK_76P8}, {"96M", CLK_96},     {"absent", CLK_NONE}
};

OAL_STATIC nv_part_str *get_nv_part_from_id(int32_t part_id)
{
    int32_t loop;
    for (loop = 0; loop < oal_array_size(g_dcxo_nv_part); loop++) {
        if (g_dcxo_nv_part[loop].part_id == part_id) {
            return &(g_dcxo_nv_part[loop]);
        }
    }

    return NULL;
}

/*
 * 功能描述  : 判断从ini文件中读取的数据是否有效，并将数据存到dcxo_info.check_value中
 *             判断数据有效的判断策略:
 *             1 从ini中读到的max 一定要比min要大，比如t0_max要比t0_min要大，否则t0_min, t0_max都视为无效数据
 *             2 从ini中读到的每一个值，都会做一次异常值判断，不能超过软件定义的最大范围。
 *
 *             无效数据的处理:
 *             使用软件定义的默认参数。
 *
 *             由于数组每次是读取一个项，所以实现策略1，需要先保存上一个值，再做判断。
 *             具体的实现方法:
 *             按照g_dcxo_ini_para数组定义的数据结构，每次都是先读min值，再读max值，最终结果存到check_value数组中
 *             此处以t0举例:
 *             当读t0_min时，将读取到的值存放到内存中check_value[0][INIT_TYPE_MIN](第一个0表明是第一组T0),同时将
 *             t0的默认值，存放到check_value[0][INIT_TYPE_MAX]
 *
 *             当读t0_max时, 先判断该值是否大于check_value[0][INIT_TYPE_MIN],如果不是，表明出现异常，
 *             先用check_value[0][INIT_TYPE_MAX]值，恢复t0_min的默认值，然后将INIT_TYPE_MAX位置赋值t0_max初始值
 *             如果是， 将INIT_TYPE_MAX位置，赋值为ini中读取的值。
 * 输入参数  : c_para_pos表明在g_dcxo_ini_para数组中第几个项的index
 *             l_val   从ini文件中读取到值
 * 返 回 值  : 读取内容有效，返回INI_SUCC，否则返回INI_FAILED
 */
OAL_STATIC int32_t deal_min_max_val(int8_t c_para_pos, int32_t l_val)
{
    int8_t c_chk_arr_pos;
    int32_t l_pair_pos;
    int32_t *al_check_coeff = NULL;

    c_chk_arr_pos = g_dcxo_ini_para[c_para_pos].c_index;
    l_pair_pos = g_dcxo_ini_para[c_para_pos].l_type;
    al_check_coeff = g_dcxo_info.check_value[c_chk_arr_pos];

    ps_print_info("[dcxo] read ini file [index,pos] [%d,%d] value %d\n", c_chk_arr_pos, l_pair_pos, l_val);
    // 对min值做处理，将min的默认值保存在check_value[INIT_TYPE_MAX]位置
    if (l_pair_pos == INIT_TYPE_MIN) {
        al_check_coeff[INIT_TYPE_MIN] = l_val;
        al_check_coeff[INIT_TYPE_MAX] = g_dcxo_ini_para[c_para_pos].l_def_val;
    }

    // 如果此时读的是max，并且不满足min < max, 需要将check_value中min的位置，用默认值替换
    if ((l_pair_pos == INIT_TYPE_MAX) && (l_val <= al_check_coeff[INIT_TYPE_MIN])) {
        al_check_coeff[INIT_TYPE_MIN] = al_check_coeff[INIT_TYPE_MAX];
        al_check_coeff[INIT_TYPE_MAX] = g_dcxo_ini_para[c_para_pos].l_def_val;
        ps_print_err("[dcxo][ini] get par array id [%d] val[%d] min >= max \n", c_para_pos, l_val);
        return INI_FAILED;
    }

    // min max的异常范围检测
    if ((l_val > g_dcxo_ini_para[c_para_pos].l_prot_max) || (l_val < g_dcxo_ini_para[c_para_pos].l_prot_min)) {
        ps_print_err("[dcxo][ini] get id [%d] val[%d], not limit between [%d]<->[%d]\n",
                     c_para_pos, l_val, g_dcxo_ini_para[c_para_pos].l_prot_min,
                     g_dcxo_ini_para[c_para_pos].l_prot_max);

        al_check_coeff[l_pair_pos] = g_dcxo_ini_para[c_para_pos].l_def_val;
        return INI_FAILED;
    }

    al_check_coeff[l_pair_pos] = l_val;

    return INI_SUCC;
}

/*
 * 功能描述  : 从ini文件中读取 min/max参数的值
 */
OAL_STATIC void read_min_max_val_from_ini_file(void)
{
    int8_t loop;
    int8_t c_index;
    int32_t l_val = 0;
    int32_t l_type;

    for (loop = 0; loop < oal_array_size(g_dcxo_ini_para); loop++) {
        if (get_cust_conf_int32(INI_MODU_PLAT, g_dcxo_ini_para[loop].pc_name, &l_val) != INI_SUCC) {
            // 如果读取ini文件失败, 使用默认配置
            c_index = g_dcxo_ini_para[loop].c_index;
            l_type = g_dcxo_ini_para[loop].l_type;
            g_dcxo_info.check_value[c_index][l_type] = g_dcxo_ini_para[loop].l_def_val;
            ps_print_err("[dcxo] [index,pos][%d,%d] read from ini failed, use default value %d\n",
                         c_index, l_type, g_dcxo_ini_para[loop].l_def_val);
            continue;
        }

        if (deal_min_max_val(loop, l_val) == INI_FAILED) {
            ps_print_err("[dcxo] deal para arry id[%d], min-max error\n", loop);
        }
    }
}

/*
 * 功能描述  : 从ini文件中读取 PMU的型号
 */
OAL_STATIC void read_pmu_type_from_ini_file(void)
{
    char ac_type_buff[INI_BUF_SIZE] = {0};
    int32_t ret;
    g_dcxo_info.pmu_chip_type = PMU_HI655V300;

    ret = get_cust_conf_string(INI_MODU_PLAT, INI_PMU_CHIP_TYPE, ac_type_buff, sizeof(ac_type_buff));
    if (ret != INI_SUCC) {
        ps_print_err("[dcxo] read pmu type from ini failed, use default HI655V300\n");
        return;
    }

    if (OAL_STRNCMP(INI_6555V300_STR, ac_type_buff, OAL_STRLEN(INI_6555V300_STR)) == 0) {
        g_dcxo_info.pmu_chip_type = PMU_HI655V300;
        ps_print_info("[dcxo] use pmu %s", INI_6555V300_STR);
    } else if (OAL_STRNCMP(INI_6421V700_STR, ac_type_buff, OAL_STRLEN(INI_6421V700_STR)) == 0) {
        g_dcxo_info.pmu_chip_type = PMU_HI6421V700;
        ps_print_info("[dcxo] use pmu %s", INI_6421V700_STR);
    } else if (OAL_STRNCMP(INI_6555V200_STR, ac_type_buff, OAL_STRLEN(INI_6555V200_STR)) == 0) {
        g_dcxo_info.pmu_chip_type = PMU_HI6555V200;
        ps_print_info("[dcxo] use pmu %s", INI_6555V200_STR);
    } else if (OAL_STRNCMP(INI_6421BV100_STR, ac_type_buff, OAL_STRLEN(INI_6421BV100_STR)) == 0) {
        g_dcxo_info.pmu_chip_type = PMU_HI6421BV100;
        ps_print_info("[dcxo] use pmu %s", INI_6421BV100_STR);
    } else {
        ps_print_err("[dcxo] read pmu info %s not vld, use default HI655V300\n", ac_type_buff);
    }
}

/*
 * 功能描述  : 上报CHR事件
 * 输入参数  : chr_errno 上报的事件id,
 *             chr_ptr 上报内容的首地址，
 *             chr_len 上报内容的长度
 */
OAL_STATIC  void report_chr_err(int32_t chr_errno, uint8_t *chr_ptr, uint16_t chr_len)
{
    switch (chr_errno) {
        case PLAT_CHR_ERRNO_DCXO_NV_RD:
            if (chr_len != sizeof(plat_chr_nv_rd_excep_stru)) {
                ps_print_err("[dcxo] [chr] id [%d] input len [%d] not match [%d]\n",
                             chr_errno, chr_len, (int32_t)sizeof(plat_chr_nv_rd_excep_stru));

                return;
            }
            if (g_dcxo_info.chr_nv_read_err > PLAT_MAX_REPORT_CNT) {
                ps_print_err("[dcxo] [chr] id [%d] exceed max report\n", chr_errno);
                return;
            }
            g_dcxo_info.chr_nv_read_err++;
            chr_exception_p(chr_errno, chr_ptr, chr_len);
            break;
        default:
            ps_print_err("[dcxo] [chr] id [%d] not support\n", chr_errno);
            return;
    }

    ps_print_info("[dcxo] [chr] id [%d] repot, report cnt %d\n", chr_errno, g_dcxo_info.chr_nv_read_err);
}

/*
 * 功能描述  : 从nv中读取gnss 参数
 * 输入参数  : pc_out 数据保存的地址
 *             nv_para nv信息
 */
OAL_STATIC  int32_t read_coeff_from_nvram(uint8_t *pc_out, nv_part_str *nv_para)
{
#ifdef HISI_NVRAM_SUPPORT
    struct external_nve_info_user info;
#endif
    int32_t ret = INI_FAILED;

    if (nv_para->part_id < 0) {  // 对于小于0的nv id为虚拟配置，目前用来配置dcxo默认值
        ret = memcpy_s(pc_out, MALLOC_LEN, g_default_coeff, nv_para->read_size);
        if (ret != EOK) {
            ps_print_err("read_coeff_from_nvram: memcpy_s failed, ret = %d", ret);
            return INI_FAILED;
        }
        return INI_SUCC;
    } else {
#ifdef HISI_NVRAM_SUPPORT
        memset_s(&info, sizeof(info), 0, sizeof(info));
        ret = memcpy_s(info.nv_name, sizeof(info.nv_name), nv_para->name, OAL_STRLEN(nv_para->name));
        if (ret != EOK) {
            ps_print_err("read_coeff_from_nvram: memcpy_s failed, ret = %d", ret);
            return INI_FAILED;
        }
        info.nv_operation = NVRAM_READ;
        info.valid_size = (uint32_t)nv_para->total_size;
        info.nv_number = (uint32_t)nv_para->part_id;
        ret = external_nve_direct_access_interface(&info);
        if (ret == INI_SUCC) {
            ret = memcpy_s(pc_out, MALLOC_LEN, info.nv_data, nv_para->read_size);
            if (ret != EOK) {
                ps_print_err("read_coeff_from_nvram: memcpy_s failed, ret = %d", ret);
                return INI_FAILED;
            }
            ps_print_info("[dcxo] read_conf_from_nvram::nvram id[%d], nv name[%s], size[%d]\r\n!",
                          info.nv_number, info.nv_name, info.valid_size);
        } else {
            ps_print_err("[dcxo] read nvm [%d] %s failed", info.nv_number, info.nv_name);
            return INI_FAILED;
        }
#else
        return INI_FAILED;
#endif
    }

    return INI_SUCC;
}

OAL_STATIC  int32_t nv_coeff_vld(int32_t *pl_coeff, uint32_t size, const char *nv_name)
{
    int32_t loop;
    int32_t l_invld_cnt = 0;
    for (loop = 0; loop < (int32_t)size; loop++) {
        if ((*pl_coeff < g_dcxo_info.check_value[loop][INIT_TYPE_MIN]) ||
            (*pl_coeff > g_dcxo_info.check_value[loop][INIT_TYPE_MAX])) {
            ps_print_err("[dcxo] get nv[%s] coeff [%d], value %d, check exceed min[%d] <-> max[%d] scope \n",
                         nv_name, loop, *pl_coeff, g_dcxo_info.check_value[loop][INIT_TYPE_MIN],
                         g_dcxo_info.check_value[loop][INIT_TYPE_MAX]);

            l_invld_cnt++;
        }
        ps_print_info("[dcxo] read nv[%s] nv a[%d] = Dec(%d) Hex(0x%x)\n", nv_name, loop, *pl_coeff, *pl_coeff);
        pl_coeff++;
    }

    if (l_invld_cnt > 0) {
        return INI_FAILED;
    }

    return INI_SUCC;
}

OAL_STATIC int32_t process_plat_cali(uint8_t *pc_buffer, dcxo_dl_para_stru *pst_dl_para)
{
    int32_t *pl_para = NULL;
    uint32_t pmu_type;
    int32_t ret;

    // 初始化成默认的三项式系数
    pl_para = pst_dl_para->coeff_para;
    ret = memcpy_s(pl_para, sizeof(pst_dl_para->coeff_para), pc_buffer, sizeof(g_default_coeff));
    if (ret != EOK) {
        ps_print_err("[dcxo] process_plat_cali, memcpy_s failed, ret = %d \n", ret);
        return INI_FAILED;
    }

    // 根据ini获取到当前的PMU 偏移地址
    pmu_type = g_dcxo_info.pmu_chip_type;
    ret = memcpy_s(&(pst_dl_para->pmu_addr), sizeof(dcxo_pmu_addr_stru),
                   &g_pmu_addr[pmu_type], sizeof(dcxo_pmu_addr_stru));
    if (ret != EOK) {
        ps_print_err("[dcxo] process_plat_cali, memcpy_s failed, ret = %d \n", ret);
        return INI_FAILED;
    }

    return INI_SUCC;
}

OAL_STATIC int32_t process_factory_cali(uint8_t *pc_buffer, dcxo_dl_para_stru *pst_dl_para)
{
    int32_t *pl_coeff = NULL;
    plat_chr_nv_rd_excep_stru chr_nv_rd;
    int32_t ret;

    ret = memcpy_s(&g_dcxo_factory, sizeof(dcxo_nv_info_stru), pc_buffer, sizeof(dcxo_nv_info_stru));
    if (ret != EOK) {
        ps_print_err("[dcxo] process_factory_cali: memcpy_s faild, ret = %d", ret);
        return INI_FAILED;
    }
    pl_coeff = g_dcxo_factory.coeff_para;

    if (g_dcxo_factory.valid != DCXO_NV_CHK_OK) {
        ps_print_info("[dcxo] read factory cali not valid\n");
        goto report_chr;
    }

    // 此处检测到工厂分区t0使用的是10位，需要修正到16位
    if (g_dcxo_factory.t0_bit_width_fix == T0_BIT_WIDTH_10) {
        pl_coeff[COEFF_T0] *= T0_WIDTH10_TO_WIDTH16;
    }

    /* 判断工厂分区读出的参数，是否在有效的min-max区间 */
    if (nv_coeff_vld(pl_coeff, COEFF_NUM, NV_FAC_NAME) == INI_FAILED) {
        ps_print_err("[dcxo] factory coeff data check failed\n");
        goto report_chr;
    }

    ret = memcpy_s(pst_dl_para->coeff_para, sizeof(pst_dl_para->coeff_para),
                   pl_coeff, COEFF_NUM * sizeof(int32_t));
    if (ret != EOK) {
        ps_print_err("[dcxo] process_factory_cali: memcpy_s faild, ret = %d", ret);
        goto report_chr;
    }

    return INI_SUCC;

report_chr:
    chr_nv_rd.l_struct_len = (int32_t)sizeof(plat_chr_nv_rd_excep_stru);
    ret = memcpy_s(chr_nv_rd.l_para, sizeof(chr_nv_rd.l_para), pl_coeff, COEFF_NUM * sizeof(int32_t));
    if (ret == EOK) {
        report_chr_err(PLAT_CHR_ERRNO_DCXO_NV_RD, (uint8_t *)&chr_nv_rd, sizeof(chr_nv_rd));
    }
    return INI_FAILED;
}

OAL_STATIC int32_t process_gnss_self_study(uint8_t *pc_buffer, dcxo_dl_para_stru *pst_dl_para)
{
    int32_t *pl_coeff = NULL;
    dcxo_nv_info_stru *pst_info;
    plat_chr_nv_rd_excep_stru chr_nv_rd;
    int32_t ret;

    pst_info = (dcxo_nv_info_stru *)pc_buffer;
    pl_coeff = pst_info->coeff_para;

    if (pst_info->valid != DCXO_NV_CHK_OK) {
        ps_print_info("[dcxo] gnss nv check  not valid \n");
        goto report_chr;
    }

    /* 判断gnss自学习分区读出的参数，是否在有效的min-max区间 */
    if (nv_coeff_vld(pl_coeff, COEFF_NUM, NV_GNSS_NAME) == INI_FAILED) {
        ps_print_err("[dcxo] factory coeff data check failed\n");
        goto report_chr;
    }

    ret = memcpy_s(pst_dl_para->coeff_para, sizeof(pst_dl_para->coeff_para),
                   pl_coeff, COEFF_NUM * sizeof(int32_t));
    if (ret != EOK) {
        ps_print_err("[dcxo] process_gnss_self_study, memcpy_s failed, ret = %d\n", ret);
        goto report_chr;
    }

    return INI_SUCC;

report_chr:
    memset_s(&chr_nv_rd, sizeof(plat_chr_nv_rd_excep_stru), 0, sizeof(plat_chr_nv_rd_excep_stru));
    chr_nv_rd.l_struct_len = (int32_t)sizeof(plat_chr_nv_rd_excep_stru);
    ret = memcpy_s(chr_nv_rd.l_para, sizeof(chr_nv_rd.l_para), pl_coeff,
                   COEFF_NUM * sizeof(int32_t));
    if (ret == EOK) {
        report_chr_err(PLAT_CHR_ERRNO_DCXO_NV_RD, (uint8_t *)&chr_nv_rd, sizeof(chr_nv_rd));
    }
    return INI_FAILED;
}

OAL_STATIC void get_dcxo_coeff(dcxo_dl_para_stru *pst_dl_para)
{
    uint8_t *buffer = NULL;
    int32_t ret = INI_FAILED;
    int32_t loop;
    int32_t nv_size;

    if (pst_dl_para == NULL) {
        ps_print_err("[dcxo] pst_dl_para input is NULL \n");
        return;
    }

    buffer = os_kzalloc_gfp(MALLOC_LEN);
    if (buffer == NULL) {
        ps_print_err("[dcxo] alloc coeff mem failed \n");
        return;
    }

    nv_size = (int32_t)oal_array_size(g_dcxo_nv_part);
    for (loop = 0; loop < nv_size; loop++) {
        if (g_dcxo_nv_part[loop].used == OAL_FALSE) {
            continue;
        }

        // 判断需要读取的buffer大小是否会越界
        if (g_dcxo_nv_part[loop].read_size > MALLOC_LEN) {
            ps_print_err("[dcxo] read nv %s size exceed malloc len\n",
                         g_dcxo_nv_part[loop].name);
            ret = INI_FAILED;
            continue;
        }

        // 读取nv中的数据
        ret = read_coeff_from_nvram(buffer, &g_dcxo_nv_part[loop]);
        if (ret == INI_FAILED) {
            memset_s(buffer, MALLOC_LEN, 0, MALLOC_LEN);
            continue;
        }

        // 对nv项内容进行处理
        if (g_dcxo_nv_part[loop].process_cali != NULL) {
            ret = g_dcxo_nv_part[loop].process_cali(buffer, pst_dl_para);
            if (ret != INI_SUCC) {
                memset_s(buffer, MALLOC_LEN, 0, MALLOC_LEN);
                continue;
            }

            // 记录成功写入到下载buffer中的nv号
            g_dcxo_info.use_part_id = g_dcxo_nv_part[loop].part_id;
        }
        memset_s(buffer, MALLOC_LEN, 0, MALLOC_LEN);
    }

    os_mem_kfree(buffer);
}

OAL_STATIC void show_dcxo_conf_info(dcxo_dl_para_stru *pst_dl_para)
{
    int32_t loop;
    nv_part_str *nv_part = NULL;
    dcxo_pmu_addr_stru *pmu_addr = NULL;

    nv_part = get_nv_part_from_id(g_dcxo_info.use_part_id);

    ps_print_info("[dcxo] check data = 0x%x use dcxo mode, nv id = %d, name = %s cali buffer len %u\n",
                  pst_dl_para->check_data, g_dcxo_info.use_part_id, (nv_part == NULL) ? NULL : nv_part->name,
                  (uint32_t)sizeof(dcxo_dl_para_stru));

    for (loop = 0; loop < COEFF_NUM; loop++) {
        ps_print_info("[dcxo] a[%d] = 0x%x \n", loop, pst_dl_para->coeff_para[loop]);
    }

    pmu_addr = &(pst_dl_para->pmu_addr);
    ps_print_info("[dcxo] pmu info  cali 0x%x, av0 0x%x av1 0x%x av2 0x%x av3 0x%x\n",
                  pmu_addr->cali_end, pmu_addr->auto_cali_ave0, pmu_addr->auto_cali_ave1,
                  pmu_addr->auto_cali_ave2, pmu_addr->auto_cali_ave3);

    ps_print_info("[dcxo] pmu info  ana 0x%x, state 0x%x low byte 0x%x high bytpe 0x%x type %d\n",
                  pmu_addr->wifi_ana_en, pmu_addr->xoadc_state, pmu_addr->reg_addr_low,
                  pmu_addr->reg_addr_high, pmu_addr->pmu_type);
}


 /*
  * 功能描述  : 当系统起来之后，动态更新内存中三项式系数a0~a3
  * 输入参数  : 传入需要更新参数的buffer地址
  */
void update_dcxo_coeff(int32_t *coeff, uint32_t coeff_cnt)
{
    dcxo_dl_para_stru *pst_para = NULL;
    int32_t ret;

    if (coeff == NULL) {
        return;
    }

    pst_para = get_dcxo_data_buf_addr();
    if (pst_para == NULL) {
        ps_print_err("[dcxo] update coeff input pst_para is NULL\n");
        return;
    }

    if (nv_coeff_vld(coeff, COEFF_NUM, "update_dcxo_coeff") != INI_SUCC) {
        ps_print_err("[dcxo] update coeff invlid\n");
        return;
    }

    ret = memcpy_s(pst_para->coeff_para, sizeof(pst_para->coeff_para), coeff, coeff_cnt);
    if (ret != EOK) {
        ps_print_err("[dcxo] memcpy_s failed, ret = %d\n", ret);
        return;
    }
}

uint8_t* board_clk_buffer_init(void)
{
    g_dcxo_data_buf = os_kzalloc_gfp(DCXO_DEVICE_MAX_BUF_LEN);
    if (g_dcxo_data_buf == NULL) {
        return NULL;
    }

    return g_dcxo_data_buf;
}

void board_clk_buffer_exit(void)
{
    if (g_dcxo_data_buf != NULL) {
        os_mem_kfree(g_dcxo_data_buf);
        g_dcxo_data_buf = NULL;
    }
}

void *get_dcxo_data_buf_addr(void)
{
    return g_dcxo_data_buf;
}

 /*
  * 功能描述  : 从ini文件中读取时钟模式
  * 返 回 值  : 当前使用的模式
  */
OAL_STATIC int32_t read_clk_para_from_ini_file(const char *label, uint32_t *mode, uint32_t *freq)
{
    int8_t ac_mode_buff[INI_BUF_SIZE] = {0};
    int8_t i;
    char *cur_mode = ac_mode_buff;
    char *token = NULL;

    if (get_cust_conf_string(INI_MODU_PLAT, label, ac_mode_buff, sizeof(ac_mode_buff)) != INI_SUCC) {
        ps_print_info("[dcxo] %s not support, skip\n", label);
        return INI_FAILED;
    }

    for (i = 0; i < oal_array_size(g_clk_mode_label); i++) {
        if (OAL_STRNCMP(ac_mode_buff, g_clk_mode_label[i].label,
                        OAL_STRLEN(g_clk_mode_label[i].label)) == 0) {
            *mode = g_clk_mode_label[i].label_value;
            break;
        }
    }

    if (i == oal_array_size(g_freq_label)) {
        return INI_FAILED;
    }

    if (*mode == CLK_MODE_ABSENT) {
        return INI_FAILED;
    }

    token = strsep(&cur_mode, ",");
    if (cur_mode == NULL) {
        ps_print_info("[dcxo] freq not set in ini, using dts config\n");
        return INI_SUCC;
    }

    for (i = 0; i < oal_array_size(g_freq_label); i++) {
        if (OAL_STRNCMP(cur_mode, g_freq_label[i].label,
                        OAL_STRLEN(g_freq_label[i].label)) == 0) {
            *freq = g_freq_label[i].label_value;
            break;
        }
    }

    if (i == oal_array_size(g_freq_label)) {
        ps_print_err("[dcxo] freq [%s] not recognize\n", cur_mode);
        return INI_FAILED;
    }

    return INI_SUCC;
}

STATIC void clk_comb_to_dword(void)
{
    int32_t i;
    for (i = 0; i < oal_array_size(g_clk_label); i++) {
        /* 4 为每个位域为4bit, freq+mode总共位域为8bit */
        g_dcxo_info.mode.as_dword |=  ((g_clk_label[i].freq << 4 | g_clk_label[i].mode) << (i * 8));
    }

    // 主路时钟频率不存在，不需要做初始化配置
    if (g_dcxo_info.mode.bits.main_clk_freq == CLK_NONE) {
        g_dcxo_info.clk_det = DET_NONE;
    }

    ps_print_info("[dcxo] clk mode [0x%x]\n", g_dcxo_info.mode.as_dword);
}

 /*
  * 功能描述  : 之前产品从dts获取时钟信息，由于不能存clk type的限制，后续改成从ini中获取
  */
#ifdef _PRE_CONFIG_USE_DTS
STATIC void main_clk_freq_init(struct device_node *np)
{
    int32_t ret;
    int32_t freq = 0;

    ret = of_property_read_u32(np, DTS_PROP_GPIO_TCXO_LEVEL, &freq);
    if (ret != INI_SUCC) {
        ps_print_err("get dts prop %s failed\n", DTS_PROP_GPIO_TCXO_LEVEL);
        return;
    }

    if (get_mpxx_subchip_type() == BOARD_VERSION_MP15) {
        g_dcxo_info.clk_io_level[CLK_38P4] = GPIO_LOWLEVEL;
        g_dcxo_info.clk_io_level[CLK_76P8] = GPIO_HIGHLEVEL;
        if (freq == 0) {
            g_clk_label[0].freq = CLK_38P4;
        } else {
            g_clk_label[0].freq = CLK_76P8;
        }
    } else {
        g_dcxo_info.clk_io_level[CLK_38P4] = GPIO_HIGHLEVEL;
        g_dcxo_info.clk_io_level[CLK_76P8] = GPIO_LOWLEVEL;
        if (freq == 0) {
            g_clk_label[0].freq = CLK_76P8;
        } else {
            g_clk_label[0].freq = CLK_38P4;
        }
    }

    ps_print_info("[dcxo] dts main clk freq id [%d]", g_clk_label[0].freq);
}

STATIC void get_main_clk_det_io(void)
{
    int32_t ret;
    int32_t physical_gpio = 0;

    ret = get_board_gpio(DTS_NODE_HISI_MPXX, DTS_PROP_MPXX_GPIO_TCXO_FREQ_DETECT, &physical_gpio);
    if (ret != INI_SUCC) {
        ps_print_err("get dts prop %s failed\n", DTS_PROP_MPXX_GPIO_TCXO_FREQ_DETECT);
        return;
    }

    g_dcxo_info.clk_io_nr = physical_gpio;

    ps_print_info("[dcxo] dts main clk io [%d]", g_dcxo_info.clk_io_nr);
}

STATIC void clk_init_from_dts(void)
{
    int32_t ret;
    struct device_node *np = NULL;

    // 初始化成非法值
    memset_s((uint8_t *)(g_dcxo_info.clk_io_level), sizeof(g_dcxo_info.clk_io_level),
             0XFF, sizeof(g_dcxo_info.clk_io_level));

    ret = get_board_dts_node(&np, DTS_NODE_HISI_MPXX);
    if (ret != INI_SUCC) {
        ps_print_err("DTS read node %s fail!!!\n", DTS_NODE_HISI_MPXX);
        return;
    }

    ret = of_property_read_bool(np, DTS_PROP_MPXX_POWER_PREPARE);
    if (ret == 0) {
        ps_print_info("no need prepare before board power on\n");
        return;
    }
    main_clk_freq_init(np);
    get_main_clk_det_io();
}
#endif

STATIC void clk_init_from_ini(void)
{
    int32_t ret, i;
    plat_clk_mode_str *clk_mode;
    uint32_t mode;
    uint32_t freq;

    clk_mode = &g_dcxo_info.mode;
    for (i = 0; i < oal_array_size(g_clk_label); i++) {
        /* 预留未使用 */
        if (g_clk_label[i].label_name == NULL) {
            continue;
        }

        /* ini中找不到表明暂不支持 */
        mode = g_clk_label[i].mode;
        freq =  g_clk_label[i].freq;
        ret = read_clk_para_from_ini_file(g_clk_label[i].label_name, &mode, &freq);
        if (ret == INI_SUCC) {
            g_clk_label[i].mode = mode;
            g_clk_label[i].freq = freq;
            continue;
        }
    }
}

 /*
  * 功能描述  : 初始化时钟配置
  */
void board_clk_init(clk_det_mode det_mode, clk_init_enum init_mode)
{
    g_dcxo_info.clk_det = det_mode;

    /* 单时钟类型的产品 */
    if (init_mode == CLK_INIT_NONE) {
        // 产品仅支持tcxo 38.4M
        g_dcxo_info.mode.bits.main_clk_freq = CLK_38P4;
        g_dcxo_info.mode.bits.main_type = CLK_MODE_TCXO;
        return;
    }
#ifdef _PRE_CONFIG_USE_DTS
    if ((init_mode & CLK_INIT_DTS) == CLK_INIT_DTS) {
        clk_init_from_dts();
    }
#endif
    if ((init_mode & CLK_INIT_INI) == CLK_INIT_INI) {
        clk_init_from_ini();
    }
    clk_comb_to_dword();
}

void board_clk_exit(void)
{
    board_clk_buffer_exit();
}

clk_freq_enum board_main_clk_freq(void)
{
    return g_dcxo_info.mode.bits.main_clk_freq;
}
EXPORT_SYMBOL(board_main_clk_freq);

clk_mode_enum board_clk_mode(void)
{
    return g_dcxo_info.mode.as_dword;
}

 /*
  * 功能描述  : 读取共时钟的校准参数
  */
int32_t read_dcxo_cali_data(void)
{
    dcxo_dl_para_stru *pst_dl_para = NULL;

    if (test_bit(DCXO_PARA_READ_OK, &g_dcxo_info.nv_init_flag)) {
        return INI_SUCC;
    }

    pst_dl_para = (dcxo_dl_para_stru *)board_clk_buffer_init();
    if (pst_dl_para == NULL) {
        ps_print_err("[dcxo][read_dcxo_cali_data] pst_para is NULL\n");
        return INI_FAILED;
    }

    pst_dl_para->check_data = DCXO_CHECK_MAGIC;
    pst_dl_para->dcxo_tcxo_mode = g_dcxo_info.mode.as_dword;

    // 主路时钟为共时钟，读取相关校准参数
    if (g_dcxo_info.mode.bits.main_type == CLK_MODE_DCXO) {
        read_min_max_val_from_ini_file();
        read_pmu_type_from_ini_file();
        get_dcxo_coeff(pst_dl_para);
        show_dcxo_conf_info(pst_dl_para);
    }

    set_bit(DCXO_PARA_READ_OK, &g_dcxo_info.nv_init_flag);
    return INI_SUCC;
}

STATIC int32_t main_clk_prepare_det_by_uart(clk_prepare_stage init_stage, clk_freq_enum freq)
{
    int32_t ret;

    if ((g_dcxo_info.clk_io_level)[freq] == 0xFF) {
        ps_print_err("main clk io from dts is invalid, freq type[%d]\n", freq);
        return INI_FAILED;
    }

    if (init_stage == STAGE_POWER_PRE) {
        ps_print_info("nr %d level %d freq %d\n", g_dcxo_info.clk_io_nr, (g_dcxo_info.clk_io_level)[freq], freq);
        ret = gpio_request_one(g_dcxo_info.clk_io_nr, GPIOF_OUT_INIT_LOW, PROC_NAME_MPXX_GPIO_TCXO_FREQ);
        if (ret != INI_SUCC) {
            ps_print_err("%s [%d] gpio_request failed\n", PROC_NAME_MPXX_GPIO_TCXO_FREQ, g_dcxo_info.clk_io_nr);
            return INI_FAILED;
        }

        gpio_direction_output(g_dcxo_info.clk_io_nr, (g_dcxo_info.clk_io_level)[freq]);
    } else {
        oal_gpio_free(g_dcxo_info.clk_io_nr);
    }

    return INI_SUCC;
}

STATIC int32_t main_clk_prepare_det_by_wkup_io(clk_prepare_stage init_stage, clk_freq_enum freq, clk_mode_enum clk_type)
{
    int32_t freq_nr, type_nr;

    freq_nr = conn_get_gpio_number_by_type(BFGX_WKUP_HOST);
    type_nr = conn_get_gpio_number_by_type(W_WKUP_HOST);
    if (freq_nr < 0 || type_nr < 0) {
        ps_print_err("wkup io invalid freq %d, type %d\n", freq_nr, type_nr);
        return INI_FAILED;
    }

    if (init_stage == STAGE_POWER_PRE) {
        conn_irq_save_all();
        if (freq == CLK_76P8) {
            gpio_direction_output(freq_nr, 1);
        } else {
            gpio_direction_output(freq_nr, 0);
        }

        if (clk_type == CLK_MODE_XO) {
            gpio_direction_output(type_nr, 1);
        } else {
            gpio_direction_output(type_nr, 0);
        }
    } else {
        conn_set_gpio_input(BFGX_WKUP_HOST);
        conn_set_gpio_input(W_WKUP_HOST);
        conn_irq_restore_all();
    }

    return INI_SUCC;
}

STATIC int32_t get_main_clk_para(clk_mode_enum *type, clk_freq_enum *freq)
{
    dcxo_dl_para_stru *pst_dl_para = NULL;
    plat_clk_mode_str mode;
    clk_mode_enum clk_type;
    clk_freq_enum clk_freq;

    // gugong等场景下，会更改主路时钟，这里需要从dl buffer中重新获取一次时钟
    pst_dl_para = (dcxo_dl_para_stru *)get_dcxo_data_buf_addr();
    if (pst_dl_para != NULL) {
        mode.as_dword = pst_dl_para->dcxo_tcxo_mode;
        clk_freq = mode.bits.main_clk_freq;
        clk_type = mode.bits.main_type;

        if (clk_freq != g_dcxo_info.mode.bits.main_clk_freq) {
            ps_print_info("main clk freq changed outside, cur[%d], init[%d]\n",
                          clk_freq, g_dcxo_info.mode.bits.main_clk_freq);
        }

        if (clk_type != g_dcxo_info.mode.bits.main_type) {
            ps_print_info("main clk mode changed outside, cur[%d], init[%d]\n",
                          clk_type, g_dcxo_info.mode.bits.main_type);
        }
    } else {
        clk_freq = g_dcxo_info.mode.bits.main_clk_freq;
        clk_type = g_dcxo_info.mode.bits.main_type;
    }

    *type = clk_type;
    *freq = clk_freq;
    return INI_SUCC;
}

int32_t main_clk_prepare_with_power_on(clk_prepare_stage init_stage)
{
    clk_mode_enum type = CLK_MODE_ABSENT;
    clk_freq_enum freq = CLK_NONE;

    if (g_dcxo_info.clk_det == DET_NONE) {
        return INI_SUCC;
    }

    if (get_main_clk_para(&type, &freq) != INI_SUCC) {
        return INI_FAILED;
    }

    if (g_dcxo_info.clk_det == DET_BY_UART) {
        return main_clk_prepare_det_by_uart(init_stage, freq);
    } else if (g_dcxo_info.clk_det == DET_BY_WKUP_IO) {
        return main_clk_prepare_det_by_wkup_io(init_stage, freq, type);
    }
    return INI_SUCC;
}