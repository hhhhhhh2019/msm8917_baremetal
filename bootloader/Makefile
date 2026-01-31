include ../Makefile.defaults

BUILD=build
SRC=src/head.S src/main.c src/str.c src/spmi.c src/log.c src/gpio.c
OBJ=$(patsubst src/%.S,$(BUILD)/%.o,$(patsubst src/%.c,$(BUILD)/%.o,$(SRC)))
DEP=$(OBJ:.o=.d)

AS_FLAGS+=
CC_FLAGS+=-MMD -MP -mgeneral-regs-only -I./include
LD_FLAGS+=-gc-sections

$(BUILD)/%.o: src/%.S | $(BUILD)
	$(AS) $(AS_FLAGS) -c $< -o $@

$(BUILD)/%.o: src/%.c | $(BUILD)
	$(CC) $(CC_FLAGS) -c $< -o $@

$(BUILD)/boot.img: $(BUILD)/bootloader-dtb
	$(MKBOOTIMG) \
		--output $@ \
		--kernel $< \
		--ramdisk ramdisk.img \
		--base 0x80000000

$(BUILD)/bootloader-dtb: $(BUILD)/bootloader.gz
	cat $< ~/opt/lk2nd/build-lk2nd-msm8952/lk2nd/device/dts/msm8952/msm8917-xiaomi-common.dtb > $@

$(BUILD)/bootloader.gz: $(BUILD)/bootloader
	gzip -fk $^ > $@

$(BUILD)/bootloader: $(BUILD)/bootloader.elf
	$(OBJCOPY) -O binary $< $@

$(BUILD)/bootloader.elf: $(OBJ) | $(BUILD)
	$(LD) $(LD_FLAGS) -T linker.ld -Map=$(BUILD)/bootloader.elf.map $^ -o $@

$(BUILD):
	@mkdir -p $@

-include $(DEP)

.PHONY: $(BUILD)/boot.img
