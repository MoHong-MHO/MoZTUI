#include "../tui_lang.h"
__attribute__((constructor)) static void reg(void) { tui_lang_register(9, "ひらがな", "なんでもキーをおして", "うえした、エンターでせんたく、ESCでおわる", "(y/n): ", "はい", "いいえ", "OK", "きゃんせる"); }
