#include "tui.h"
#include "tui_lang.h"
#include <unistd.h>
#include <string.h>
#include <termios.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <sys/select.h>

static struct termios orig_termios;
tui_ctx_t g_ctx = {80, 24};
static int initialized = 0;
static const char *msg_continue, *msg_menu_help, *msg_yes_no;
static const char *msg_yes, *msg_no, *msg_ok, *msg_cancel;

static void update_messages(void) {
    tui_lang_get_messages(&msg_continue, &msg_menu_help, &msg_yes_no,
                          &msg_yes, &msg_no, &msg_ok, &msg_cancel);
}

static void tui_set_bg(void) { printf("\033[44m"); }

static void sig_handler(int sig) {
    if (sig == SIGINT) { tui_cleanup(); printf("\n"); exit(0); }
    if (sig == SIGWINCH) tui_get_size(&g_ctx.width, &g_ctx.height);
}

static int key_avail(void) {
    struct timeval tv = {0, 0};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    return select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
}

int tui_init(void) {
    if (initialized) return 0;
    tcgetattr(STDIN_FILENO, &orig_termios);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    signal(SIGINT, sig_handler);
    signal(SIGWINCH, sig_handler);
    tui_get_size(&g_ctx.width, &g_ctx.height);
    printf("\033[?25l");
    tui_set_bg();
    initialized = 1;
    update_messages();
    return 0;
}

void tui_cleanup(void) {
    if (!initialized) return;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
    printf("\033[?25h\033[0m\033[2J\033[H");
    fflush(stdout);
    initialized = 0;
}

void tui_clear(void) { printf("\033[2J\033[H"); fflush(stdout); }
void tui_gotoxy(int x, int y) { printf("\033[%d;%dH", y, x); }
void tui_refresh(void) { fflush(stdout); }
void tui_get_size(int *w, int *h) {
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0) {
        if (w) *w = ws.ws_col;
        if (h) *h = ws.ws_row;
    } else { if (w) *w = 80; if (h) *h = 24; }
}

void tui_set_language(int lang) {
    tui_lang_set(lang);
    update_messages();
}

int tui_get_language(void) {
    return tui_lang_get();
}

const char* tui_get_lang_name(void) {
    return tui_lang_get_name();
}

void tui_beep(void) { printf("\007"); fflush(stdout); }
void tui_set_title(const char *title) { printf("\033]0;%s\007", title); }

void tui_center_text(int y, const char *text) {
    int len = strlen(text);
    int x = (g_ctx.width - len) / 2;
    if (x < 0) x = 0;
    tui_gotoxy(x, y);
    printf("%s", text);
}

void tui_hline(int y, int x1, int x2, char c) {
    if (x1 < 1) x1 = 1;
    if (x2 > g_ctx.width) x2 = g_ctx.width;
    tui_gotoxy(x1, y);
    for (int i = x1; i <= x2; i++) putchar(c);
}

void tui_vline(int x, int y1, int y2, char c) {
    if (y1 < 1) y1 = 1;
    if (y2 > g_ctx.height) y2 = g_ctx.height;
    for (int i = y1; i <= y2; i++) {
        tui_gotoxy(x, i);
        putchar(c);
    }
}

void tui_rect(int x, int y, int w, int h, int fill, char edge) {
    tui_hline(y, x, x + w - 1, edge);
    tui_hline(y + h - 1, x, x + w - 1, edge);
    tui_vline(x, y, y + h - 1, edge);
    tui_vline(x + w - 1, y, y + h - 1, edge);
    if (fill) for (int i = y + 1; i < y + h - 1; i++) {
        tui_gotoxy(x + 1, i);
        for (int j = 0; j < w - 2; j++) putchar(' ');
    }
}

void tui_draw_menu_item(int x, int y, int selected, const char *text) {
    tui_gotoxy(x, y);
    if (selected) {
        printf("\033[30;47m %s \033[0m", text);
        tui_set_bg();
    } else printf(" %s ", text);
}

int tui_get_key(void) {
    char buf[4];
    int n = read(STDIN_FILENO, buf, sizeof(buf));
    if (n <= 0) return 0;
    if (buf[0] == 3) return TUI_KEY_CTRL_C;
    if (buf[0] == 27 && n == 1) return TUI_KEY_ESC;
    if (buf[0] == 10 || buf[0] == 13) return TUI_KEY_ENTER;
    if (buf[0] == ' ') return TUI_KEY_SPACE;
    if (buf[0] == 127 || buf[0] == 8) return TUI_KEY_BACKSPACE;
    if (buf[0] == 9) return TUI_KEY_TAB;
    if (buf[0] == 27 && n >= 3 && buf[1] == '[') {
        switch (buf[2]) {
            case 'A': return TUI_KEY_UP;
            case 'B': return TUI_KEY_DOWN;
            case 'C': return TUI_KEY_LEFT;
            case 'D': return TUI_KEY_RIGHT;
            case 'H': return TUI_KEY_HOME;
            case 'F': return TUI_KEY_END;
            case '5': return TUI_KEY_PGUP;
            case '6': return TUI_KEY_PGDN;
        }
    }
    while (key_avail()) read(STDIN_FILENO, buf, 1);
    return buf[0];
}

void tui_wait_key(void) {
    while (tui_get_key() == 0) usleep(10000);
    while (key_avail()) read(STDIN_FILENO, NULL, 1);
}

