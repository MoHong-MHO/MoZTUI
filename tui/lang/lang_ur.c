#include "../tui_lang.h"
__attribute__((constructor)) static void reg(void) { tui_lang_register(18, "اردو", "کوئی بھی کلید دبائیں", "تیر، انٹر منتخب کریں، ESC باہر", "(y/n): ", "ہاں", "نہیں", "ٹھیک", "منسوخ"); }
