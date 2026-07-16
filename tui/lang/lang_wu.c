#include "../tui_lang.h"
__attribute__((constructor)) static void reg(void) { tui_lang_register(5, "吳語", "捺任意鍵繼續", "上下移動，Enter選擇，ESC退出", "(y/n): ", "是", "勿是", "確定", "取消"); }
