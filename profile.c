#include <ncurses.h>
#include "menu.h"

void show_profile()
{
    printw("Displaying user profile...\n");
    refresh();
    getch();
}
