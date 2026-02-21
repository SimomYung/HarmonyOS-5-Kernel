#ifndef _HISILICON_DDR_H_
#define _HISILICON_DDR_H_

#ifdef CONFIG_DDR_DDRC_SEC
extern void dmss_ipi_handler(void);
extern void dmss_fiq_handler(void);
#else
static inline void dmss_ipi_handler(void){}
static inline void dmss_fiq_handler(void){}
#endif

#if defined(CONFIG_DDR_CHAMAELEON) || defined(CONFIG_DDR_DFX_MNTN)
bool check_qice_intr(void);
#else
static inline bool check_qice_intr(void) {return false;}
#endif

#if defined(CONFIG_DFX_DIAGINFO_BASEINFO) && defined(CONFIG_DDR_DFX_MNTN)
int get_qice_status_str(const void *rdr_memory, size_t rdr_size, char *buf, size_t size);
int get_qice_freq_info_str(const void *rdr_memory, size_t rdr_size, char *buf, size_t size);
int get_ddr_freq_info_str(const void *rdr_memory, size_t rdr_size, char *buf, size_t size);
int get_ddr_volt_info_str(const void *rdr_memory, size_t rdr_size, char *buf, size_t size);
#else
static inline int get_qice_status_str(const void *rdr_memory, size_t rdr_size, char *buf, size_t size) {return -1;}
static inline int get_qice_freq_info_str(const void *rdr_memory, size_t rdr_size, char *buf, size_t size) {return -1;}
static inline int get_ddr_freq_info_str(const void *rdr_memory, size_t rdr_size, char *buf, size_t size) {return -1;}
static inline int get_ddr_volt_info_str(const void *rdr_memory, size_t rdr_size, char *buf, size_t size) {return -1;}
#endif

#endif
