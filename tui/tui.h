#ifndef TUI_H
#define TUI_H

#include <stdio.h>
#include <stdlib.h>

#define TUI_KEY_UP      1000
#define TUI_KEY_DOWN    1001
#define TUI_KEY_LEFT    1002
#define TUI_KEY_RIGHT   1003
#define TUI_KEY_ENTER   10
#define TUI_KEY_ESC     27
#define TUI_KEY_SPACE   32
#define TUI_KEY_CTRL_C  3
#define TUI_KEY_BACKSPACE 127
#define TUI_KEY_TAB     9
#define TUI_KEY_HOME    1004
#define TUI_KEY_END     1005
#define TUI_KEY_PGUP    1006
#define TUI_KEY_PGDN    1007

typedef struct { int width, height; } tui_ctx_t;

// 核心
int  tui_init(void);
void tui_cleanup(void);
void tui_clear(void);
void tui_gotoxy(int x, int y);
void tui_refresh(void);
void tui_get_size(int *w, int *h);
int  tui_get_key(void);
void tui_wait_key(void);
void tui_set_language(int lang);
int  tui_get_language(void);
const char* tui_get_lang_name(void);
void tui_beep(void);
void tui_set_title(const char *title);

// 绘图
void tui_center_text(int y, const char *text);
void tui_hline(int y, int x1, int x2, char c);
void tui_vline(int x, int y1, int y2, char c);
void tui_rect(int x, int y, int w, int h, int fill, char edge);
void tui_draw_menu_item(int x, int y, int selected, const char *text);

// 对话框
void tui_alert(const char *msg);
int  tui_confirm(const char *msg);
char* tui_input(const char *prompt, char *buf, int size);
char* tui_password(const char *prompt, char *buf, int size);
void tui_message(const char *title, const char *msg);
void tui_notify(const char *msg, int ms);
void tui_progress(const char *title, int percent);

// 菜单选择
int  tui_menu(const char *title, const char **items, int count);
int  tui_list(const char *title, const char **items, int count, int *selected);
int  tui_checkbox(const char *title, const char **items, int count, int *checked);
int  tui_form(const char *title, const char **labels, char **values, int *sizes, int fc);

// 表格与卡片
void tui_table(const char **headers, int hc, const char ***rows, int rc, int cc);
void tui_card(const char *title, const char *content, int w, int h);
void tui_tag(const char *label, const char *type);
void tui_badge(const char *text, const char *type);

// 分割
void tui_split(int *x1, int *x2, int *y1, int *y2);

#endif