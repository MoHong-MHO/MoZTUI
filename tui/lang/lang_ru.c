#include "../tui_lang.h"
__attribute__((constructor)) static void reg(void) { tui_lang_register(13, "Русский", "Нажмите любую клавишу", "Стрелки, Enter выбор, ESC выход", "(д/н): ", "Да", "Нет", "ОК", "Отмена"); }
