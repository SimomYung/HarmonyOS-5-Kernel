default: all

MAJORVER	:= 1
MINORVER	:= 0

mkfilepath	:= $(abspath $(lastword $(MAKEFILE_LIST)))
topdir		:= $(shell dirname $(mkfilepath))
srcdir		:= $(topdir)/src

ifneq ($(O),)
  outdir	:= $(abspath $(O))
else
  outdir	:= $(topdir)/out
endif
objdir		:= $(shell mkdir -p $(outdir)/obj && cd $(outdir)/obj && pwd)
picobjdir	:= $(shell mkdir -p $(outdir)/obj/.libs && cd $(outdir)/obj/.libs && pwd)

includedir	= /usr/include
libdir		= /usr/lib
DESTDIR		= $(outdir)

ifeq ($(ARCH),aarch64)
ifeq ($(SUBARCH),be)
  CROSS_COMPILE = aarch64_be-linux-gnu-
else
  CROSS_COMPILE = aarch64-linux-gnu-
endif
endif

ifeq ($(ARCH),arm)
ifeq ($(SUBARCH),be)
  CROSS_COMPILE = armeb-linux-gnueabi-
else
  CROSS_COMPILE = arm-linux-gnueabi-
endif
endif

CC		?= $(CROSS_COMPILE)gcc
AR		?= $(CROSS_COMPILE)ar
RANLIB		?= $(CROSS_COMPILE)ranlib
STRIP		?= $(CROSS_COMPILE)strip

INCLUDE_PATHS	= $(topdir)/include
INCLUDE_PATH	= $(INCLUDE_PATHS:%=-I%)
CFLAGS_AUTO	= -O2 -std=gnu99 -fno-asynchronous-unwind-tables -Wall -DSECUREC_IN_KERNEL=0


ifeq ($(ARCH),i386)
  CFLAGS_USR += -m32
endif

CFLAGS += $(CFLAGS_AUTO) $(CFLAGS_USR) $(INCLUDE_PATH)

SOURCES = $(sort $(wildcard $(srcdir)/*.c))
OBJECTS = $(patsubst %.c,%.o,$(SOURCES))

.c.o:
	$(CC) $(CFLAGS) -fPIC -c $< -o $(picobjdir)/$(patsubst %.c,%.o,$(notdir $<))
	$(CC) $(CFLAGS) -c $< -o $(objdir)/$(patsubst %.c,%.o,$(notdir $<)) >/dev/null 2>&1

all: $(OBJECTS)
	$(AR) cru $(outdir)/libhwsecurec.a $(patsubst %.o,$(objdir)/%.o,$(notdir $^))
	$(RANLIB) $(outdir)/libhwsecurec.a
	# For hmld.so.elf
	$(AR) cru $(outdir)/libhwsecurec.pic.a $(patsubst %.o,$(picobjdir)/%.o,$(notdir $^))
	$(RANLIB) $(outdir)/libhwsecurec.pic.a
	$(CC) $(CFLAGS) $(LDFLAGS) -nostdlib -shared -Wl,-soname,libhwsecurec.so.$(MAJORVER) \
		-o $(outdir)/libhwsecurec.so.$(MAJORVER).$(MINORVER) \
		$(patsubst %.o,$(picobjdir)/%.o,$(notdir $^)) -lc
	cp -a $(outdir)/libhwsecurec.so.$(MAJORVER).$(MINORVER) $(outdir)/libhwsecurec.unstripped.so.$(MAJORVER).$(MINORVER)
	$(STRIP) -D $(STRIP_OPTION) $(outdir)/libhwsecurec.so.$(MAJORVER).$(MINORVER)
	ln -sf libhwsecurec.so.$(MAJORVER).$(MINORVER) $(outdir)/libhwsecurec.so.$(MAJORVER)
	ln -sf libhwsecurec.so.$(MAJORVER) $(outdir)/libhwsecurec.so
	ln -sf libhwsecurec.so.$(MAJORVER).$(MINORVER) $(outdir)/libsecurec.so.$(MAJORVER)
	ln -sf libsecurec.so.$(MAJORVER) $(outdir)/libsecurec.so

install:
	install -m 0755 -d $(DESTDIR)$(includedir)
	install -m 0755 -d $(DESTDIR)$(libdir)
	install -m 0644 $(topdir)/include/securec.h $(DESTDIR)$(includedir)
	install -m 0644 $(topdir)/include/securectype.h $(DESTDIR)$(includedir)
	install -m 0644 $(outdir)/libhwsecurec.a $(DESTDIR)$(libdir)
	install -m 0644 $(outdir)/libhwsecurec.pic.a $(DESTDIR)$(libdir)
	install -m 0644 $(outdir)/libhwsecurec.a $(DESTDIR)$(libdir)/libsecurec.a
	install -m 0644 $(outdir)/libhwsecurec.pic.a $(DESTDIR)$(libdir)/libsecurec.pic.a
	install -m 0644 $(outdir)/libhwsecurec.so.$(MAJORVER).$(MINORVER) $(DESTDIR)$(libdir)
	cp -P $(outdir)/libhwsecurec.so.$(MAJORVER) $(DESTDIR)$(libdir)
	cp -P $(outdir)/libhwsecurec.so $(DESTDIR)$(libdir)
	cp -P $(outdir)/libsecurec.so.$(MAJORVER) $(DESTDIR)$(libdir)
	cp -P $(outdir)/libsecurec.so $(DESTDIR)$(libdir)

clean:
	rm -rf $(outdir)

.PHONY: all clean
