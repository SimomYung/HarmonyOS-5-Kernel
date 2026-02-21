#ifndef __SOC_SPEC_INFO_H__
#define __SOC_SPEC_INFO_H__

#include <mntn_public_interface.h>
#include <soc_lpmcu_baseaddr_interface.h>

#if defined(CONFIG_SOC_PG_INFO_IN_LPRAM)
#define SOC_PG_INFO_ADDR 0x70000
#define SOC_PG_INFO_ADDR_IN_DDR 0x10969000
#elif defined(CONFIG_SOC_PG_INFO_IN_SC)
#define SOC_PG_INFO_ADDR 0xD00F2000
#define SOC_PG_INFO_ADDR_IN_DDR 0x10969000
#elif defined(CONFIG_SOC_PG_INFO_IN_DDR)
#define SOC_PG_INFO_ADDR 0x10969000
#elif defined(CONFIG_SOC_PG_INFO)
#define SOC_PG_INFO_ADDR 0x10969000
#endif

#define SOC_PG_INFO_SIZE 0x1000
typedef union
{
	unsigned int value;
	struct
	{
		unsigned int reserved : 19;             /* bit[0-18]  */
		unsigned int chip_ec_version : 2;       /* bit[19-20] */
		unsigned int chip_version : 3;          /* bit[21-23] */
		unsigned int soc_spec_info : 4;         /* bit[24-27] */
		unsigned int magic : 4;                 /* bit[28-31] */
	} reg;
} SELF_SCTRL_SPEC_UNION;

typedef enum chip_bin_version {
	BIN1_VERSION,
	BINX2_VERSION,
	MAX_VERSION,
	INVALID_VERSION,
} CHIP_BIN_VERSION;

typedef enum chip_ec_version {
	CHIP_NO_EC,
	CHIP_EC,
	CHIP_MAX_VERSION,
	CHIP_INVALID_VERSION,
} CHIP_EC_VERSION;

#define SOC_SPEC_INFO_MAGIC 0xA

#define SCTRL_SPEC_INFO(core_type) \
		(*(volatile SELF_SCTRL_SPEC_UNION*)(SOC_SCTRL_SCBAKDATA12_ADDR(SOC_##core_type##_SCTRL_BASE_ADDR)))

#define get_soc_chip_version(core_type) \
		(unsigned int)(((SCTRL_SPEC_INFO(core_type).reg.magic == SOC_SPEC_INFO_MAGIC) ? \
		SCTRL_SPEC_INFO(core_type).reg.chip_version : INVALID_VERSION))

#define get_soc_chip_ec_version(core_type) \
		(SCTRL_SPEC_INFO(core_type).reg.chip_ec_version)

static inline unsigned int get_lpmcu_chip_version()
{
	return get_soc_chip_version(LPMCU);
}

static inline unsigned int get_acpu_chip_version()
{
	return get_soc_chip_version(ACPU);
}

static inline enum chip_ec_version get_lpmcu_chip_ec_version()
{
	return get_soc_chip_ec_version(LPMCU);
}

static inline enum chip_ec_version get_acpu_chip_ec_version()
{
	return get_soc_chip_ec_version(ACPU);
}

// pg
typedef enum soc_spec_level {
	SOC_SPEC_LEVEL1_EFUSE_NONE = 0,
	SOC_SPEC_LEVEL1,
	SOC_SPEC_LITE_PHONE,
	SOC_SPEC_LITE2_PHONE,
	SOC_SPEC_WIFI,
	SOC_SPEC_WIFI_LITE,
	SOC_SPEC_PHONE_TURBO,
	SOC_SPEC_PHONE_TURBO_TO_LEVEL1,
    SOC_SPEC_PHONE_POWER_P1_LEVEL1,
    SOC_SPEC_PHONE_POWER_P2_LEVEL1,
	SOC_SPEC_INVALID,
	SOC_SPEC_MAX
} SOC_SPEC_LEVEL;

enum soc_component_pg_info_index {
	SOC_COMPONENT_INFO_START     = 0,
	SOC_COMPONENT_CHIP_TYPE_INFO = SOC_COMPONENT_INFO_START,
	SOC_COMPONENT_MODEM_INFO,
	SOC_COMPONENT_PROFILE_STRATEGY_INFO,
	SOC_COMPONENT_PG_TEST_VERSION_INFO,
	SOC_COMPONENT_CPUB_CORE_INFO,
	SOC_COMPONENT_CPUM_CORE_INFO,
	SOC_COMPONENT_CPUL_CORE_INFO,
	SOC_COMPONENT_HC_CAPACITY_INFO,
	SOC_COMPONENT_CPUB_FREQ_INFO,
	SOC_COMPONENT_CPUM_FREQ_INFO,
	SOC_COMPONENT_CPUL_FREQ_INFO,
	SOC_COMPONENT_GPU_CORE_INFO,
	SOC_COMPONENT_NPU_INFO,
	SOC_COMPONENT_SC_CAPACITY_INFO,
	SOC_COMPONENT_SC_SF_CAPACITY_INFO,
	SOC_COMPONENT_MIPI_FREQ_INFO,
	SOC_COMPONENT_DDR_FREQ_INFO,
	SOC_COMPONENT_UFS_INFO,
	SOC_COMPONENT_MEDIA2_ISP_INFO,
	SOC_COMPONENT_VENC_INFO,
	SOC_COMPONENT_VDEC_INFO,
	SOC_COMPONENT_DSS_INFO,
	SOC_COMPONENT_AUDIO_INFO,
	SOC_COMPONENT_IOMCU_INFO,
	SOC_COMPONENT_PCIE_INFO,
	SOC_COMPONENT_INFO_END,
	SOC_COMPONENT_INFO_MAX = 512,  // 4k / sizeof(struct soc_component_pg_info) = 512
};

#endif
