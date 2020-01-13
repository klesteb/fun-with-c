
#include <ncurses.h>
#include <panel.h>


void create_window(int row, int col, int width, int height, char *text) {

    WINDOW *window = newwin(height, width, row, col);
    PANEL *panel = new_panel(window);

    set_panel_userptr(panel, (void *)text);
    
}

int main(void) {

    char *text;
    PANEL *panel;
    WINDOW *window;

    initscr();

    create_window(2, 4, 15, 10, "window 1");
    create_window(4, 6, 15, 10, "window 2");
    create_window(6, 8, 15, 10, "window 3");

    printw("Walk the panel stack forewards\n");
    refresh();

    /* walk the internal panel "deck" */

    for (panel = panel_above(NULL);
         panel != NULL;
         panel = panel_above(panel)) {

        window = panel_window(panel);
        box(window, ACS_VLINE, ACS_HLINE);
        text = (char *)panel_userptr(panel);
        mvwprintw(window, 1, 1, "%s", text);
        wnoutrefresh(window);

    }

    update_panels();
    doupdate();
    getch();

    erase();
    printw("Walk the panel stack backwards\n");
    refresh();

    for (panel = panel_below(NULL);
         panel != NULL;
         panel = panel_below(panel)) {

        window = panel_window(panel);
        box(window, ACS_VLINE, ACS_HLINE);
        text = (char *)panel_userptr(panel);
        mvwprintw(window, 1, 1, "%s", text);
        wnoutrefresh(window);

    }

    update_panels();
    doupdate();
    getch();

    erase();
    printw("deallocation\n");
    refresh();

    while ((panel = panel_above(NULL))) {

        window = panel_window(panel);
        text = (char *)panel_userptr(panel);
        printw("deallocating: %s\n", text);
        del_panel(panel);
        delwin(window);

    }

    printw("Press any key to continue\n");
    update_panels();
    doupdate();
    getch();

    if ((panel = panel_below(NULL))) {

        text = (char *)panel_userptr(panel);

        erase();
        printw("panel: %s\n", text);
        refresh();
        getch();

    }

    erase();
    printw("Press any key to exit\n");
    refresh();
    getch();

    endwin();

    return 0;

}

