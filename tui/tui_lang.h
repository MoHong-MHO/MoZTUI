#ifndef TUI_LANG_H
#define TUI_LANG_H

typedef struct {
    int code;
    const char *name;
    const char *msg_continue;
    const char *msg_menu_help;
    const char *msg_yes_no;
    const char *msg_yes;
    const char *msg_no;
    const char *msg_ok;
    const char *msg_cancel;
} tui_lang_t;

int  tui_lang_register(int code, const char *name,
    const char *cont, const char *help, const char *yes_no,
    const char *yes, const char *no, const char *ok, const char *cancel);
int  tui_lang_set(int code);
int  tui_lang_get(void);
const char* tui_lang_get_name(void);
void tui_lang_get_messages(const char **cont, const char **help, const char **yes_no,
    const char **yes, const char **no, const char **ok, const char **cancel);
int  tui_lang_count(void);
int* tui_lang_list(void);

#endif