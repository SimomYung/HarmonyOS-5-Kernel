# Installation directories.
PREFIX ?= /usr
INCLUDEDIR ?= $(PREFIX)/include
LIBDIR ?= $(PREFIX)/lib
SHLIBDIR ?= /lib
RANLIB ?= ranlib
CILDIR ?= ../cil

VERSION = $(shell cat ../VERSION)
LIBVERSION = 2

LEX = flex
CIL_GENERATED = $(CILDIR)/src/cil_lexer.c

LIBA=libsepol.a 
TARGET=libsepol.so
LIBPC=libsepol.pc
LIBMAP=libsepol.map
LIBMAPTEMP=libsepolmap.temp
LIBSO=$(TARGET).$(LIBVERSION)
OBJS= $(patsubst %.c,%.o,$(sort $(wildcard *.c)))
LOBJS= $(patsubst %.c,%.lo,$(sort $(wildcard *.c)))
CFLAGS ?= -Werror -Wall -W -Wundef -Wshadow -Wmissing-format-attribute -O2 -fno-semantic-interposition

override CFLAGS += -I. -I../include -D_GNU_SOURCE

ifneq ($(DISABLE_CIL),y)
OBJS += $(sort $(patsubst %.c,%.o,$(sort $(wildcard $(CILDIR)/src/*.c)) $(CIL_GENERATED)))
LOBJS += $(sort $(patsubst %.c,%.lo,$(sort $(wildcard $(CILDIR)/src/*.c)) $(CIL_GENERATED)))
override CFLAGS += -I$(CILDIR)/include
endif

LD_SONAME_FLAGS=-soname,$(LIBSO),--version-script=$(LIBMAP),-z,defs

LN=ln
OS := $(shell uname)
ifeq ($(OS), Darwin)
LD_SONAME_FLAGS=-install_name,$(LIBSO)
LDFLAGS += -undefined dynamic_lookup
LN=gln
endif

all: $(LIBA) $(LIBSO) $(LIBPC)


$(LIBA):  $(OBJS)
	$(AR) rcs $@ $^
	$(RANLIB) $@

$(LIBSO): $(LOBJS) $(LIBMAP)
	$(CC) $(CFLAGS) $(LDFLAGS) -shared -o $@ $(LOBJS) -Wl,$(LD_SONAME_FLAGS)
	ln -sf $@ $(TARGET) 

$(LIBPC): $(LIBPC).in ../VERSION
	sed -e 's/@VERSION@/$(VERSION)/; s:@prefix@:$(PREFIX):; s:@libdir@:$(LIBDIR):; s:@includedir@:$(INCLUDEDIR):' < $< > $@

$(LIBMAP): $(LIBMAP).in
	cp $< $(LIBMAPTEMP)
ifneq ($(ENABLE_SEHARMONY_POLICY_SPLIT),y)
	sed -i '/^\s*class_callback_register;/d' $(LIBMAPTEMP)
	sed -i '/^\s*class_callback_deregister;/d' $(LIBMAPTEMP)
endif

ifneq ($(ENABLE_SEHARMONY),y)
	sed -i '/^\s*avtab_search;/d' $(LIBMAPTEMP)
	sed -i '/^\s*avtab_search_node;/d' $(LIBMAPTEMP)
	sed -i '/^\s*avtab_search_node_next;/d' $(LIBMAPTEMP)
	sed -i '/^\s*context_compute_av_norbuf;/d' $(LIBMAPTEMP)
	sed -i '/^\s*context_compute_xpermsd;/d' $(LIBMAPTEMP)
	sed -i '/^\s*constraint_expr_eval;/d' $(LIBMAPTEMP)
	sed -i '/^\s*ebitmap_get_bit;/d' $(LIBMAPTEMP)
	sed -i '/^\s*hashtab_search;/d' $(LIBMAPTEMP)
	sed -i '/^\s*policydb_destroy;/d' $(LIBMAPTEMP)
	sed -i '/^\s*policydb_init;/d' $(LIBMAPTEMP)
	sed -i '/^\s*policydb_read;/d' $(LIBMAPTEMP)
	sed -i '/^\s*policy_file_init;/d' $(LIBMAPTEMP)
	sed -i '/^\s*sepol_set_policydb;/d' $(LIBMAPTEMP)
	sed -i '/^\s*mls_compute_context_len;/d' $(LIBMAPTEMP)
	sed -i '/^\s*mls_sid_to_context;/d' $(LIBMAPTEMP)
	sed -i '/^\s*mls_context_to_sid;/d' $(LIBMAPTEMP)
	sed -i '/^\s*mls_context_isvalid;/d' $(LIBMAPTEMP)
	sed -i '/^\s*mls_compute_sid_nohkip;/d' $(LIBMAPTEMP)
	sed -i '/^\s*ebitmap_destroy_nohkip;/d' $(LIBMAPTEMP)
endif

ifneq ($(ENABLE_HKIP_SEHARMONY_PROT),y)
	sed -i '/^\s*prmem_callback_register;/d' $(LIBMAPTEMP)
	sed -i '/^\s*prmem_callback_deregister;/d' $(LIBMAPTEMP)
endif

ifneq ($(DISABLE_CIL),y)
	cp $(LIBMAPTEMP) $@
else
	sed -e '/^\s*cil_/d' < $(LIBMAPTEMP) > $@
endif

ifneq ($(DISABLE_CIL),y)
$(CILDIR)/src/cil_lexer.o: $(CILDIR)/src/cil_lexer.c
	$(CC) $(filter-out -Werror, $(CFLAGS)) -fPIC -c -o $@ $<

$(CILDIR)/src/cil_lexer.lo: $(CILDIR)/src/cil_lexer.c
	$(CC) $(filter-out -Werror, $(CFLAGS)) -fPIC -DSHARED -c -o $@ $<

$(CILDIR)/src/cil_lexer.c: $(CILDIR)/src/cil_lexer.l
	$(LEX) -o $@ $<

endif

%.o:  %.c 
	$(CC) $(CFLAGS) -fPIC -c -o $@ $<

%.lo:  %.c
	$(CC) $(CFLAGS) -fPIC -DSHARED -c -o $@ $<

install: all
	test -d $(DESTDIR)$(LIBDIR) || install -m 755 -d $(DESTDIR)$(LIBDIR)
	install -m 644 $(LIBA) $(DESTDIR)$(LIBDIR)
	test -d $(DESTDIR)$(SHLIBDIR) || install -m 755 -d $(DESTDIR)$(SHLIBDIR)
	install -m 755 $(LIBSO) $(DESTDIR)$(SHLIBDIR)
	test -d $(DESTDIR)$(LIBDIR)/pkgconfig || install -m 755 -d $(DESTDIR)$(LIBDIR)/pkgconfig
	install -m 644 $(LIBPC) $(DESTDIR)$(LIBDIR)/pkgconfig
	$(LN) -sf --relative $(DESTDIR)$(SHLIBDIR)/$(LIBSO) $(DESTDIR)$(LIBDIR)/$(TARGET)

relabel:
	/sbin/restorecon $(DESTDIR)$(SHLIBDIR)/$(LIBSO)

clean: 
	-rm -f $(LIBPC) $(LIBMAP) $(OBJS) $(LOBJS) $(LIBA) $(LIBSO) $(TARGET) $(CIL_GENERATED) $(LIBMAPTEMP)

indent:
	../../scripts/Lindent $(wildcard *.[ch])

