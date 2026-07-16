#include "../tui_lang.h"
__attribute__((constructor)) static void reg(void) { tui_lang_register(12, "Deutsch", "Drücken Sie eine beliebige Taste", "Pfeile, Enter wählt, ESC beendet", "(j/n): ", "Ja", "Nein", "OK", "Abbrechen"); }
