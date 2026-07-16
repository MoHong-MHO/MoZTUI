#include "../tui_lang.h"
__attribute__((constructor)) static void reg(void) { tui_lang_register(19, "Bahasa Indonesia", "Tekan sembarang tombol", "Panah, Enter pilih, ESC keluar", "(y/t): ", "Ya", "Tidak", "OK", "Batal"); }
