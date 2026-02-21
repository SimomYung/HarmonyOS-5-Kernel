/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022.
 * Description: Querying information about the speed of the network card
 * Author: xiechengliang <xiechengliang1@huawei.com>
 * Create: 2022-08-24
 */
#include <linux/ethtool.h>
#include <linux/netlink.h>
#include <net/if.h>
#include "libbb.h"

#define SIOCETHTOOL     0x8946

static unsigned int get_num_stats(int fd, struct ifreq *ifr)
{
	unsigned int ret;
	struct {
		struct ethtool_sset_info info;
		uint32_t buf[1];
	} sset_info;

	sset_info.info.cmd = ETHTOOL_GSSET_INFO;
	sset_info.info.reserved = 0;
	sset_info.info.sset_mask = 1ULL << ETH_SS_STATS;
	ifr->ifr_data = (void*)&sset_info;
	ret = ioctl(fd, SIOCETHTOOL, ifr);
	if (ret != 0) {
		perror("ioctl failed in get_num_stats");
		return 0;
	}
	ret = sset_info.info.sset_mask != 0 ? sset_info.info.data[0] : 0;
	return ret;
}

static struct ethtool_gstrings *get_strings(int ethtool_fd, struct ifreq *ifr, unsigned int n_stats)
{
	struct ethtool_gstrings *strings;

	strings = calloc(1, sizeof(*strings) + n_stats * ETH_GSTRING_LEN);
	if (strings == NULL) {
		perror("calloc memory failed in get_strings");
		return NULL;
	}

	strings->cmd = ETHTOOL_GSTRINGS;
	strings->len = n_stats;
	strings->string_set = ETH_SS_STATS;
	ifr->ifr_data = (void *)strings;

	int ret = ioctl(ethtool_fd, SIOCETHTOOL, ifr);
	if (ret != 0) {
		perror("ioctl failed in get_strings");
		free(strings);
		return NULL;
	}

	return strings;
}

static struct ethtool_stats *get_stats(int ethtool_fd, struct ifreq *ifr, unsigned int n_stats)
{
	unsigned int sz_stats = n_stats * sizeof(unsigned long long);
	struct ethtool_stats *stats = calloc(1, sz_stats + sizeof(struct ethtool_stats));
	if (stats == NULL) {
		perror("calloc memory failed in get_stats");
		return NULL;
	}

	stats->cmd = ETHTOOL_GSTATS;
	stats->n_stats = n_stats;
	ifr->ifr_data = (void*)stats;
	int ret = ioctl(ethtool_fd, SIOCETHTOOL, ifr);
	if (ret != 0) {
		perror("ioctl failed in get_stats");
		free(stats);
		return NULL;
	}

	return stats;
}


int FAST_FUNC query_interface_statistics(const char *query_ifname)
{
	unsigned int n_stats, i;
	struct ethtool_gstrings *strings;
	struct ifreq ifr = {};
	struct ethtool_stats *stats;
	int fd;

	strncpy_IFNAMSIZ(ifr.ifr_name, query_ifname);
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd < 0) {
		fd = xsocket(AF_NETLINK, SOCK_RAW, NETLINK_GENERIC);
	}

	n_stats = get_num_stats(fd, &ifr);
	if (n_stats < 1) {
		fprintf(stderr, "no stats available\n");
		close(fd);
		return 1;
	}

	strings = get_strings(fd, &ifr, n_stats);
	if (!strings) {
		fprintf(stderr, "get stats strings info failed in get_strings\n");
		close(fd);
		return 1;
	}

	stats =  get_stats(fd, &ifr, n_stats);
	if (!stats) {
		fprintf(stderr, "get stats info failed in get_stats\n");
		close(fd);
		free(strings);
		return 1;
	}

	fprintf(stdout, "NIC statistics:\n");
	for (i = 0; i < n_stats; i++) {
		fprintf(stdout, "     %.*s: %llu\n", ETH_GSTRING_LEN,
			&strings->data[i * ETH_GSTRING_LEN], stats->data[i]);
	}
	free(strings);
	free(stats);
	close(fd);

	return 0;
}
