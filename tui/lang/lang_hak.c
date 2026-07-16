#include "../tui_lang.h"
__attribute__((constructor)) static void reg(void) { tui_lang_register(4, "客語", "撳任意鍵繼續", "上下移動，Enter選擇，ESC退出", "(y/n): ", "係", "唔係", "確定", "取消"); }
