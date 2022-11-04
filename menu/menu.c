#include <stdio.h>
#include "menu.h"

#define MAX_MENU_DEPTH 10

static struct Menu * menupath[MAX_MENU_DEPTH];
static int current_menu = 0;

static void _show_prompt(void)
{
    int i = 0; // (prompt1 -> prompt2 -> prompt3)
    printf("\033[1;33m");
    printf("(");
    for (; i < current_menu; i++) {
        printf("%s -> ", menupath[i]->prompt);
    }
    printf("%s) ", menupath[current_menu]->prompt);
    printf("\033[m");
}

void menu_set_root(struct Menu * root)
{
    menupath[current_menu] = root;
    _show_prompt();
}

void menu_process_user_input(char input)
{
    struct MenuItem * mit = NULL;
    switch (input) {
        case '?':
            mit = menupath[current_menu]->items;
            while (mit->letter != 0) {
                printf("%c - %s\n", mit->letter, mit->desc);
                mit++;
            }
        break;

        case '0':
            if (current_menu > 0) {
                current_menu--;
            }
        break;

        default:
            mit = menupath[current_menu]->items;
            int menuitem_found = 0;
            while (mit->letter != 0) {
                if (mit->letter == input) {
                    menuitem_found = 1;
                    break;
                }
                mit++;
            }
            if (menuitem_found) {
                if (mit->type == MENU_ACTION) {
                    mit->menuact.action();
                }
                else {
                    if (current_menu < MAX_MENU_DEPTH - 1) {
                        menupath[++current_menu] = mit->menuact.menu;
                    }
                    else {
                        printf("No more memory for more deep menues\n");
                    }
                }
            }
            else {
                printf("%c is not a recognized menu item\n", input);
            }

        break;
    }
    _show_prompt();

}