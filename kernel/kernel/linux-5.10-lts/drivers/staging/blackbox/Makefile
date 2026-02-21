# SPDX-License-Identifier: GPL-2.0
SHELL=/bin/bash

obj-$(CONFIG_BLACKBOX)		+= blackbox_core.o \
	blackbox_storage.o			   \
	blackbox_common.o

obj-$(CONFIG_BLACKBOX_HISI) 	+= hisi/system_adapter.o

ifeq ($(CONFIG_BLACKBOX_EXPAND_EVENT),y)
mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
cur_makefile_path := $(patsubst %/, %, $(dir $(mkfile_path)))
ifeq ($(CONFIG_BLACKBOX_HISI),y)
curr_chip = hisi
endif

define check-symlink
	set -e; \
	if [ -L $@ ]; then \
		expandlink=$(readlink $@);	\
		if [[ ! $$expandlink =~ $(curr_chip) ]]; then \
			echo "ERROR: the symlink $@ points to $$expandlink but $(curr_chip) was expected"; \
			echo "       and run 'rm $@' to fix it"; \
			exit 1;	\
		fi; \
		test -e $$expandlink || rm $@; \
	fi
endef

define create-symlink
	if [ ! -L $@ ]; then	\
		$(kecho) '  SYMLINK $@ -> $(cur_makefile_path)/$(curr_chip)/$(notdir $@)'; \
		ln -fsn $(cur_makefile_path)/$(curr_chip)/$(notdir $@) $@; \
	fi
endef

$(obj)/blackbox_core.o: $(srctree)/include/linux/blackbox_expand_event.h $(srctree)/include/linux/blackbox_subsystem_def.h

$(srctree)/include/linux/blackbox_expand_event.h $(srctree)/include/linux/blackbox_subsystem_def.h:
	$(Q)$(check-symlink)
	$(Q)$(create-symlink)
endif
