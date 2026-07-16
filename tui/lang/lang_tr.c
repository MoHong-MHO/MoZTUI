#include "../tui_lang.h"
__attribute__((constructor)) static void reg(void) { tui_lang_register(20, "Türkçe", "Herhangi bir tuşa basın", "Oklar, Enter seçer, ESC çıkar", "(e/h): ", "Evet", "Hayır", "Tamam", "İptal"); }
