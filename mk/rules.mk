$(build)/%.o: $(src)/%.c | $(KCONFIG_AUTOHEADER)
	@mkdir -p $(dir $@)

	$(eval OPTS = $(shell grep -Eo 'CONFIG_[A-Za-z0-9_]+' $< | tr '[:upper:]' '[:lower:]' | sed 's/^config_//' | sort -u | sed 's|_|/|g' | awk '{print "$(KCONFIG_DEPS)/" $$0 ".h"}'))
	@for opt in $(OPTS); do \
		mkdir -p `dirname $$opt` ; \
		[ -f $$opt ] || touch $$opt ; \
	done

	$(CC) $(CC_FLAGS) -o $@ -c $<

	@for opt in $(OPTS); do \
		echo "$@: $$opt" >> $(@:.o=.d) ; \
	done
	@$(SED) -i "s|$(KCONFIG_AUTOHEADER)||g" $(@:.o=.d)

$(build)/%.o: $(src)/%.S | $(KCONFIG_AUTOHEADER)
	@mkdir -p $(dir $@)
	$(AS) $(AS_FLAGS) -o $@ -c $<
