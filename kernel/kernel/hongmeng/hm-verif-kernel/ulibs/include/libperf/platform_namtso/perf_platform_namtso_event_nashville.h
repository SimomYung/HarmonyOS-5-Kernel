/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: add Namtso PMU list for nashville platform
 * Author: HISI_DRV
 * Create: Sat May 18 10:30:43 2024
 */

#ifdef ULIBS_LIBPERF_PLATFORM_NAMTSO_DEF
#define DSU_PMU_CHAIN						0x001e
#define DSU_PMU_BUS_ACCESS_RD				0x0060
#define DSU_PMU_BUS_ACCESS_WR				0x0061
#define DSU_PMU_BUS_ACCESS_SHARED			0x0062
#define DSU_PMU_BUS_ACCESS_NOTSHARED		0x0063
#define DSU_PMU_BUS_ACCESS_NORMAL			0x0064
#define DSU_PMU_BUS_ACCESS_PERIPH			0x0065

#define DSU_PMU_L3D_CACHE_RD				0x00a0
#define DSU_PMU_L3D_CACHE_WR				0x00a1
#define DSU_PMU_L3D_CACHE_REFILL_RD			0x00a2
#define DSU_PMU_L3D_CACHE_REFILL_WR			0x00a3

/* architech defined */
#define DSU_PMU_L3D_CACHE_HIT				0x00c5
#define DSU_PMU_L3D_CACHE_MISS				0x00c6
#define DSU_PMU_MPAM_CONFLICT				0x00c7

/* ACP */
#define DSU_PMU_ACP_CYCLES					0x011d
#define DSU_PMU_ACP_ACCESS					0x0119
#define DSU_PMU_ACP_ACCESS_RD				0x0160
#define DSU_PMU_ACP_ACCESS_WR				0x0161
/* stach */
#define DSU_PMU_STASH_CPU_ACCESS			0x0300
#define DSU_PMU_STASH_CPU_MISS				0x0301
#define DSU_PMU_STASH_CPU_HIT				0x0302
#define DSU_PMU_STASH_CPU_MERGE				0x0303
#define DSU_PMU_STASH_CPU_DROP				0x0305
#define DSU_PMU_STASH_HWP_ACCESS			0x0330
#define DSU_PMU_STASH_HWP_MISS				0x0331
#define DSU_PMU_STASH_HWP_HIT				0x0332
#define DSU_PMU_STAH_HWP_MERGE				0x0333
#define DSU_PMU_STASH_HWP_DROP				0x0335
/* msps */
#define DSU_PMU_MSPS_EVENT0					0x0336
#define DSU_PMU_MSPS_EVENT1					0x0337
#define DSU_PMU_MSPS_EVENT2					0x0338
#define DSU_PMU_MSPS_EVENT3					0x0339
#define DSU_PMU_MSPS_EVENT4					0x033a
#define DSU_PMU_MSPS_EVENT5					0x033b
#define DSU_PMU_MSPS_EVENT6					0x033c
#define DSU_PMU_WATERLINE_ADD				0x033d
#define DSU_PMU_WATERLINE_SUB				0x033e
/* rtm */
#define DSU_PMU_SNP_REQ_NO_CPU_SNP			0x0340
#define DSU_PMU_SNP_REQ_CPU_SNP				0x0341
#define DSU_PMU_SNP_CPUB_TO_CPUM			0x0342
#define DSU_PMU_SNP_CPUM_TO_CPUB			0x0344
#define DSU_PMU_SNP_CPUB_TO_CPUB			0x0348
#define DSU_PMU_SNP_CPUM_TO_CPUM			0x0349
/* others */
#define DSU_PMU_SCU_HZD_ADDRESS				0x0402
#define DSU_PMU_SCU_BIB_ACCESS				0x0406
#define DSU_PMU_SCU_SAFE_MODE_CYCLES		0x0408
#define DSU_PMU_CFRQ_ENQ_STALL				0x0410
#define DSU_PMU_RXDAT_STALL					0x0412
#define DSU_PMU_CFRQ_LOW_OCC				0x0420
#define DSU_PMU_RXSNPSHARED_B0				0x0430
#define DSU_PMU_RXSNPSHARED_M0				0x0434
#define DSU_PMU_RXSNPSHARED_M1				0x0435
#define DSU_PMU_RXSNPSHARED_M2				0x0436
#define DSU_PMU_RXSNPSHARED_M3				0x0437
#define DSU_PMU_RXSNPUNIQUE_B0				0x0440
#define DSU_PMU_RXSNPUNIQUE_M0				0x0444
#define DSU_PMU_RXSNPUNIQUE_M1				0x0445
#define DSU_PMU_RXSNPUNIQUE_M2				0x0446
#define DSU_PMU_RXSNPUNIQUE_M3				0x0447

