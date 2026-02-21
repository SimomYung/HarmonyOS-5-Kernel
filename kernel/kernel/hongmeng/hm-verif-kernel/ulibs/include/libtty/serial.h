/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Some user level struct of serial to export
 * Author: Huawei OS Kernel Lab
 * Create: Tue Apr 07 12:11:57 2020
 */
#ifndef ULIBS_LIBTTY_SERIAL_H
#define ULIBS_LIBTTY_SERIAL_H

/*
 * Since serial_icounter_struct is only defined in linux/serial.h,
 * so we have to define it in hm for udk tty
 */
#define SERIAL_ICOUNT_STRUCT_RESERVED 9
struct serial_icounter_struct {
	int cts;
	int dsr;
	int rng;
	int dcd;
	int rx;
	int tx;
	int frame;
	int overrun;
	int parity;
	int brk;
	int buf_overrun;
	int reserved[SERIAL_ICOUNT_STRUCT_RESERVED];
};

#endif	/* ULIBS_LIBTTY_SERIAL_H */
