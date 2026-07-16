#include "../tui_lang.h"
__attribute__((constructor)) static void reg(void) { tui_lang_register(16, "বাংলা", "যেকোনো কী চাপুন", "তীর, এন্টার নির্বাচন, ESC প্রস্থান", "(y/n): ", "হ্যাঁ", "না", "ঠিক", "বাতিল"); }
