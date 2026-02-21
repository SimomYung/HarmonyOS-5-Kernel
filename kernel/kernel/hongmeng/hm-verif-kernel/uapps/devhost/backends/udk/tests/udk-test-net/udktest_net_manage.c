/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Fri Oct 18 18:48:37 2019
 */
#include <libhmlog/hmlog.h>
#include <libhmucap/ucap.h>
#include <libhwsecurec/securec.h>
#include <libhmsrv_io/net.h>
#include <net/if.h>
#include <sys/ioctl.h>

#include "udktest_net.h"

#define ETH_NAME_SIZE 4

static int g_normal_net_ifindex = 1;
static int g_err_net_ifindex = -1;

static int test_open_close_once(void)
{
	int ret;
	unsigned int ifindex = g_normal_net_ifindex;
	struct hmsrv_io_ctx ctx;
	struct ifreq ifr;

	mem_zero_s(ctx);

	ret = hmsrv_io_net_init(ifindex, &ctx, 0);
	ASSERT_ZERO(ret);

	ret = strncpy_s(ifr.ifr_name, IFNAMSIZ, "eth0", ETH_NAME_SIZE);
	ASSERT_ZERO(ret);

	ifr.ifr_flags = IFF_UP;
	ret = hmsrv_io_net_ioctl(&ctx, SIOCSIFFLAGS, (unsigned long)&ifr, false);
	ASSERT_ZERO(ret);

	ifr.ifr_flags = 0;
	ret = hmsrv_io_net_ioctl(&ctx, SIOCSIFFLAGS, (unsigned long)&ifr, false);
	ASSERT_ZERO(ret);

	return 0;
}

static int test_open_close_twice(void)
{
	int ret;
	unsigned int ifindex = g_normal_net_ifindex;
	struct hmsrv_io_ctx ctx, ctx1;
	struct ifreq ifr;

	mem_zero_s(ctx);
	mem_zero_s(ctx1);

	ret = hmsrv_io_net_init(ifindex, &ctx, 0);
	ASSERT_ZERO(ret);

	ret = strncpy_s(ifr.ifr_name, IFNAMSIZ, "eth0", ETH_NAME_SIZE);
	ASSERT_ZERO(ret);

	ifr.ifr_flags = IFF_UP;
	ret = hmsrv_io_net_ioctl(&ctx, SIOCSIFFLAGS, (unsigned long)&ifr, false);
	ASSERT_ZERO(ret);

	ifr.ifr_flags = 0;
	ret = hmsrv_io_net_ioctl(&ctx, SIOCSIFFLAGS, (unsigned long)&ifr, false);
	ASSERT_ZERO(ret);

	/* close twice */
	ret = hmsrv_io_net_ioctl(&ctx, SIOCSIFFLAGS, (unsigned long)&ifr, false);
	ASSERT_ZERO(ret);

	ifr.ifr_flags = IFF_UP;
	ret = hmsrv_io_net_ioctl(&ctx, SIOCSIFFLAGS, (unsigned long)&ifr, false);
	ASSERT_ZERO(ret);

	/* open twice */
	ret = hmsrv_io_net_init(ifindex, &ctx1, 0);
	ASSERT_ZERO(ret);

	ifr.ifr_flags = IFF_UP;
	ret = hmsrv_io_net_ioctl(&ctx, SIOCSIFFLAGS, (unsigned long)&ifr, false);
	ASSERT_ZERO(ret);

	ifr.ifr_flags = 0;
	ret = hmsrv_io_net_ioctl(&ctx, SIOCSIFFLAGS, (unsigned long)&ifr, false);
	ASSERT_ZERO(ret);

	ret = hmsrv_io_net_ioctl(&ctx, SIOCSIFFLAGS, (unsigned long)&ifr, false);
	ASSERT_ZERO(ret);

	return 0;
}

static int test_init_err_ifindex(void)
{
	int ret;
	unsigned int ifindex = g_err_net_ifindex;
	struct hmsrv_io_ctx ctx;

	mem_zero_s(ctx);

	ret = hmsrv_io_net_init(ifindex, &ctx, 0);
	ASSERT_ZERO(!ret);

	return 0;
}

static int test_net_status(void)
{
	int ret;
	unsigned int ifindex = g_normal_net_ifindex;
	struct hmsrv_io_ctx ctx;
	struct netdev_status net_status;
	struct netdev_status *pnet_status = NULL;
	struct ifreq ifr;

	mem_zero_s(ctx);
	mem_zero_s(net_status);

	ret = hmsrv_io_net_init(ifindex, &ctx, 0);
	ASSERT_ZERO(ret);

	ret = strncpy_s(ifr.ifr_name, IFNAMSIZ, "eth0", ETH_NAME_SIZE);
	ASSERT_ZERO(ret);

	ifr.ifr_flags = IFF_UP;
	ret = hmsrv_io_net_ioctl(&ctx, SIOCSIFFLAGS, (unsigned long)&ifr, false);
	ASSERT_ZERO(ret);

	ret = hmsrv_io_net_status(&ctx, &net_status);

	pnet_status = &net_status;
	hm_info("devhost: get netdev status:"
		"total_rx:%llu total_tx:%llu total_alloc:%llu"
		"total_free:%llu rx_in:%llu tx_in:%llu"
		"drv_alloc:%llu drv_free:%llu devhost_alloc:%llu"
		"devhost_free:%llu full_drop:%llu deact_drop:%llu"
		"null_drop:%llu",
		pnet_status->total_rx, pnet_status->total_tx,
		pnet_status->total_alloc, pnet_status->total_free,
		pnet_status->rx_in, pnet_status->tx_in,
		pnet_status->drv_alloc, pnet_status->drv_free,
		pnet_status->devhost_alloc, pnet_status->devhost_free,
		pnet_status->full_drop, pnet_status->deact_drop,
		pnet_status->null_drop);

	ifr.ifr_flags = 0;
	ret = hmsrv_io_net_ioctl(&ctx, SIOCSIFFLAGS, (unsigned long)&ifr, false);
	ASSERT_ZERO(ret);

	return 0;
}

struct test net_tests[] = {
	TESTHM("test_open_close_once", test_open_close_once)
	TESTHM("test_open_close_twice", test_open_close_twice)
	TESTHM("test_init_err_ifindex", test_init_err_ifindex)
	TESTHM("test_net_status", test_net_status)

	{
		.desc = NULL,
		.func = NULL,
		.funcname = NULL,
	}
};