void tui_alert(const char *msg) {
    int y = g_ctx.height / 2;
    tui_clear();
    tui_gotoxy(2, y);
    printf("%s", msg);
    tui_gotoxy(2, y + 2);
    printf("%s", msg_continue);
    tui_refresh();
    tui_wait_key();
}

int tui_confirm(const char *msg) {
    int key, y = g_ctx.height / 2;
    tui_clear();
    tui_gotoxy(2, y);
    printf("%s %s", msg, msg_yes_no);
    tui_refresh();
    while (1) {
        key = tui_get_key();
        if (key == 'y' || key == 'Y') return 1;
        if (key == 'n' || key == 'N' || key == TUI_KEY_ESC) return 0;
        usleep(10000);
    }
}

char* tui_input(const char *prompt, char *buf, int size) {
    int i = 0, key, y = g_ctx.height / 2;
    tui_clear();
    tui_gotoxy(2, y);
    printf("%s: ", prompt);
    tui_gotoxy(2, y + 1);
    printf("> ");
    int input_x = 4, input_y = y + 1;
    tui_gotoxy(input_x, input_y);
    while (i < size - 1) {
        key = tui_get_key();
        if (key == TUI_KEY_ENTER) break;
        if (key == TUI_KEY_ESC) { buf[0] = '\0'; return buf; }
        if (key == TUI_KEY_BACKSPACE && i > 0) {
            i--;
            tui_gotoxy(input_x + i, input_y);
            printf(" ");
            tui_gotoxy(input_x + i, input_y);
        } else if (key >= 32 && key <= 126 && i < size - 1) {
            buf[i++] = key;
            putchar(key);
        }
        tui_refresh();
    }
    buf[i] = '\0';
    return buf;
}

char* tui_password(const char *prompt, char *buf, int size) {
    int i = 0, key, y = g_ctx.height / 2;
    tui_clear();
    tui_gotoxy(2, y);
    printf("%s: ", prompt);
    tui_gotoxy(2, y + 1);
    printf("> ");
    int input_x = 4, input_y = y + 1;
    tui_gotoxy(input_x, input_y);
    while (i < size - 1) {
        key = tui_get_key();
        if (key == TUI_KEY_ENTER) break;
        if (key == TUI_KEY_ESC) { buf[0] = '\0'; return buf; }
        if (key == TUI_KEY_BACKSPACE && i > 0) {
            i--;
            tui_gotoxy(input_x + i, input_y);
            printf(" ");
            tui_gotoxy(input_x + i, input_y);
        } else if (key >= 32 && key <= 126 && i < size - 1) {
            buf[i++] = key;
            putchar('*');
        }
        tui_refresh();
    }
    buf[i] = '\0';
    return buf;
}

void tui_message(const char *title, const char *msg) {
    int y = g_ctx.height / 2 - 2;
    tui_clear();
    if (title) tui_gotoxy(2, y), printf("%s", title);
    tui_gotoxy(2, y + 2), printf("%s", msg);
    tui_gotoxy(2, y + 4), printf("%s", msg_continue);
    tui_refresh();
    tui_wait_key();
}

void tui_notify(const char *msg, int ms) {
    int w = strlen(msg) + 4, x = (g_ctx.width - w) / 2, y = g_ctx.height - 3;
    if (x < 2) x = 2;
    tui_gotoxy(x, y);
    printf("\033[30;47m%s\033[0m", msg);
    tui_refresh();
    usleep(ms * 1000);
    tui_gotoxy(x, y);
    for (int i = 0; i < w; i++) printf(" ");
    tui_refresh();
}

void tui_progress(const char *title, int percent) {
    int w = 50, x = (g_ctx.width - w) / 2, y = g_ctx.height / 2;
    int filled = (w - 2) * percent / 100;
    tui_gotoxy(x, y);
    if (title) printf("%s: ", title);
    printf("[");
    for (int i = 0; i < filled; i++) printf("=");
    for (int i = filled; i < w - 2; i++) printf(" ");
    printf("] %3d%%", percent);
}

int tui_menu(const char *title, const char **items, int count) {
    int selected = 0, key, start_x = 4, start_y = 4;
    tui_clear();
    if (title) tui_gotoxy(start_x, start_y - 1), printf("%s", title);
    for (int i = 0; i < count; i++)
        tui_draw_menu_item(start_x, start_y + i, i == selected, items[i]);
    tui_gotoxy(start_x, start_y + count + 1);
    printf("%s", msg_menu_help);
    tui_refresh();
    while (1) {
        key = tui_get_key();
        if (key == TUI_KEY_UP) {
            tui_draw_menu_item(start_x, start_y + selected, 0, items[selected]);
            selected = (selected - 1 + count) % count;
            tui_draw_menu_item(start_x, start_y + selected, 1, items[selected]);
            tui_refresh();
        } else if (key == TUI_KEY_DOWN) {
            tui_draw_menu_item(start_x, start_y + selected, 0, items[selected]);
            selected = (selected + 1) % count;
            tui_draw_menu_item(start_x, start_y + selected, 1, items[selected]);
            tui_refresh();
        } else if (key == TUI_KEY_ENTER) return selected;
        else if (key == TUI_KEY_ESC) return -1;
    }
}

void tui_split(int *x1, int *x2, int *y1, int *y2) {
    if (x1) *x1 = 0;
    if (x2) *x2 = g_ctx.width / 2;
    if (y1) *y1 = 0;
    if (y2) *y2 = g_ctx.height / 2;
}