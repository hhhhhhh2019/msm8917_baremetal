build ?= build
src ?= src

$(build)/boot.img: $(build)/bootloader-dtb
	$(MKBOOTIMG) \
		--output "$@" \
		--kernel "$<" \
		--ramdisk ramdisk.img \
		--base 0x80080000

include ../mk/toolchain.mk
include ../mk/rules.mk

include $(KCONFIG_CONFIG)

include $(src)/Makefile

DEP := $(OBJ:%.o=%.d)

CC_FLAGS += -mgeneral-regs-only -ffreestanding

.PHONY: $(build)/boot.img menuconfig clean

$(build)/bootloader-dtb: $(build)/bootloader.gz
	cat $< ./pmos.dtb > $@

$(build)/bootloader.gz: $(build)/bootloader.bin
	$(GZIP) -f -k $< > $@

$(build)/bootloader.bin: $(build)/bootloader.elf
	$(OBJCOPY) -O binary $< $@

$(build)/bootloader.elf: $(OBJ)
	$(LD) $(LD_FLAGS) -T linker.ld -Map=$(build)/bootloader.elf.map -o $@ $^

clean:
	rm -rf $(build)

-include $(DEP)
