#include "../tui_lang.h"
__attribute__((constructor)) static void reg(void) { tui_lang_register(14, "العربية", "اضغط أي مفتاح", "أسهم، أدخل اختيار، ESC خروج", "(y/n): ", "نعم", "لا", "موافق", "إلغاء"); }
