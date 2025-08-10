#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "user.h"

#define USERNAME_MAX_LENGTH 30
#define PASSWORD_MIN_LENGTH 7
#define EMAIL_MAX_LENGTH 50

void show_error_register()
{

    curs_set(0);
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    int win_height = 10, win_width = 90;
    int start_y = (max_y - win_height) / 2;
    int start_x = (max_x - win_width) / 2;

    WINDOW *win = newwin(win_height, win_width, start_y, start_x);
    box(win, 0, 0);
    mvwprintw(win, 2, 35, "INVALID CREDENTIALS!");
    wrefresh(win);

    WINDOW *win1 = newwin(3, 10, start_y + 5, start_x + 30);
    box(win1, 0, 0);
    mvwprintw(win1, 1, 3, "EXIT");
    wrefresh(win1);

    WINDOW *win2 = newwin(3, 10, start_y + 5, start_x + 50);
    box(win2, 0, 0);
    mvwprintw(win2, 1, 2.5, "RETRY");
    wrefresh(win2);

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
                    if (event.y == start_y + 6 && event.x >= start_x + 52.5 && event.x <= start_x + 57.5)
                    {
                        delwin(win);
                        clear();
                        refresh();
                        curs_set(1);
                        register_user();
                        return;
                    }
                    else if (event.y == start_y + 6 && event.x >= start_x + 33 && event.x <= start_x + 37)
                    {
                        delwin(win);
                        endwin();
                        printf("Exiting the game...\n");
                        exit(0);
                    }
                }
            }
        }
    }
    delwin(win);
}
void show_error_register1()
{

    curs_set(0);
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    int win_height = 10, win_width = 90;
    int start_y = (max_y - win_height) / 2;
    int start_x = (max_x - win_width) / 2;

    WINDOW *win = newwin(win_height, win_width, start_y, start_x);
    box(win, 0, 0);
    mvwprintw(win, 2, 16, "Invalid email format and Username already exists");
    wrefresh(win);

    WINDOW *win1 = newwin(3, 10, start_y + 5, start_x + 30);
    box(win1, 0, 0);
    mvwprintw(win1, 1, 3, "EXIT");
    wrefresh(win1);

    WINDOW *win2 = newwin(3, 10, start_y + 5, start_x + 50);
    box(win2, 0, 0);
    mvwprintw(win2, 1, 2.5, "RETRY");
    wrefresh(win2);

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
                    if (event.y == start_y + 6 && event.x >= start_x + 52.5 && event.x <= start_x + 57.5)
                    {
                        delwin(win);
                        clear();
                        refresh();
                        curs_set(1);
                        register_user();
                        return;
                    }
                    else if (event.y == start_y + 6 && event.x >= start_x + 33 && event.x <= start_x + 37)
                    {
                        delwin(win);
                        endwin();
                        printf("Exiting the game...\n");
                        exit(0);
                    }
                }
            }
        }
    }
    delwin(win);
}
void show_error_register2()
{

    curs_set(0);
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    int win_height = 10, win_width = 90;
    int start_y = (max_y - win_height) / 2;
    int start_x = (max_x - win_width) / 2;

    WINDOW *win = newwin(win_height, win_width, start_y, start_x);
    box(win, 0, 0);
    mvwprintw(win, 2, 20.5, "Invalid Email format and Invalid Password format");
    wrefresh(win);

    WINDOW *win1 = newwin(3, 10, start_y + 5, start_x + 30);
    box(win1, 0, 0);
    mvwprintw(win1, 1, 3, "EXIT");
    wrefresh(win1);

    WINDOW *win2 = newwin(3, 10, start_y + 5, start_x + 50);
    box(win2, 0, 0);
    mvwprintw(win2, 1, 2.5, "RETRY");
    wrefresh(win2);

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
                    if (event.y == start_y + 6 && event.x >= start_x + 52.5 && event.x <= start_x + 57.5)
                    {
                        delwin(win);
                        clear();
                        refresh();
                        curs_set(1);
                        register_user();
                        return;
                    }
                    else if (event.y == start_y + 6 && event.x >= start_x + 33 && event.x <= start_x + 37)
                    {
                        delwin(win);
                        endwin();
                        printf("Exiting the game...\n");
                        exit(0);
                    }
                }
            }
        }
    }
    delwin(win);
}
void show_error_register3()
{

    curs_set(0);
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    int win_height = 10, win_width = 90;
    int start_y = (max_y - win_height) / 2;
    int start_x = (max_x - win_width) / 2;

    WINDOW *win = newwin(win_height, win_width, start_y, start_x);
    box(win, 0, 0);
    mvwprintw(win, 2, 19.5, "Username already exists and Invalid Password format");
    wrefresh(win);

    WINDOW *win1 = newwin(3, 10, start_y + 5, start_x + 30);
    box(win1, 0, 0);
    mvwprintw(win1, 1, 3, "EXIT");
    wrefresh(win1);

    WINDOW *win2 = newwin(3, 10, start_y + 5, start_x + 50);
    box(win2, 0, 0);
    mvwprintw(win2, 1, 2.5, "RETRY");
    wrefresh(win2);

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
                    if (event.y == start_y + 6 && event.x >= start_x + 52.5 && event.x <= start_x + 57.5)
                    {
                        delwin(win);
                        clear();
                        refresh();
                        curs_set(1);
                        register_user();
                        return;
                    }
                    else if (event.y == start_y + 6 && event.x >= start_x + 33 && event.x <= start_x + 37)
                    {
                        delwin(win);
                        endwin();
                        printf("Exiting the game...\n");
                        exit(0);
                    }
                }
            }
        }
    }
    delwin(win);
}
void show_error_register4()
{

    curs_set(0);
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    int win_height = 10, win_width = 90;
    int start_y = (max_y - win_height) / 2;
    int start_x = (max_x - win_width) / 2;

    WINDOW *win = newwin(win_height, win_width, start_y, start_x);
    box(win, 0, 0);
    mvwprintw(win, 2, 28.5, "Username already exists or invalid");
    wrefresh(win);

    WINDOW *win1 = newwin(3, 10, start_y + 5, start_x + 30);
    box(win1, 0, 0);
    mvwprintw(win1, 1, 3, "EXIT");
    wrefresh(win1);

    WINDOW *win2 = newwin(3, 10, start_y + 5, start_x + 50);
    box(win2, 0, 0);
    mvwprintw(win2, 1, 2.5, "RETRY");
    wrefresh(win2);

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
                    if (event.y == start_y + 6 && event.x >= start_x + 52.5 && event.x <= start_x + 57.5)
                    {
                        delwin(win);
                        clear();
                        refresh();
                        curs_set(1);
                        register_user();
                        return;
                    }
                    else if (event.y == start_y + 6 && event.x >= start_x + 33 && event.x <= start_x + 37)
                    {
                        delwin(win);
                        endwin();
                        printf("Exiting the game...\n");
                        exit(0);
                    }
                }
            }
        }
    }
    delwin(win);
}
void show_error_register5()
{

    curs_set(0);
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    int win_height = 10, win_width = 90;
    int start_y = (max_y - win_height) / 2;
    int start_x = (max_x - win_width) / 2;

    WINDOW *win = newwin(win_height, win_width, start_y, start_x);
    box(win, 0, 0);
    mvwprintw(win, 2, 35, "Invalid email format");
    wrefresh(win);

    WINDOW *win1 = newwin(3, 10, start_y + 5, start_x + 30);
    box(win1, 0, 0);
    mvwprintw(win1, 1, 3, "EXIT");
    wrefresh(win1);

    WINDOW *win2 = newwin(3, 10, start_y + 5, start_x + 50);
    box(win2, 0, 0);
    mvwprintw(win2, 1, 2.5, "RETRY");
    wrefresh(win2);

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
                    if (event.y == start_y + 6 && event.x >= start_x + 52.5 && event.x <= start_x + 57.5)
                    {
                        delwin(win);
                        clear();
                        refresh();
                        curs_set(1);
                        register_user();
                        return;
                    }
                    else if (event.y == start_y + 6 && event.x >= start_x + 33 && event.x <= start_x + 37)
                    {
                        delwin(win);
                        endwin();
                        printf("Exiting the game...\n");
                        exit(0);
                    }
                }
            }
        }
    }
    delwin(win);
}
void show_error_register6()
{

    curs_set(0);
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    int win_height = 10, win_width = 90;
    int start_y = (max_y - win_height) / 2;
    int start_x = (max_x - win_width) / 2;

    WINDOW *win = newwin(win_height, win_width, start_y, start_x);
    box(win, 0, 0);
    mvwprintw(win, 2, 33.5, "Invalid Password format");
    wrefresh(win);

    WINDOW *win1 = newwin(3, 10, start_y + 5, start_x + 30);
    box(win1, 0, 0);
    mvwprintw(win1, 1, 3, "EXIT");
    wrefresh(win1);

    WINDOW *win2 = newwin(3, 10, start_y + 5, start_x + 50);
    box(win2, 0, 0);
    mvwprintw(win2, 1, 2.5, "RETRY");
    wrefresh(win2);

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
                    if (event.y == start_y + 6 && event.x >= start_x + 52.5 && event.x <= start_x + 57.5)
                    {
                        delwin(win);
                        clear();
                        refresh();
                        curs_set(1);
                        register_user();
                        return;
                    }
                    else if (event.y == start_y + 6 && event.x >= start_x + 33 && event.x <= start_x + 37)
                    {
                        delwin(win);
                        endwin();
                        printf("Exiting the game...\n");
                        exit(0);
                    }
                }
            }
        }
    }
    delwin(win);
}

