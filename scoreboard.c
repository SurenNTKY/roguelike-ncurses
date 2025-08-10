#include <stdio.h>
#include <ncurses.h>
#include <string.h>
#include "menu.h"

typedef struct
{
    char username[20];
    char experience[20];
    int score;
    int gold;
    int finished;
} Player;

int load_users_from_file(const char *filename, Player players[], int max_players)
{
    FILE *file = fopen(filename, "r");
    if (!file)
        return -1;

    int count = 0;
    while (count < max_players && !feof(file))
    {
        char username[20], password[20], email[50];
        char date[20];
        char time[20];
        if (fscanf(file, "%s %s %s %s %s", username, password, email, date, time) == 5)
        {
            strcpy(players[count].username, username);
            players[count].score = 0;
            players[count].gold = 0;
            players[count].finished = 0;
            strcpy(players[count].experience, date);
            count++;
        }
    }

    fclose(file);
    return count;
}

void draw_scoreboard(Player players[], int num_players)
{
    int row, col;
    getmaxyx(stdscr, row, col);
    int startx = (col - 60) / 2;
    int starty = row - row + 2;
    int width = 60;
    init_pair(4, COLOR_BLACK, COLOR_RED);

    WINDOW *sign_box = newwin(3, 30, 2, 2);
    box(sign_box, 0, 0);
    wbkgd(sign_box, COLOR_PAIR(4));
    attron(COLOR_PAIR(1));
    mvwprintw(sign_box, 1, 4.5, "B A C K   T O   M E N U");
    attroff(COLOR_PAIR(1));
    wrefresh(sign_box);

    WINDOW *legeng = newwin(1, 8, starty + 2, startx - 10);
    box(legeng, 0, 0);
    wbkgd(legeng, COLOR_PAIR(1));
    attron(COLOR_PAIR(1));
    mvwprintw(legeng, 0.5, 1, "LEGEND");
    attroff(COLOR_PAIR(1));
    wrefresh(legeng);

    WINDOW *goat = newwin(1, 8, starty + 4, startx - 10);
    box(goat, 0, 0);
    wbkgd(goat, COLOR_PAIR(2));
    attron(COLOR_PAIR(1));
    mvwprintw(goat, 0.5, 2, "GOAT");
    attroff(COLOR_PAIR(1));
    wrefresh(goat);

    WINDOW *wizard = newwin(1, 8, starty + 6, startx - 10);
    box(wizard, 0, 0);
    wbkgd(wizard, COLOR_PAIR(3));
    attron(COLOR_PAIR(1));
    mvwprintw(wizard, 0.5, 1, "WIZARD");
    attroff(COLOR_PAIR(1));
    wrefresh(wizard);

    mvprintw(starty, startx, "## | Username   | Score | Gold | Finished | Experience");
    mvhline(starty + 1, startx, '-', width);

    for (int i = 0; i < num_players; i++)
    {
        if (i == 0)
        {
            attron(COLOR_PAIR(1));
            mvprintw(starty + 2 + i * 2, startx, "%2d | %-10s | %5d | %4d | %8d | %s",
                     i + 1, players[i].username, players[i].score, players[i].gold,
                     players[i].finished, players[i].experience);
            attroff(COLOR_PAIR(1));
        }
        else if (i == 1)
        {
            attron(COLOR_PAIR(2));
            mvprintw(starty + 2 + i * 2, startx, "%2d | %-10s | %5d | %4d | %8d | %s",
                     i + 1, players[i].username, players[i].score, players[i].gold,
                     players[i].finished, players[i].experience);
            attroff(COLOR_PAIR(2));
        }
        else if (i == 2)
        {
            attron(COLOR_PAIR(3));
            mvprintw(starty + 2 + i * 2, startx, "%2d | %-10s | %5d | %4d | %8d | %s",
                     i + 1, players[i].username, players[i].score, players[i].gold,
                     players[i].finished, players[i].experience);
            attroff(COLOR_PAIR(3));
        }
        else
        {
            mvprintw(starty + 2 + i * 2, startx, "%2d | %-10s | %5d | %4d | %8d | %s",
                     i + 1, players[i].username, players[i].score, players[i].gold,
                     players[i].finished, players[i].experience);
        }
    }

    mvhline(starty + 2 + num_players * 2, startx, '-', width);
}

int show_scoreboard()
{
    Player players[100];
    int num_players;

    num_players = load_users_from_file("users.txt", players, 100);
    if (num_players < 0)
        return 1;

    initscr();
    start_color();
    cbreak();
    noecho();
    mousemask(ALL_MOUSE_EVENTS, NULL);

    init_pair(1, COLOR_WHITE, COLOR_YELLOW);
    init_pair(2, COLOR_BLACK, COLOR_CYAN);
    init_pair(3, COLOR_BLACK, COLOR_GREEN);

    draw_scoreboard(players, num_players);

    MEVENT event;
    while (1)
    {
        int ch = getch();
        if (ch == KEY_MOUSE)
        {
            if (getmouse(&event) == OK)
            {
                if (event.bstate & BUTTON1_CLICKED)
                {
                    if (event.y >= 2 && event.y <= 5 && event.x >= 2 && event.x <= 32)
                    {
                        show_menu();
                        break;
                    }
                }
            }
        }
    }

    endwin();
    return 0;
}
