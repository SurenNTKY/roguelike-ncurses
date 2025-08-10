#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include "user.h"

void show_error_login()
{

    curs_set(0);
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    int win_height = 10, win_width = 70;
    int start_y = (max_y - win_height) / 2;
    int start_x = (max_x - win_width) / 2;

    WINDOW *win = newwin(win_height, win_width, start_y, start_x);
    box(win, 0, 0);
    mvwprintw(win, 2, 25, "INVALID CREDENTIALS!");
    wrefresh(win);

    WINDOW *win1 = newwin(3, 10, start_y + 5, start_x + 10);
    box(win1, 0, 0);
    mvwprintw(win1, 1, 3, "EXIT");
    wrefresh(win1);

    WINDOW *win2 = newwin(3, 10, start_y + 5, start_x + 30);
    box(win2, 0, 0);
    mvwprintw(win2, 1, 2.5, "RETRY");
    wrefresh(win2);

    WINDOW *win3 = newwin(3, 10, start_y + 5, start_x + 50);
    box(win3, 0, 0);
    mvwprintw(win3, 1, 1, "REGISTER");
    wrefresh(win3);

    MEVENT event;
    mousemask(ALL_MOUSE_EVENTS, NULL);

    int ch;
    while ((ch = getch()) != 27)
    {
        if (ch == KEY_MOUSE)
        {
            if (getmouse(&event) == OK)
            {
                if (event.bstate & BUTTON1_CLICKED)
                {
                    if (event.y == start_y + 6 && event.x >= start_x + 32.5 && event.x <= start_x + 37.5)
                    {
                        delwin(win);
                        clear();
                        refresh();
                        curs_set(1);
                        login();
                        return;
                    }
                    else if (event.y == start_y + 6 && event.x >= start_x + 13 && event.x <= start_x + 17)
                    {
                        delwin(win);
                        endwin();
                        printf("Exiting the game...\n");
                        exit(0);
                    }
                    else if (event.y == start_y + 6 && event.x >= start_x + 51 && event.x <= start_x + 59)
                    {
                        delwin(win);
                        clear();
                        refresh();
                        curs_set(1);
                        register_user();
                        return;
                    }
                }
            }
        }
    }
    delwin(win);
}

void login()
{
    start_color();
    mousemask(0, NULL);

    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    init_pair(2, COLOR_BLACK, COLOR_GREEN);
    init_pair(3, COLOR_BLACK, COLOR_CYAN);
    init_pair(4, COLOR_BLACK, COLOR_RED);

    char username[USERNAME_MAX_LENGTH];
    char password[PASSWORD_MIN_LENGTH * 2];
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    int win_height = 25, win_width = 60;
    int start_y = (max_y - win_height) / 2;
    int start_x = (max_x - win_width) / 2;

    WINDOW *win = newwin(win_height, win_width, start_y, start_x);
    box(win, 0, 0);
    mvwprintw(win, 2, 26, "Username: ");
    mvwprintw(win, 8, 26, "Password: ");
    wbkgd(win, COLOR_PAIR(3));
    wrefresh(win);

    WINDOW *sign_box = newwin(3, 15, start_y + 19, start_x + 22.5);
    box(sign_box, 0, 0);
    wbkgd(sign_box, COLOR_PAIR(2));
    attron(COLOR_PAIR(1));
    mvwprintw(sign_box, 1, 4, "SIGN IN");
    attroff(COLOR_PAIR(1));
    wrefresh(sign_box);

    WINDOW *user_box = newwin(3, 40, start_y + 4, start_x + 10);
    box(user_box, 0, 0);
    wbkgd(user_box, COLOR_PAIR(1));
    wrefresh(user_box);

    WINDOW *pass_box = newwin(3, 40, start_y + 10, start_x + 10);
    box(pass_box, 0, 0);
    wbkgd(pass_box, COLOR_PAIR(1));
    wrefresh(pass_box);

    echo();
    mvwgetnstr(user_box, 1, 1, username, USERNAME_MAX_LENGTH);
    mvwgetnstr(pass_box, 1, 1, password, PASSWORD_MIN_LENGTH * 2);
    // printw("%s", username);
    noecho();

    mousemask(ALL_MOUSE_EVENTS, NULL);

    curs_set(0);
    int ch;
    while ((ch = getch()) != 27)
    {
        if (ch == KEY_MOUSE)
        {
            MEVENT event;
            if (getmouse(&event) == OK)
            {
                if (event.bstate & BUTTON1_CLICKED)
                {
                    if (event.y >= start_y + 19 && event.y <= start_y + 22 && event.x >= start_x + 26.5 && event.x <= start_x + 33.5)
                    {
                        if (check_username(username) && check_password_login(username, password))
                        {
                            int max_y2, max_x2;
                            getmaxyx(stdscr, max_y2, max_x2);
                            int menu_height2 = 5;
                            int menu_width2 = 70;
                            int start_y2 = (max_y2 - menu_height2) / 2;
                            int start_x2 = (max_x2 - menu_width2) / 2;

                            WINDOW *menu_win2 = newwin(menu_height2, menu_width2, start_y2, start_x2);
                            box(menu_win2, 0, 0);
                            mvwprintw(menu_win2, 2.5, 19, "Login successful! Redirecting...");
                            wrefresh(menu_win2);
                            napms(2000);
                            delwin(user_box);
                            delwin(pass_box);
                            delwin(win);
                            endwin();
                            return;
                        }
                        else
                        {
                            delwin(user_box);
                            delwin(pass_box);
                            delwin(win);
                            show_error_login();
                            return;
                        }
                    }
                }
            }
        }
    }

    delwin(user_box);
    delwin(pass_box);
    delwin(win);
}
