#ifndef PLAT_ERR_H
#define PLAT_ERR_H
#include <linux/types.h>
#include "idi2axi_intf.h"

int read_err_info(struct idi2axi_channel *ich, idi2axi_err_info *m);

#endif /* end of include guard: PLAT_ERR_H */