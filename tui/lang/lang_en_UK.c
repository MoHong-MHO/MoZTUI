#include "../tui_lang.h"
__attribute__((constructor)) static void reg(void) { tui_lang_register(7, "English(UK)", "Press any key", "Arrows, Enter select, ESC exit", "(y/n): ", "Yes", "No", "OK", "Cancel"); }
