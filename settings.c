#include <ncurses.h>
#include <string.h>
#include "menu.h"

void show_settings()
{

    start_color();
    init_pair(1, COLOR_BLACK, COLOR_YELLOW);
    init_pair(2, COLOR_BLACK, COLOR_WHITE);
    init_pair(3, COLOR_BLACK, COLOR_GREEN);

    int highlight = 1, choice = 0, c;
    char *choices[] = {"DIFFICULTY", "CHARACTER COLOR", "MUSIC", "BACK TO MENU"};
    int n_choices = sizeof(choices) / sizeof(char *);

    clear();
    refresh();

    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    int menu_height = 20;
    int menu_width = 60;
    int start_y = (max_y - menu_height) / 2;
    int start_x = (max_x - menu_width) / 2;

    WINDOW *menu_win = newwin(menu_height, menu_width, start_y, start_x);
    box(menu_win, 0, 0);
    wbkgd(menu_win, COLOR_PAIR(1));

    int max_y1, max_x1;
    getmaxyx(stdscr, max_y1, max_x1);
    int menu_height1 = 22;
    int menu_width1 = 62;
    int start_y1 = (max_y1 - menu_height1) / 2;
    int start_x1 = (max_x1 - menu_width1) / 2;

    WINDOW *menu_win1 = newwin(menu_height1, menu_width1, start_y1, start_x1);
    box(menu_win1, 0, 0);
    wbkgd(menu_win1, COLOR_PAIR(2));

    keypad(menu_win, TRUE);

    while (1)
    {
        wbkgd(menu_win, COLOR_PAIR(1));

        if (highlight == 1)
        {
            wattron(menu_win, A_REVERSE);
            mvwprintw(menu_win, 2, 25, "DIFFICULTY");
            wattroff(menu_win, A_REVERSE);
        }
        else
        {
            mvwprintw(menu_win, 2, 25, "DIFFICULTY");
        }
        if (highlight == 2)
        {
            wattron(menu_win, A_REVERSE);
            mvwprintw(menu_win, 7, 22.5, "CHARACTER COLOR");
            wattroff(menu_win, A_REVERSE);
        }
        else
        {
            mvwprintw(menu_win, 7, 22.5, "CHARACTER COLOR");
        }
        if (highlight == 3)
        {
            wattron(menu_win, A_REVERSE);
            mvwprintw(menu_win, 12, 27.5, "MUSIC");
            wattroff(menu_win, A_REVERSE);
        }
        else
        {
            mvwprintw(menu_win, 12, 27.5, "MUSIC");
        }
        if (highlight == 4)
        {
            wattron(menu_win, A_REVERSE);
            mvwprintw(menu_win, 17, 25, "BACK TO MENU");
            wattroff(menu_win, A_REVERSE);
        }
        else
        {
            mvwprintw(menu_win, 17, 25, "BACK TO MENU");
        }

        wrefresh(menu_win1);
        wrefresh(menu_win);
        c = wgetch(menu_win);
        switch (c)
        {
        case KEY_UP:
            highlight = (highlight == 1) ? n_choices : highlight - 1;
            break;
        case KEY_DOWN:
            highlight = (highlight == n_choices) ? 1 : highlight + 1;
            break;
        case 10:
            choice = highlight;
            break;
        }
        if (choice != 0)
            break;
    }

    delwin(menu_win);

    switch (choice)
    {
    case 1:
        clear();
        refresh();
        break;
    case 2:
        break;
    case 3:
        clear();
        refresh();
        break;
    case 4:
        clear();
        refresh();
        show_menu();
        break;
    }
}
