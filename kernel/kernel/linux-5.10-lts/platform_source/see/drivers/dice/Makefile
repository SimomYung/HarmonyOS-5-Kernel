ccflags-$(CONFIG_DICE) += -I$(srctree)/drivers/platform_drivers/tzdriver
ccflags-$(CONFIG_DICE)  += -I$(srctree)/drivers/platform_drivers/dice

ifeq ($(CONFIG_DICE), y)
obj-$(CONFIG_DFX_DEBUG_FS)	+= test/dice_function_test.o
endif

subdir-ccflags-y += -Wdate-time -Wextra -Wfloat-equal -Wformat=2 -Wshadow
