/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Header of libdh irq
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jul 20 17:27:13 2020
 */
#ifndef ULIBS_LIBDEVHOST_IRQ_H
#define ULIBS_LIBDEVHOST_IRQ_H

#include <hmkernel/capability.h>
#include <hmkernel/cpuset.h>

struct libdh_irq_server;
typedef struct libdh_irq_server *libdh_irq_server_t;
typedef int (*libdh_irq_handler_t)(int logic_irq, void *args);
struct libdh_irq_ctx;
typedef struct libdh_irq_ctx *libdh_irq_ctx_t;

int libdh_irq_create_server(const char *server_name, int priority,
			    __cpuset_t affinity, libdh_irq_server_t *server /* out */);
int libdh_irq_create_fastpath(void);
int libdh_irq_ap_table_init(void);

int libdh_irq_destroy_server(libdh_irq_server_t server);

int libdh_irq_xlate_ex(int irqctrl_id,
		       const unsigned int *hwconf, unsigned int hwconf_len,
		       unsigned int *logic_irq);

static inline int libdh_irq_xlate(int irqctrl_id, unsigned int hwirq,
				  unsigned int *logic_irq)
{
	static const unsigned int intr_config = 2u;
	static const unsigned int intr_cpu_config = 1u;
	unsigned int hwconf[] = {hwirq, intr_config, intr_cpu_config};
	return libdh_irq_xlate_ex(irqctrl_id, hwconf,
				  (unsigned int)(sizeof(hwconf) / sizeof(hwconf[0])),
				  logic_irq);
}

struct libdh_irq_setup_args {
	int irqctrl_id;
	unsigned int logic_irq;
	unsigned int linux_irq;
	unsigned int affinity;
	libdh_irq_handler_t handler;
	const char *name;
	void *args;
};

int libdh_irq_setup(libdh_irq_server_t server,
		    const struct libdh_irq_setup_args *arg,
		    libdh_irq_ctx_t *ctx_out);
int libdh_irq_setup_bind(libdh_irq_server_t *server, unsigned int server_cnt,
			 const struct libdh_irq_setup_args *arg,
			 libdh_irq_ctx_t *ctx_out);
int libdh_irq_close(libdh_irq_server_t server, libdh_irq_ctx_t ctx);
int libdh_irq_close_bind(libdh_irq_server_t *server, unsigned int server_cnt,
			 libdh_irq_ctx_t ctx);

int libdh_irq_enable(const struct libdh_irq_ctx *ctx);
int libdh_irq_disable(const struct libdh_irq_ctx *ctx);
int libdh_irq_setaffinity(struct libdh_irq_ctx *ctx, unsigned long affinity);
int libdh_irq_getaffinity(const struct libdh_irq_ctx *ctx, unsigned long *affinity);

#endif	/* ULIBS_LIBDEVHOST_IRQ_H */
