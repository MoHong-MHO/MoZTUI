#include "../tui_lang.h"
__attribute__((constructor)) static void reg(void) { tui_lang_register(11, "Français", "Appuyez sur une touche", "Flèches, Entrée sélectionne, ESC quitte", "(o/n): ", "Oui", "Non", "OK", "Annuler"); }
