void main() {
#ifdef CONFIG_ENABLE_MMU
  hang();
#endif
}
