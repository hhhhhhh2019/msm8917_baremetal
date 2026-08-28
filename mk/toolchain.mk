CROSS_COMPILE ?= aarch64-none-elf-

CC := $(CROSS_COMPILE)gcc
AS := $(CROSS_COMPILE)as
LD := $(CROSS_COMPILE)ld
OBJCOPY := $(CROSS_COMPILE)objcopy

MKBOOTIMG := mkbootimg

GZIP := gzip
SED := sed
GREP := grep
AWK := awk
TR := rt

CC_FLAGS += -MMD -MP -I./include
LD_FLAGS +=

KCONFIG_CONFIG ?= .config
KCONFIG_AUTOHEADER := $(build)/generated/config.h
KCONFIG_DEPS       := $(build)/generated/include

CC_FLAGS += -include $(KCONFIG_AUTOHEADER) -I $(KCONFIG_DEPS)

menuconfig:
	@menuconfig

$(KCONFIG_AUTOHEADER): $(KCONFIG_CONFIG)
	@mkdir -p $(dir $@)
	@mkdir -p $(KCONFIG_DEPS)
	genconfig --header-path $@ --sync-deps $(KCONFIG_DEPS)
