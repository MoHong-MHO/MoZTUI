#include "../tui_lang.h"
__attribute__((constructor)) static void reg(void) { tui_lang_register(15, "हिन्दी", "कोई भी कुंजी दबाएं", "तीर, एंटर चुनें, ESC बाहर", "(y/n): ", "हाँ", "नहीं", "ठीक", "रद्द"); }
