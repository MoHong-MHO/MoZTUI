#include "tui_lang.h"
#include <stdlib.h>
#include <string.h>

typedef struct lang_entry {
    int code;
    char *name, *cont, *help, *yes_no, *yes, *no, *ok, *cancel;
    struct lang_entry *next;
} entry_t;

static entry_t *langs = NULL;
static entry_t *current = NULL;

int tui_lang_register(int code, const char *name,
    const char *cont, const char *help, const char *yes_no,
    const char *yes, const char *no, const char *ok, const char *cancel) {
    entry_t *e = malloc(sizeof(entry_t));
    if (!e) return -1;
    e->code = code;
    e->name = strdup(name);
    e->cont = strdup(cont);
    e->help = strdup(help);
    e->yes_no = strdup(yes_no);
    e->yes = strdup(yes);
    e->no = strdup(no);
    e->ok = strdup(ok);
    e->cancel = strdup(cancel);
    e->next = langs;
    langs = e;
    if (!current) current = e;
    return 0;
}

int tui_lang_set(int code) {
    for (entry_t *p = langs; p; p = p->next)
        if (p->code == code) { current = p; return 0; }
    return -1;
}

int tui_lang_get(void) { return current ? current->code : -1; }
const char* tui_lang_get_name(void) { return current ? current->name : "Unknown"; }

void tui_lang_get_messages(const char **cont, const char **help, const char **yes_no,
    const char **yes, const char **no, const char **ok, const char **cancel) {
    if (current) {
        if (cont) *cont = current->cont;
        if (help) *help = current->help;
        if (yes_no) *yes_no = current->yes_no;
        if (yes) *yes = current->yes;
        if (no) *no = current->no;
        if (ok) *ok = current->ok;
        if (cancel) *cancel = current->cancel;
    }
}

int tui_lang_count(void) {
    int n = 0;
    for (entry_t *p = langs; p; p = p->next) n++;
    return n;
}

int* tui_lang_list(void) {
    int n = tui_lang_count();
    int *list = malloc((n + 1) * sizeof(int));
    int i = 0;
    for (entry_t *p = langs; p; p = p->next) list[i++] = p->code;
    list[i] = -1;
    return list;
}