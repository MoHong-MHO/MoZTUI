#include "../tui_lang.h"
__attribute__((constructor)) static void reg(void) { tui_lang_register(1, "繁體中文", "按任意鍵繼續", "上下移動，回車選擇，ESC退出", "(y/n): ", "是", "否", "確定", "取消"); }
