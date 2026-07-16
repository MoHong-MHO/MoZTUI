#include "../tui_lang.h"
__attribute__((constructor)) static void reg(void) { tui_lang_register(8, "日本語", "任意のキーを押してください", "上下移動、Enter選択、ESC終了", "(y/n): ", "はい", "いいえ", "OK", "キャンセル"); }
