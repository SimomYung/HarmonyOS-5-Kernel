-include $(srctree)/drivers/platform_drivers/modem/drv/product_config/product_config.mk

KBUILD_CPPFLAGS += -I$(srctree)/drivers/platform_drivers/modem/drv/diag_v500/ \
                   -I$(srctree)/drivers/platform_drivers/modem/drv/diag_v500/chan_mgr \
                   -I$(srctree)/drivers/platform_drivers/modem/drv/diag_v500/msg_mgr \
                   -I$(srctree)/drivers/platform_drivers/modem/drv/diag_v500/port_mgr \
                   -I$(srctree)/drivers/platform_drivers/modem/drv/diag_v500/report_mgr

obj-y += diag_init.o
obj-y += diag_debug.o
obj-y += diag_nve.o
obj-y += chan_mgr/decode_algorithm.o
obj-y += chan_mgr/msg_chan.o
obj-y += chan_mgr/scm_dst.o
obj-y += chan_mgr/scm_src.o
obj-y += chan_mgr/soft_decode.o
obj-y += msg_mgr/diag_msg_proc.o
obj-y += msg_mgr/diag_msp_msg.o
obj-y += port_mgr/ppm_core.o
obj-y += port_mgr/genl_port.o
obj-y += port_mgr/sock_port.o
obj-y += port_mgr/usb_port.o
obj-y += report_mgr/diag_report.o
