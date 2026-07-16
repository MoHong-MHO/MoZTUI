#include "../tui_lang.h"
__attribute__((constructor)) static void reg(void) { tui_lang_register(17, "Português", "Pressione qualquer tecla", "Setas, Enter seleciona, ESC sai", "(s/n): ", "Sim", "Não", "OK", "Cancelar"); }
