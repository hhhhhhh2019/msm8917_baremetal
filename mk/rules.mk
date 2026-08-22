$(build)/%.o: $(src)/%.c | $(KCONFIG_AUTOHEADER)
	@mkdir -p $(dir $@)
	$(CC) $(CC_FLAGS) -o $@ -c $<
	@$(SED) -i "s|$(KCONFIG_AUTOHEADER)||g" $(@:.o=.d)

$(build)/%.o: $(src)/%.S | $(KCONFIG_AUTOHEADER)
	@mkdir -p $(dir $@)
	$(AS) $(AS_FLAGS) -o $@ -c $<
