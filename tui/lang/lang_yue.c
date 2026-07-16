#include "../tui_lang.h"
__attribute__((constructor)) static void reg(void) { tui_lang_register(2, "粵語", "㩒是但一粒掣繼續", "上下移動，撳Enter揀，ESC退出", "(y/n): ", "係", "唔係", "確定", "取消"); }