void register_user()
{
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    init_pair(2, COLOR_BLACK, COLOR_CYAN);
    init_pair(3, COLOR_BLACK, COLOR_GREEN);

    char username[USERNAME_MAX_LENGTH];
    char password[PASSWORD_MIN_LENGTH * 2];
    char email[EMAIL_MAX_LENGTH];
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    int win_height = 40, win_width = 80;
    int start_y = (max_y - win_height) / 2;
    int start_x = (max_x - win_width) / 2;

    WINDOW *win = newwin(win_height, win_width, start_y, start_x);
    box(win, 0, 0);
    keypad(win, TRUE);
    wbkgd(win, COLOR_PAIR(2));
    mvwprintw(win, 2, 36, "Username: ");
    mvwprintw(win, 10, 36, "Password: ");
    mvwprintw(win, 11, 23.5, "Password must be at least 7 chars");
    mvwprintw(win, 12, 13, "including one digit, one uppercase, and one lowercase.");
    mvwprintw(win, 18, 37.5, "Email: ");
    wrefresh(win);

    WINDOW *user_box = newwin(3, 40, start_y + 6, start_x + 20);
    box(user_box, 0, 0);
    wbkgd(user_box, COLOR_PAIR(1));
    wrefresh(user_box);

    WINDOW *pass_box = newwin(3, 40, start_y + 14, start_x + 20);
    box(pass_box, 0, 0);
    wbkgd(pass_box, COLOR_PAIR(1));
    wrefresh(pass_box);

    WINDOW *email_box = newwin(3, 40, start_y + 22, start_x + 20);
    box(email_box, 0, 0);
    wbkgd(email_box, COLOR_PAIR(1));
    wrefresh(email_box);

    WINDOW *sign_box = newwin(3, 15, start_y + 30, start_x + 32.5);
    box(sign_box, 0, 0);
    wbkgd(sign_box, COLOR_PAIR(3));
    attron(COLOR_PAIR(3));
    mvwprintw(sign_box, 1, 3.5, "REGISTER");
    attroff(COLOR_PAIR(1));
    wrefresh(sign_box);

    echo();
    mvwgetnstr(user_box, 1, 1, username, USERNAME_MAX_LENGTH);
    mvwgetnstr(pass_box, 1, 1, password, PASSWORD_MIN_LENGTH * 2);
    mvwgetnstr(email_box, 1, 1, email, EMAIL_MAX_LENGTH);
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
                    if (event.y == start_y + 31 && event.x >= start_x + 36.5 && event.x <= start_x + 43.5)
                    {
                        if (!check_username(username) && check_password(password) && check_email(email))
                        {
                            create_user(username, password, email);

                            int max_y2, max_x2;
                            getmaxyx(stdscr, max_y2, max_x2);
                            int menu_height2 = 5;
                            int menu_width2 = 70;
                            int start_y2 = (max_y2 - menu_height2) / 2;
                            int start_x2 = (max_x2 - menu_width2) / 2;

                            WINDOW *menu_win2 = newwin(menu_height2, menu_width2, start_y2, start_x2);
                            box(menu_win2, 0, 0);
                            mvwprintw(menu_win2, 2.5, 19, "User created successfully.");
                            wrefresh(menu_win2);
                            napms(2000);
                            delwin(user_box);
                            delwin(pass_box);
                            delwin(win);
                            endwin();
                            clear();
                            refresh();
                            curs_set(1);
                            login();
                            return;
                        }
                        else if (check_username(username) && !check_email(email) && !check_password(password))
                        {
                            delwin(user_box);
                            delwin(pass_box);
                            delwin(win);
                            show_error_register();
                            return;
                        }
                        else if (check_username(username) && !check_email(email))
                        {
                            delwin(user_box);
                            delwin(pass_box);
                            delwin(win);
                            show_error_register1();
                            return;
                        }
                        else if (!check_email(email) && !check_password(password))
                        {
                            delwin(user_box);
                            delwin(pass_box);
                            delwin(win);
                            show_error_register2();
                            return;
                        }
                        else if (!check_password(email) && check_username(username))
                        {
                            delwin(user_box);
                            delwin(pass_box);
                            delwin(win);
                            show_error_register3();
                            return;
                        }
                        else if (check_username(username))
                        {
                            delwin(user_box);
                            delwin(pass_box);
                            delwin(win);
                            show_error_register4();
                            return;
                        }
                        else if (!check_email(email))
                        {
                            delwin(user_box);
                            delwin(pass_box);
                            delwin(win);
                            show_error_register5();
                            return;
                        }
                        else if (!check_password(email))
                        {
                            delwin(user_box);
                            delwin(pass_box);
                            delwin(win);
                            show_error_register6();
                            return;
                        }
                    }
                }
            }
        }
    }

    delwin(win);
}
