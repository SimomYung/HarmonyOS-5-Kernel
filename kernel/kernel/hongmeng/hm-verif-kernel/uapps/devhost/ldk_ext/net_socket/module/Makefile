obj-m += net_socket.o
net_socket-y := net_socket_main.o
net_socket-$(CONFIG_NET_NS) += net_namespace.o
ccflags-y += ${DEVHOST_API_CFLAGS}
ccflags-y += "-I$(srctree)/uapps/devhost/ldk_ext/net_socket/include"
ccflags-y += "-I$(srctree)/uapps/devhost/include"
ccflags-y += -I${HMSDKSYSROOTPATH}/usr/include