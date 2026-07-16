#include "../tui_lang.h"
__attribute__((constructor)) static void reg(void) { tui_lang_register(10, "Español", "Presione cualquier tecla", "Flechas, Enter selecciona, ESC sale", "(s/n): ", "Sí", "No", "OK", "Cancelar"); }