#endif /* ULIBS_LIBPERF_PLATFORM_NAMTSO_DEF */

#ifdef PERF_PLATFORM_NAMTSO_ENUM
	HM_HW_DSU_CHAIN,
	HM_HW_DSU_BUS_ACCESS_RD,
	HM_HW_DSU_BUS_ACCESS_WR,
	HM_HW_DSU_BUS_ACCESS_SHARED,
	HM_HW_DSU_BUS_ACCESS_NOTSHARED,
	HM_HW_DSU_BUS_ACCESS_NORMAL,
	HM_HW_DSU_BUS_ACCESS_PERIPH,
	HM_HW_DSU_L3D_CACHE_RD,
	HM_HW_DSU_L3D_CACHE_WR,
	HM_HW_DSU_L3D_CACHE_REFILL_RD,
	HM_HW_DSU_L3D_CACHE_REFILL_WR,

	/* architech defined */
	HM_HW_DSU_L3D_CACHE_HIT,
	HM_HW_DSU_L3D_CACHE_MISS,
	HM_HW_DSU_MPAM_CONFLICT,

	/* ACP */
	HM_HW_DSU_ACP_CYCLES,
	HM_HW_DSU_ACP_ACCESS,
	HM_HW_DSU_ACP_ACCESS_RD,
	HM_HW_DSU_ACP_ACCESS_WR,
	/* stach */
	HM_HW_DSU_STASH_CPU_ACCESS,
	HM_HW_DSU_STASH_CPU_MISS,
	HM_HW_DSU_STASH_CPU_HIT,
	HM_HW_DSU_STASH_CPU_MERGE,
	HM_HW_DSU_STASH_CPU_DROP,
	HM_HW_DSU_STASH_HWP_ACCESS,
	HM_HW_DSU_STASH_HWP_MISS,
	HM_HW_DSU_STASH_HWP_HIT,
	HM_HW_DSU_STAH_HWP_MERGE,
	HM_HW_DSU_STASH_HWP_DROP,
	/* msps */
	HM_HW_DSU_MSPS_EVENT0,
	HM_HW_DSU_MSPS_EVENT1,
	HM_HW_DSU_MSPS_EVENT2,
	HM_HW_DSU_MSPS_EVENT3,
	HM_HW_DSU_MSPS_EVENT4,
	HM_HW_DSU_MSPS_EVENT5,
	HM_HW_DSU_MSPS_EVENT6,
	HM_HW_DSU_WATERLINE_ADD,
	HM_HW_DSU_WATERLINE_SUB,
	/* rtm */
	HM_HW_DSU_SNP_REQ_NO_CPU_SNP,
	HM_HW_DSU_SNP_REQ_CPU_SNP,
	HM_HW_DSU_SNP_CPUB_TO_CPUM,
	HM_HW_DSU_SNP_CPUM_TO_CPUB,
	HM_HW_DSU_SNP_CPUB_TO_CPUB,
	HM_HW_DSU_SNP_CPUM_TO_CPUM,
	/* others */
	HM_HW_DSU_SCU_HZD_ADDRESS,
	HM_HW_DSU_SCU_BIB_ACCESS,
	HM_HW_DSU_SCU_SAFE_MODE_CYCLES,
	HM_HW_DSU_CFRQ_ENQ_STALL,
	HM_HW_DSU_RXDAT_STALL,
	HM_HW_DSU_CFRQ_LOW_OCC,
	HM_HW_DSU_RXSNPSHARED_B0,
	HM_HW_DSU_RXSNPSHARED_M0,
	HM_HW_DSU_RXSNPSHARED_M1,
	HM_HW_DSU_RXSNPSHARED_M2,
	HM_HW_DSU_RXSNPSHARED_M3,
	HM_HW_DSU_RXSNPUNIQUE_B0,
	HM_HW_DSU_RXSNPUNIQUE_M0,
	HM_HW_DSU_RXSNPUNIQUE_M1,
	HM_HW_DSU_RXSNPUNIQUE_M2,
	HM_HW_DSU_RXSNPUNIQUE_M3,
