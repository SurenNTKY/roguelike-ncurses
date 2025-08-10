#include <ncurses.h>
#include "menu.h"

void continue_game()
{
    printw("Displaying the scoreboard...\n");
    refresh();
    getch();
}