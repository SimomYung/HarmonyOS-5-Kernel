# Todo - when yocto support release and debug version

obj-m += serial_pl011.o
serial_pl011-objs := serial/serial_core.o
serial_pl011-objs += serial/serial_pl011.o
serial_pl011-objs += serial/serial_pl011_init.o

obj-m += serial_8250.o
serial_8250-objs := serial/serial_core.o
serial_8250-objs += serial/serial_8250.o
serial_8250-objs += serial/serial_8250_init.o

ifeq ($(CONFIG_DEVHOST_TTY_STREAM), y)
obj-m += stream.o
stream-objs := stream/stream_core.o
stream-objs += stream/stream_io.o
stream-objs += stream/stream_init.o
stream-objs += stream/stream_ptmx.o
endif

obj-m += kconsole.o
kconsole-objs := kconsole/kconsole_init.o
kconsole-objs += kconsole/kconsole_core.o
kconsole-objs += kconsole/kconsole_kvic.o

obj-m += tty.o
tty-objs := tty_core.o
tty-objs += tty_io.o
tty-objs += tty_ioctl.o
tty-objs += tty_line_buf.o
tty-objs += tty_convert.o
tty-objs += tty_keyboard.o
tty-objs += tty_echo.o
tty-objs += tty_chrdev.o
tty-objs += tty_init.o
tty-objs += tty_wrap.o
tty-objs += tty_flow.o
tty-objs += tty_workqueue.o

ccflags-y += ${DEVHOST_API_CFLAGS}
ccflags-y += -nostdlib
ccflags-y += -D__LIBLINUX_TTY__
