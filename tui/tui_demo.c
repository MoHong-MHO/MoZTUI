#include "tui.h"
#include "tui_lang.h"
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int main(void) {
    char buf[256];
    int choice;
    tui_init();
    tui_set_title("TUI Demo - Full Features");
    
    const char *main_menu[] = {
        "Alert", "Confirm", "Input", "Password", "Message", "Notify",
        "Progress", "Menu", "List", "Checkbox", "Form", "Table",
        "Card", "Tag", "Badge", "Language", "Exit"
    };
    
    do {
        choice = tui_menu("TUI Full Demo", main_menu, 17);
        
        if (choice == 0) tui_alert("Hello TUI!");
        else if (choice == 1) tui_alert(tui_confirm("Sure?") ? "Yes" : "No");
        else if (choice == 2) { tui_input("Enter", buf, sizeof(buf)); tui_alert(buf); }
        else if (choice == 3) { tui_password("Password", buf, sizeof(buf)); tui_alert("OK"); }
        else if (choice == 4) tui_message("Message", "Line1\nLine2\nLine3");
        else if (choice == 5) tui_notify("Notification", 1000);
        else if (choice == 6) for (int p = 0; p <= 100; p += 20) tui_progress("Loading", p), usleep(200000);
        else if (choice == 7) {
            const char *sub[] = {"A", "B", "C"};
            int r = tui_menu("Submenu", sub, 3);
            char msg[32]; snprintf(msg, sizeof(msg), "Selected: %d", r);
            tui_alert(msg);
        } else if (choice == 8) {
            const char *items[] = {"Item1", "Item2", "Item3", "Item4", "Item5"};
            int sel = 0;
            tui_list("Select", items, 5, &sel);
            char msg[64]; snprintf(msg, sizeof(msg), "Selected: %s", items[sel]);
            tui_alert(msg);
        } else if (choice == 9) {
            const char *items[] = {"Apple", "Banana", "Cherry"};
            int checked[3] = {0};
            tui_checkbox("Checkbox", items, 3, checked);
            char msg[128] = "";
            for (int i = 0; i < 3; i++) if (checked[i]) { strcat(msg, items[i]); strcat(msg, " "); }
            tui_alert(msg[0] ? msg : "None");
        } else if (choice == 10) {
            const char *labels[] = {"Name", "Age"};
            char name[64] = "", age[8] = "";
            char *vals[] = {name, age};
            int sizes[] = {64, 8};
            tui_form("Form", labels, vals, sizes, 2);
            char msg[256]; snprintf(msg, sizeof(msg), "Name: %s\nAge: %s", name, age);
            tui_message("Result", msg);
        } else if (choice == 11) {
            const char *headers[] = {"Name", "Value"};
            const char *row1[] = {"A", "1"};
            const char *row2[] = {"B", "2"};
            const char *row3[] = {"C", "3"};
            const char **rows[] = {row1, row2, row3};
            tui_table(headers, 2, rows, 3, 2);
        } else if (choice == 12) {
            tui_card("Card Title", "Card content line1\nline2", 40, 7);
            tui_wait_key();
        } else if (choice == 13) {
            tui_tag("Success", "success");
            tui_tag("Error", "error");
            tui_tag("Warning", "warning");
            tui_tag("Info", "info");
            tui_wait_key();
        } else if (choice == 14) {
            tui_badge("New", "info");
            tui_badge("Hot", "warning");
            tui_wait_key();
        } else if (choice == 15) {
            int *codes = tui_lang_list();
            int count = tui_lang_count();
            const char **names = malloc(count * sizeof(char*));
            for (int i = 0; i < count; i++) {
                tui_lang_set(codes[i]);
                names[i] = tui_lang_get_name();
            }
            int sel = tui_menu("Language", names, count);
            if (sel >= 0) tui_set_language(codes[sel]);
            free(names);
            free(codes);
            tui_alert(tui_get_lang_name());
        }
    } while (choice != 16);
    
    tui_cleanup();
    return 0;
}