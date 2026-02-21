// SPDX-License-Identifier: GPL-2.0
#include <trace/hooks/liblinux.h>

extern int32_t g_pcie_auto_disable_aspm;
extern char *oal_get_pcie_linux_res(void);
extern int32_t oal_pcie_set_device_aspm_dync_disable_mp(char *pcie_res, uint8_t disable);
extern void oal_pcie_device_xfer_pending_sig_clr_mp(char *pcie_res, uint8_t clear);

static uint8_t g_cur_pcie_aspm_status = 1; /* PCIe aspm enabled by default */

/*
 * The pci_rx_hi_task thread does the following before each time write to the PCIe:
 * 1. alloc skb.
 * 2. dma map.
 *
 * They take exactly longer than the PCIe aspm exit latency time.
 * It always traverses 128 times (ringbuf size) in high-throughput scenarios.
 * The result is that the PCIe enters low power consumption and wakeup repeatedly.
 * So we disable the PCIe aspm temporarily when the rx throughput threshold is exceeded.
 */
#define WLAN_THROUGHPUT_THRESHOLD_HIGH 400 /* mbps */
#define PCI_RES_ADDR(base) (base + 0x38)
static void hook_hmac_calc_throughput_aspm(void *args, unsigned int rx_mbps)
{
	uint8_t pcie_aspm_status;
	char *pci_res = NULL;
	char *pci_base = oal_get_pcie_linux_res();

	if (pci_base == NULL || g_pcie_auto_disable_aspm != 0) {
		return;
	}

	pcie_aspm_status = (rx_mbps >= WLAN_THROUGHPUT_THRESHOLD_HIGH) ? 0 : 1;
	if (pcie_aspm_status == g_cur_pcie_aspm_status) {
		return;
	}
	pr_info("hmac_calc_throughput_aspm throughput rx = %d Mbs\n", rx_mbps);

	pci_res = *(char **)PCI_RES_ADDR(pci_base);
	if (pcie_aspm_status) {
		oal_pcie_set_device_aspm_dync_disable_mp(pci_res, 0);
		oal_pcie_device_xfer_pending_sig_clr_mp(pci_res, 1);
	} else {
		oal_pcie_set_device_aspm_dync_disable_mp(pci_res, 1);
		oal_pcie_device_xfer_pending_sig_clr_mp(pci_res, 0);
	}
	g_cur_pcie_aspm_status = pcie_aspm_status;
}

INIT_VENDOR_HOOK(ldk_rvh_hmac_calc_throughput_aspm, hook_hmac_calc_throughput_aspm);
