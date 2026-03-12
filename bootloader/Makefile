include ../Makefile.defaults

BUILD?=build
SRC=$(shell find ./src -type f -name '*.c' -o -name '*.S')
OBJ:=$(SRC:.c=.o)
OBJ:=$(OBJ:.S=.o)
OBJ:=$(patsubst ./src/%,./$(BUILD)/%,$(OBJ))
DEP=$(OBJ:.o=.d)

AS_FLAGS+=
CC_FLAGS+=-MMD -MP -mgeneral-regs-only -I./include
LD_FLAGS+=-gc-sections

$(BUILD)/%.o: src/%.S
	@mkdir -p `dirname $@`
	$(AS) $(AS_FLAGS) -c $< -o $@

$(BUILD)/%.o: src/%.c
	@mkdir -p `dirname $@`
	$(CC) $(CC_FLAGS) -c $< -o $@

$(BUILD)/boot.img: $(BUILD)/bootloader-dtb
	$(MKBOOTIMG) \
		--output $@ \
		--kernel $< \
		--ramdisk ramdisk.img \
		--base 0x80080000

$(BUILD)/bootloader-dtb: $(BUILD)/bootloader.gz
	# cat $< ~/opt/lk2nd/build-lk2nd-msm8952/lk2nd/device/dts/msm8952/msm8917-xiaomi-common.dtb > $@
	cat $< ../pmos.dtb > $@

$(BUILD)/bootloader.gz: $(BUILD)/bootloader
	gzip -fk $^ > $@

$(BUILD)/bootloader: $(BUILD)/bootloader.elf
	$(OBJCOPY) -O binary $< $@

$(BUILD)/bootloader.elf: $(OBJ) | $(BUILD)
	$(LD) $(LD_FLAGS) -T linker.ld -Map=$(BUILD)/bootloader.elf.map $^ -o $@

-include $(DEP)

.PHONY: $(BUILD)/boot.img