#endif /* PERF_PLATFORM_NAMTSO_ENUM */

#ifdef PERF_PLATFORM_NAMTSO_SYMBOL
	{ chw(DSU_CHAIN),						"dsu-chain",						13	},
	{ chw(DSU_BUS_ACCESS_RD),				"dsu-bus-access-rd",				21	},
	{ chw(DSU_BUS_ACCESS_WR),				"dsu-bus-access-wr",				21	},
	{ chw(DSU_BUS_ACCESS_SHARED),			"dsu-bus-access-shared",			25	},
	{ chw(DSU_BUS_ACCESS_NOTSHARED),		"dsu-bus-access-notshared",			28	},
	{ chw(DSU_BUS_ACCESS_NORMAL),			"dsu-bus-access-normal",			25	},
	{ chw(DSU_BUS_ACCESS_PERIPH),			"dsu-bus-access-periph",			25	},
	{ chw(DSU_L3D_CACHE_RD),				"dsu-l3d-cache-rd",					20	},
	{ chw(DSU_L3D_CACHE_WR),				"dsu-l3d-cache-wr",					20	},
	{ chw(DSU_L3D_CACHE_REFILL_RD),			"dsu-l3d-cache-refill-rd",			27	},
	{ chw(DSU_L3D_CACHE_REFILL_WR),			"dsu-l3d-cache-refill-wr",			27	},

	/* architech defined */
	{ chw(DSU_L3D_CACHE_HIT),				"dsu-l3d-cache-hit",				21	},
	{ chw(DSU_L3D_CACHE_MISS),				"dsu-l3d-cache-miss",				22	},
	{ chw(DSU_MPAM_CONFLICT),				"dsu-mpam-conflict",				21	},

	/* ACP */
	{ chw(DSU_ACP_CYCLES),					"dsu-acp-cycles",					18	},
	{ chw(DSU_ACP_ACCESS),					"dsu-acp-access",					18	},
	{ chw(DSU_ACP_ACCESS_RD),				"dsu-acp-access-rd",				21	},
	{ chw(DSU_ACP_ACCESS_WR),				"dsu-acp-access-wr",				21	},
	/* stach */
	{ chw(DSU_STASH_CPU_ACCESS),			"dsu-stash-cpu-access",				24	},
	{ chw(DSU_STASH_CPU_MISS),				"dsu-stash-cpu-miss",				22	},
	{ chw(DSU_STASH_CPU_HIT),				"dsu-stash-cpu-hit",				21	},
	{ chw(DSU_STASH_CPU_MERGE),				"dsu-stash-cpu-merge",				23	},
	{ chw(DSU_STASH_CPU_DROP),				"dsu-stash-cpu-drop",				22	},
	{ chw(DSU_STASH_HWP_ACCESS),			"dsu-stash-hwp-access",				24	},
	{ chw(DSU_STASH_HWP_MISS),				"dsu-stash-hwp-miss",				22	},
	{ chw(DSU_STASH_HWP_HIT),				"dsu-stash-hwp-hit",				21	},
	{ chw(DSU_STAH_HWP_MERGE),				"dsu-stah-hwp-merge",				22	},
	{ chw(DSU_STASH_HWP_DROP),				"dsu-stash-hwp-drop",				22	},
	/* msps */
	{ chw(DSU_MSPS_EVENT0),					"dsu-msps-event0",					19	},
	{ chw(DSU_MSPS_EVENT1),					"dsu-msps-event1",					19	},
	{ chw(DSU_MSPS_EVENT2),					"dsu-msps-event2",					19	},
	{ chw(DSU_MSPS_EVENT3),					"dsu-msps-event3",					19	},
	{ chw(DSU_MSPS_EVENT4),					"dsu-msps-event4",					19	},
	{ chw(DSU_MSPS_EVENT5),					"dsu-msps-event5",					19	},
	{ chw(DSU_MSPS_EVENT6),					"dsu-msps-event6",					19	},
	{ chw(DSU_WATERLINE_ADD),				"dsu-waterline-add",				21	},
	{ chw(DSU_WATERLINE_SUB),				"dsu-waterline-sub",				21	},
	/* rtm */
	{ chw(DSU_SNP_REQ_NO_CPU_SNP),			"dsu-snp-req-no-cpu-snp",			26	},
	{ chw(DSU_SNP_REQ_CPU_SNP),				"dsu-snp-req-cpu-snp",				23	},
	{ chw(DSU_SNP_CPUB_TO_CPUM),			"dsu-snp-cpub-to-cpum",				24	},
	{ chw(DSU_SNP_CPUM_TO_CPUB),			"dsu-snp-cpum-to-cpub",				24	},
	{ chw(DSU_SNP_CPUB_TO_CPUB),			"dsu-snp-cpub-to-cpub",				24	},
	{ chw(DSU_SNP_CPUM_TO_CPUM),			"dsu-snp-cpum-to-cpum",				24	},
	/* others */
	{ chw(DSU_SCU_HZD_ADDRESS),				"dsu-scu-hzd-address",				23	},
	{ chw(DSU_SCU_BIB_ACCESS),				"dsu-scu-bib-access",				22	},
	{ chw(DSU_SCU_SAFE_MODE_CYCLES),		"dsu-scu-safe-mode-cycles",			28	},
	{ chw(DSU_CFRQ_ENQ_STALL),				"dsu-cfrq-enq-stall",				22	},
	{ chw(DSU_RXDAT_STALL),					"dsu-rxdat-stall",					19	},
	{ chw(DSU_CFRQ_LOW_OCC),				"dsu-cfrq-low-occ",					20	},
	{ chw(DSU_RXSNPSHARED_B0),				"dsu-rxsnpshared-b0",				22	},
	{ chw(DSU_RXSNPSHARED_M0),				"dsu-rxsnpshared-m0",				22	},
	{ chw(DSU_RXSNPSHARED_M1),				"dsu-rxsnpshared-m1",				22	},
	{ chw(DSU_RXSNPSHARED_M2),				"dsu-rxsnpshared-m2",				22	},
	{ chw(DSU_RXSNPSHARED_M3),				"dsu-rxsnpshared-m3",				22	},
	{ chw(DSU_RXSNPUNIQUE_B0),				"dsu-rxsnpunique-b0",				22	},
	{ chw(DSU_RXSNPUNIQUE_M0),				"dsu-rxsnpunique-m0",				22	},
	{ chw(DSU_RXSNPUNIQUE_M1),				"dsu-rxsnpunique-m1",				22	},
	{ chw(DSU_RXSNPUNIQUE_M2),				"dsu-rxsnpunique-m2",				22	},
	{ chw(DSU_RXSNPUNIQUE_M3),				"dsu-rxsnpunique-m3",				22	},
