#include <stdio.h>
#include <ncurses.h>
#include "user.h"

void guest()
{
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    int win_height = 12, win_width = 50;
    int start_y = (max_y - win_height) / 2;
    int start_x = (max_x - win_width) / 2;

    WINDOW *win = newwin(win_height, win_width, start_y, start_x);
    box(win, 0, 0);
    mvwprintw(win, 1, 1, "Welcome, Guest!");
    wrefresh(win);

    delwin(win);
}
