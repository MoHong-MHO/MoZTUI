#include "../tui_lang.h"
__attribute__((constructor)) static void reg(void) { tui_lang_register(3, "閩南語", "任意鍵繼續", "上下移動，Enter選擇，ESC離開", "(y/n): ", "是", "毋是", "確定", "取消"); }
