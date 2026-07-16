
#include "tui.h"
#include <string.h>
#include <unistd.h>

extern tui_ctx_t g_ctx;

int tui_list(const char *title, const char **items, int count, int *selected) {
    int key, offset = 0, visible = g_ctx.height - 8;
    if (visible > count) visible = count;
    tui_clear();
    if (title) tui_gotoxy(2, 2), printf("%s", title);
    for (int i = 0; i < visible && offset + i < count; i++)
        tui_draw_menu_item(4, 4 + i, (offset + i) == *selected, items[offset + i]);
    while (1) {
        key = tui_get_key();
        if (key == TUI_KEY_UP && *selected > 0) {
            tui_draw_menu_item(4, 4 + (*selected - offset), 0, items[*selected]);
            (*selected)--;
            if (*selected < offset) offset--;
            tui_draw_menu_item(4, 4 + (*selected - offset), 1, items[*selected]);
        } else if (key == TUI_KEY_DOWN && *selected < count - 1) {
            tui_draw_menu_item(4, 4 + (*selected - offset), 0, items[*selected]);
            (*selected)++;
            if (*selected >= offset + visible) offset++;
            tui_draw_menu_item(4, 4 + (*selected - offset), 1, items[*selected]);
        } else if (key == TUI_KEY_ENTER) return 0;
        else if (key == TUI_KEY_ESC) return -1;
        tui_refresh();
    }
}

int tui_checkbox(const char *title, const char **items, int count, int *checked) {
    int key, selected = 0;
    tui_clear();
    if (title) tui_gotoxy(2, 2), printf("%s", title);
    for (int i = 0; i < count; i++) {
        tui_gotoxy(4, 4 + i);
        printf("%c %s", checked[i] ? 'X' : ' ', items[i]);
    }
    while (1) {
        key = tui_get_key();
        if (key == TUI_KEY_UP && selected > 0) selected--;
        else if (key == TUI_KEY_DOWN && selected < count - 1) selected++;
        else if (key == TUI_KEY_SPACE) {
            checked[selected] = !checked[selected];
            tui_gotoxy(4, 4 + selected);
            printf("%c %s", checked[selected] ? 'X' : ' ', items[selected]);
        } else if (key == TUI_KEY_ENTER) return 0;
        else if (key == TUI_KEY_ESC) return -1;
        tui_refresh();
    }
}

int tui_form(const char *title, const char **labels, char **values, int *sizes, int fc) {
    int selected = 0, key;
    tui_clear();
    if (title) tui_gotoxy(2, 2), printf("%s", title);
    for (int i = 0; i < fc; i++)
        tui_gotoxy(2, 4 + i), printf("%s: %s", labels[i], values[i]);
    while (1) {
        key = tui_get_key();
        if (key == TUI_KEY_UP && selected > 0) selected--;
        else if (key == TUI_KEY_DOWN && selected < fc - 1) selected++;
        else if (key >= 32 && key <= 126 && (int)strlen(values[selected]) < sizes[selected] - 1) {
            int len = strlen(values[selected]);
            values[selected][len] = key;
            values[selected][len + 1] = '\0';
            tui_gotoxy(2 + strlen(labels[selected]) + 2, 4 + selected);
            printf("%s", values[selected]);
        } else if (key == TUI_KEY_BACKSPACE && strlen(values[selected]) > 0) {
            int len = strlen(values[selected]);
            values[selected][len - 1] = '\0';
            tui_gotoxy(2 + strlen(labels[selected]) + 2, 4 + selected);
            printf("%-20s", values[selected]);
        } else if (key == TUI_KEY_ENTER) return 0;
        else if (key == TUI_KEY_ESC) return -1;
        tui_refresh();
    }
}

void tui_table(const char **headers, int hc, const char ***rows, int rc, int cc) {
    int *w = malloc(hc * sizeof(int));
    for (int i = 0; i < hc; i++) {
        w[i] = strlen(headers[i]);
        for (int j = 0; j < rc; j++) {
            int len = strlen(rows[j][i]);
            if (len > w[i]) w[i] = len;
        }
        w[i] += 2;
    }
    int tw = 1;
    for (int i = 0; i < hc; i++) tw += w[i] + 1;
    int sx = (g_ctx.width - tw) / 2;
    if (sx < 0) sx = 0;
    tui_clear();
    tui_gotoxy(sx, 2);
    for (int i = 0; i < hc; i++) printf("|%-*s", w[i], headers[i]);
    printf("|");
    tui_hline(3, sx, sx + tw - 1, '-');
    for (int j = 0; j < rc && j < g_ctx.height - 6; j++) {
        tui_gotoxy(sx, 4 + j);
        for (int i = 0; i < hc; i++) printf("|%-*s", w[i], rows[j][i]);
        printf("|");
    }
    tui_gotoxy(2, g_ctx.height - 2);
    printf("Press any key");
    tui_refresh();
    tui_wait_key();
    free(w);
}

void tui_card(const char *title, const char *content, int w, int h) {
    int x = (g_ctx.width - w) / 2, y = (g_ctx.height - h) / 2;
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    tui_rect(x, y, w, h, 1, '*');
    if (title) tui_gotoxy(x + 2, y), printf("%s", title);
    tui_gotoxy(x + 2, y + 2), printf("%s", content);
    tui_refresh();
}

void tui_tag(const char *label, const char *type) {
    if (strcmp(type, "success") == 0) printf("\033[32m[%s]\033[0m", label);
    else if (strcmp(type, "error") == 0) printf("\033[31m[%s]\033[0m", label);
    else if (strcmp(type, "warning") == 0) printf("\033[33m[%s]\033[0m", label);
    else printf("[%s]", label);
}

void tui_badge(const char *text, const char *type) { tui_tag(text, type); }