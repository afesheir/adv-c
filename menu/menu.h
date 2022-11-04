enum MenuItemType {
    MENU_SUBMENU,
    MENU_ACTION
};

union MenuAction {
    struct Menu * menu;
    void (*action)(void);
};

struct MenuItem {
    char letter;
    char * desc;
    enum MenuItemType type;   // 0: Another menu, 1: Action
    union MenuAction menuact;
};

struct Menu {
    char * prompt;
    struct MenuItem * items;
};

void menu_set_root(struct Menu * root);
void menu_process_user_input(char input);