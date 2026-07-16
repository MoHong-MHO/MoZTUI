#include "../tui_lang.h"
__attribute__((constructor)) static void reg(void) { tui_lang_register(0, "简体中文", "按任意键继续", "上下移动，回车选择，ESC退出", "(y/n): ", "是", "否", "确定", "取消"); }