#endif /* PERF_PLATFORM_NAMTSO_SYMBOL */

#ifdef PERF_PLATFORM_NAMTSO_MAP
	[dsu_index(HM_HW_DSU_CHAIN)]						= DSU_PMU_CHAIN,
	[dsu_index(HM_HW_DSU_BUS_ACCESS_RD)]				= DSU_PMU_BUS_ACCESS_RD,
	[dsu_index(HM_HW_DSU_BUS_ACCESS_WR)]				= DSU_PMU_BUS_ACCESS_WR,
	[dsu_index(HM_HW_DSU_BUS_ACCESS_SHARED)]			= DSU_PMU_BUS_ACCESS_SHARED,
	[dsu_index(HM_HW_DSU_BUS_ACCESS_NOTSHARED)]			= DSU_PMU_BUS_ACCESS_NOTSHARED,
	[dsu_index(HM_HW_DSU_BUS_ACCESS_NORMAL)]			= DSU_PMU_BUS_ACCESS_NORMAL,
	[dsu_index(HM_HW_DSU_BUS_ACCESS_PERIPH)]			= DSU_PMU_BUS_ACCESS_PERIPH,
	[dsu_index(HM_HW_DSU_L3D_CACHE_RD)]					= DSU_PMU_L3D_CACHE_RD,
	[dsu_index(HM_HW_DSU_L3D_CACHE_WR)]					= DSU_PMU_L3D_CACHE_WR,
	[dsu_index(HM_HW_DSU_L3D_CACHE_REFILL_RD)]			= DSU_PMU_L3D_CACHE_REFILL_RD,
	[dsu_index(HM_HW_DSU_L3D_CACHE_REFILL_WR)]			= DSU_PMU_L3D_CACHE_REFILL_WR,

	/* ARCHITECH DEFINED */
	[dsu_index(HM_HW_DSU_L3D_CACHE_HIT)]				= DSU_PMU_L3D_CACHE_HIT,
	[dsu_index(HM_HW_DSU_L3D_CACHE_MISS)]				= DSU_PMU_L3D_CACHE_MISS,
	[dsu_index(HM_HW_DSU_MPAM_CONFLICT)]				= DSU_PMU_MPAM_CONFLICT,

	/* ACP */
	[dsu_index(HM_HW_DSU_ACP_CYCLES)]					= DSU_PMU_ACP_CYCLES,
	[dsu_index(HM_HW_DSU_ACP_ACCESS)]					= DSU_PMU_ACP_ACCESS,
	[dsu_index(HM_HW_DSU_ACP_ACCESS_RD)]				= DSU_PMU_ACP_ACCESS_RD,
	[dsu_index(HM_HW_DSU_ACP_ACCESS_WR)]				= DSU_PMU_ACP_ACCESS_WR,
	/* STACH */
	[dsu_index(HM_HW_DSU_STASH_CPU_ACCESS)]				= DSU_PMU_STASH_CPU_ACCESS,
	[dsu_index(HM_HW_DSU_STASH_CPU_MISS)]				= DSU_PMU_STASH_CPU_MISS,
	[dsu_index(HM_HW_DSU_STASH_CPU_HIT)]				= DSU_PMU_STASH_CPU_HIT,
	[dsu_index(HM_HW_DSU_STASH_CPU_MERGE)]				= DSU_PMU_STASH_CPU_MERGE,
	[dsu_index(HM_HW_DSU_STASH_CPU_DROP)]				= DSU_PMU_STASH_CPU_DROP,
	[dsu_index(HM_HW_DSU_STASH_HWP_ACCESS)]				= DSU_PMU_STASH_HWP_ACCESS,
	[dsu_index(HM_HW_DSU_STASH_HWP_MISS)]				= DSU_PMU_STASH_HWP_MISS,
	[dsu_index(HM_HW_DSU_STASH_HWP_HIT)]				= DSU_PMU_STASH_HWP_HIT,
	[dsu_index(HM_HW_DSU_STAH_HWP_MERGE)]				= DSU_PMU_STAH_HWP_MERGE,
	[dsu_index(HM_HW_DSU_STASH_HWP_DROP)]				= DSU_PMU_STASH_HWP_DROP,
	/* MSPS */
	[dsu_index(HM_HW_DSU_MSPS_EVENT0)]					= DSU_PMU_MSPS_EVENT0,
	[dsu_index(HM_HW_DSU_MSPS_EVENT1)]					= DSU_PMU_MSPS_EVENT1,
	[dsu_index(HM_HW_DSU_MSPS_EVENT2)]					= DSU_PMU_MSPS_EVENT2,
	[dsu_index(HM_HW_DSU_MSPS_EVENT3)]					= DSU_PMU_MSPS_EVENT3,
	[dsu_index(HM_HW_DSU_MSPS_EVENT4)]					= DSU_PMU_MSPS_EVENT4,
	[dsu_index(HM_HW_DSU_MSPS_EVENT5)]					= DSU_PMU_MSPS_EVENT5,
	[dsu_index(HM_HW_DSU_MSPS_EVENT6)]					= DSU_PMU_MSPS_EVENT6,
	[dsu_index(HM_HW_DSU_WATERLINE_ADD)]				= DSU_PMU_WATERLINE_ADD,
	[dsu_index(HM_HW_DSU_WATERLINE_SUB)]				= DSU_PMU_WATERLINE_SUB,
	/* RTM */
	[dsu_index(HM_HW_DSU_SNP_REQ_NO_CPU_SNP)]			= DSU_PMU_SNP_REQ_NO_CPU_SNP,
	[dsu_index(HM_HW_DSU_SNP_REQ_CPU_SNP)]				= DSU_PMU_SNP_REQ_CPU_SNP,
	[dsu_index(HM_HW_DSU_SNP_CPUB_TO_CPUM)]				= DSU_PMU_SNP_CPUB_TO_CPUM,
	[dsu_index(HM_HW_DSU_SNP_CPUM_TO_CPUB)]				= DSU_PMU_SNP_CPUM_TO_CPUB,
	[dsu_index(HM_HW_DSU_SNP_CPUB_TO_CPUB)]				= DSU_PMU_SNP_CPUB_TO_CPUB,
	[dsu_index(HM_HW_DSU_SNP_CPUM_TO_CPUM)]				= DSU_PMU_SNP_CPUM_TO_CPUM,
	/* OTHERS */
	[dsu_index(HM_HW_DSU_SCU_HZD_ADDRESS)]				= DSU_PMU_SCU_HZD_ADDRESS,
	[dsu_index(HM_HW_DSU_SCU_BIB_ACCESS)]				= DSU_PMU_SCU_BIB_ACCESS,
	[dsu_index(HM_HW_DSU_SCU_SAFE_MODE_CYCLES)]			= DSU_PMU_SCU_SAFE_MODE_CYCLES,
	[dsu_index(HM_HW_DSU_CFRQ_ENQ_STALL)]				= DSU_PMU_CFRQ_ENQ_STALL,
	[dsu_index(HM_HW_DSU_RXDAT_STALL)]					= DSU_PMU_RXDAT_STALL,
	[dsu_index(HM_HW_DSU_CFRQ_LOW_OCC)]					= DSU_PMU_CFRQ_LOW_OCC,
	[dsu_index(HM_HW_DSU_RXSNPSHARED_B0)]				= DSU_PMU_RXSNPSHARED_B0,
	[dsu_index(HM_HW_DSU_RXSNPSHARED_M0)]				= DSU_PMU_RXSNPSHARED_M0,
	[dsu_index(HM_HW_DSU_RXSNPSHARED_M1)]				= DSU_PMU_RXSNPSHARED_M1,
	[dsu_index(HM_HW_DSU_RXSNPSHARED_M2)]				= DSU_PMU_RXSNPSHARED_M2,
	[dsu_index(HM_HW_DSU_RXSNPSHARED_M3)]				= DSU_PMU_RXSNPSHARED_M3,
	[dsu_index(HM_HW_DSU_RXSNPUNIQUE_B0)]				= DSU_PMU_RXSNPUNIQUE_B0,
	[dsu_index(HM_HW_DSU_RXSNPUNIQUE_M0)]				= DSU_PMU_RXSNPUNIQUE_M0,
	[dsu_index(HM_HW_DSU_RXSNPUNIQUE_M1)]				= DSU_PMU_RXSNPUNIQUE_M1,
	[dsu_index(HM_HW_DSU_RXSNPUNIQUE_M2)]				= DSU_PMU_RXSNPUNIQUE_M2,
	[dsu_index(HM_HW_DSU_RXSNPUNIQUE_M3)]				= DSU_PMU_RXSNPUNIQUE_M3,
#endif /* PERF_PLATFORM_NAMTSO_MAP */