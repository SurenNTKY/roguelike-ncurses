#include <ncurses.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <locale.h>
#include <SDL2/SDL_mixer.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include "menu.h"

#define MAX_HEIGHT 10
#define MIN_HEIGHT 8
#define MAX_WIDTH 14
#define MIN_WIDTH 9

typedef struct
{
    int x;
    int y;
} Point;

typedef struct
{
    int from;
    int to;
    Point *points;
    int count;
    int index;
    int visiblePoint;
} Passway;

typedef struct
{
    Point cord;
    char type;
    int password;
} Door;

typedef struct
{
    int health;
    bool isUsed;
    Point cord;
} Food;

typedef struct
{
    int damage;
    Point cord;
    bool isVisible;
} Trap;

typedef struct
{
    int from;
    int to;
    Point cord;
} Stair;

typedef struct
{
    int from;
    int to;
    Point cord;
} Win;

typedef struct
{
    Point cord;
    int height;
    int width;
    Door *doors;
    int doorCount;
    Point window;
    bool isVisible;
    int index;
    Food *foods;
    Trap *traps;
    Stair stair;
    Win win;
    int keyCount;
    int foodCount;
    int goldCount;
    int gunCount;
    int enchantCount;
    int trapCount;
    char type;
    int stairCount;
    int winCount;
} Room;

typedef struct
{
    Point cord;
    int health;
    int state;
    Room *room;
    Passway *passway;
    int foodCount;
    int gunCount;
    int enchantCount;
    int level;
    char *name;
    int acientKey;
    int gold;
    int brokenAcientKey;
} Player;

typedef struct
{
    Room **rooms;
    Passway **passways;
    Point key;
    int roomsCount;
    int level;
} Level;

typedef struct
{
    Level **levels;
    Player *player;

    int currentLevel;
} Game;

void new_game();
Room *createRoom();
void handleMove();
void movePlayer();
void showPlayeInfo();
bool hasOverlap();
void printDoors();
void printRoom();
int distance();
bool isInRoom();
void createPassway();
int inRoom();
int inPassway();
void showLevel();
void showPass();
void createLevel();
int changeLevel();
int isTrap();
void printStair();
void printFoods();
void printTraps();
void *damagePlayer();
void show_end_game();
void exit_game();
Food findFood();
void printWin();

int maxY = 45;
int maxX = 168;
int mapMode = 0;
int damageTime = 30;
WINDOW *mapWin;
Game *game;
Player *player;

int handleMenuSelection(WINDOW *win, char **menu, int size, int withEsc)
{
    keypad(win, true);
    int highlight = 0;
    int x, y;
    x = getcurx(win);
    y = getcury(win);
    y++;
    while (1)
    {
        for (int i = 0; i < size; i++)
        {
            if (i == highlight)
            {
                wattron(win, A_REVERSE);
            }

            mvwprintw(win, y + i * 2, x + 1, "%d - %s", i + 1, menu[i]);
            wattroff(win, A_REVERSE);
        }
        int choice = wgetch(win);
        if (choice == KEY_UP)
        {
            highlight--;
            if (highlight < 0)
                highlight = 0;
        }
        else if (choice == KEY_DOWN)
        {
            highlight++;
            if (highlight > size - 1)
                highlight = size - 1;
        }
        else if (choice == 10)
        {
            break;
        }
        else if (choice == 27 && withEsc)
        {
            highlight = -1;
            break;
        }
    }
    return highlight;
}

void new_game()
{
    srand(time(NULL));
    noecho();

    game = (Game *)malloc(sizeof(Game));
    game->levels = (Level **)malloc(4 * sizeof(Level *));
    for (int i = 0; i < 4; i++)
    {
        game->levels[i] = (Level *)malloc(sizeof(Level));
        createLevel(game->levels[i], i);
    }
    game->currentLevel = 0;

    player = (Player *)malloc(sizeof(Player));
    player->cord.x = game->levels[game->currentLevel]->rooms[0]->cord.x + 2;
    player->cord.y = game->levels[game->currentLevel]->rooms[0]->cord.y + 2;
    player->state = 1;
    player->room = game->levels[game->currentLevel]->rooms[0];
    player->passway = NULL;
    player->health = 30;
    player->foodCount = 0;
    player->level = 0;
    game->player = player;

    showLevel(game->levels[game->currentLevel]);
    getchar();

    keypad(stdscr, true);

    pthread_t damageThread;
    pthread_create(&damageThread, NULL, damagePlayer, NULL);

    handleMove(game->levels[game->currentLevel]);

    handleMove();
    pthread_exit(NULL);
    refresh();
    return;
}

int randomNumber(int min, int max) { return min + rand() % (max - min + 1); }

void *damagePlayer()
{
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    int counter = 0;
    while (1)
    {
        sleep(1);
        counter++;
        if (counter >= damageTime)
        {
            (player)->health -= 5;
            showPlayeInfo(*player);
            if (player->health <= 0)
            {
                show_end_game();
                show_menu();
            }
            counter = 0;
        }
    }
    pthread_exit(NULL);
}

void handleMove()
{
    while (1)
    {
        Level *level = game->levels[game->currentLevel];
        if (player->health <= 0)
        {
            WINDOW *lostWin = newwin(maxY, maxX, 0, 0);
            mvwprintw(lostWin, maxY / 2, 50, "RIP dear %s :(( ...", player->name);
            wrefresh(lostWin);
            sleep(5);
            wclear(lostWin);
            clear();
            break;
        }
        char c = getchar();
        if (c == 27)
        {
            exit_game();
            break;
        }
        else if (c == 'w' || c == '8')
        {
            movePlayer(game->player, level->rooms, level->passways, level->roomsCount, game->player->cord.x, game->player->cord.y - 1);
        }
        else if (c == 's' || c == '2')
        {
            movePlayer(game->player, level->rooms, level->passways, level->roomsCount, game->player->cord.x, game->player->cord.y + 1);
        }
        else if (c == 'd' || c == '6')
        {
            movePlayer(game->player, level->rooms, level->passways, level->roomsCount, game->player->cord.x + 1, game->player->cord.y);
        }
        else if (c == 'a' || c == '4')
        {
            movePlayer(game->player, level->rooms, level->passways, level->roomsCount, game->player->cord.x - 1, game->player->cord.y);
        }
        else if (c == 'e')
        {

            mapWin = newwin(maxY, maxX, 0, 0);
            mvwprintw(mapWin, 1, 1, "Map Mode !!");
            wrefresh(mapWin);
            mapMode = 1;
            for (int i = 0; i < level->roomsCount; i++)
            {
                printRoom(level->rooms[i]);
            }
            wrefresh(mapWin);
            getchar();
            wclear(mapWin);
            wrefresh(mapWin);
            mapMode = 0;
            showLevel(level);
            refresh();
        }
        else if (c == 'f')
        {
            if (player->foodCount == 0)
            {
                continue;
            }
            int max_y, max_x;
            getmaxyx(stdscr, max_y, max_x);
            int menu_height = 20;
            int menu_width = 60;
            int start_y = (max_y - menu_height) / 2;
            int start_x = (max_x - menu_width) / 2;
            WINDOW *menuWin = newwin(start_y, start_x, menu_height, menu_width);
            curs_set(0);
            refresh();
            box(menuWin, 0, 0);
            wrefresh(menuWin);

            char **menu = (char **)malloc(player->foodCount * sizeof(char *));
            for (int i = 0; i < player->foodCount; i++)
            {

                menu[i] = malloc(20 * sizeof(char));
                sprintf(menu[i], "food %d", i + 1);
            }

            wattron(menuWin, A_BLINK);
            mvwprintw(menuWin, 1, 21.5, "THE FOOD IS READY");
            wbkgd(menuWin, COLOR_PAIR(1));
            wrefresh(menuWin);
            wattroff(menuWin, A_BLINK);

            mvwprintw(menuWin, 3, 2, "  ;)( ;");
            mvwprintw(menuWin, 4, 2, " :----:     o8Oo./");
            mvwprintw(menuWin, 5, 2, "C|====| ._o8o8o8Oo_.");
            mvwprintw(menuWin, 6, 2, " |    |  \\========/");
            mvwprintw(menuWin, 7, 2, " `----'   `------'");
            wmove(menuWin, 3, 25);
            wrefresh(menuWin);
            int highlight = handleMenuSelection(menuWin, menu, player->foodCount, 0);
            player->foodCount--;
            player->health += 3;
            getchar();
            wclear(menuWin);
            free(menu);
            clear();
            refresh();
            showLevel(level);
        }
    }
}

void createLevel(Level *level, int levelIndex)
{
    int roomsCounts = randomNumber(6, 8);
    if (roomsCounts == 7)
    {
        roomsCounts = 6;
    }
    Room **rooms = (Room **)malloc(roomsCounts * sizeof(Room *));

    for (int i = 0; i < roomsCounts; i++)
    {
        if (i < roomsCounts / 2)
        {
            int min_x = 4 + (i % (roomsCounts / 2)) * (maxX - 10) / (roomsCounts / 2);
            int max_x = min_x + (maxX - 10) / (roomsCounts / 2);
            int min_y = 4;
            int max_y = maxY / 2 - 4;
            rooms[i] = createRoom(rooms, i, min_x, min_y, max_x, max_y);
            if (i == 0 || i == 5)
            {
                rooms[i]->doorCount = 1;
            }
        }
        else
        {
            int min_x = 4 + (i % (roomsCounts / 2)) * (maxX - 10) / (roomsCounts / 2);
            int max_x = min_x + (maxX - 10) / (roomsCounts / 2);
            int min_y = maxY / 2 + 4;
            int max_y = maxY - 4;
            rooms[i] = createRoom(rooms, i, min_x, min_y, max_x, max_y);
            if (i == 0 || i == roomsCounts - 1)
            {
                rooms[i]->doorCount = 1;
            }
        }
        if (i == 0)
        {
            rooms[0]->isVisible = true;
        }
        if (i == roomsCounts - 1)
        {
            rooms[i]->doors[0].type = 'n';
            rooms[i]->doors[1].type = 'n';
        }
        rooms[i]->index = i;
        rooms[i]->keyCount = 0;
    }

    Passway **passways = (Passway **)malloc((roomsCounts - 1) * sizeof(Passway *));
    for (int i = 0; i < roomsCounts - 1; i++)
    {
        passways[i] = (Passway *)malloc(sizeof(Passway));
        passways[i]->index = i;
        passways[i]->visiblePoint = 0;
    }

    createPassway(passways, rooms, roomsCounts);
    refresh();

    for (int i = 0; i < roomsCounts; i++)
    {
        rooms[i]->foodCount = 0;
        rooms[i]->trapCount = 0;
        rooms[i]->stairCount = 0;
        rooms[i]->winCount = 0;
        int num = rand();
        if (num % 3 == 0)
        {
            int count = randomNumber(2, 4);
            rooms[i]->foodCount = count;
            rooms[i]->foods = (Food *)malloc(count * sizeof(Food));
            for (int j = 0; j < count; j++)
            {
                rooms[i]->foods[j].cord.x = randomNumber(rooms[i]->cord.x + 2, rooms[i]->cord.x + rooms[i]->width - 2);
                rooms[i]->foods[j].cord.y = randomNumber(rooms[i]->cord.y + 3, rooms[i]->cord.y + rooms[i]->height - 3);
                rooms[i]->foods[j].health = 3;
                rooms[i]->foods[j].isUsed = false;
            }
        }
        if (num % 3 == 1)
        {
            int count = randomNumber(1, 3);
            rooms[i]->trapCount = count;
            rooms[i]->traps = (Trap *)malloc(count * sizeof(Trap));
            for (int j = 0; j < count; j++)
            {
                rooms[i]->traps[j].cord.x = randomNumber(rooms[i]->cord.x + 2, rooms[i]->cord.x + rooms[i]->width - 2);
                rooms[i]->traps[j].cord.y = randomNumber(rooms[i]->cord.y + 3, rooms[i]->cord.y + rooms[i]->height - 3);
                rooms[i]->traps[j].isVisible = false;
            }
        }
        if (i == 0 && levelIndex != 0)
        {
            rooms[i]->stairCount = 1;
            rooms[i]->stair.cord.x = rooms[i]->cord.x + rooms[i]->width - 2;
            rooms[i]->stair.cord.y = rooms[i]->cord.y + 2;
            rooms[i]->stair.from = levelIndex;
            rooms[i]->stair.to = levelIndex - 1;
        }
        if (i == roomsCounts - 1 && levelIndex != 3)
        {
            rooms[i]->stairCount = 1;
            rooms[i]->stair.cord.x = rooms[i]->cord.x + rooms[i]->width - 2;
            rooms[i]->stair.cord.y = rooms[i]->cord.y + 2;
            rooms[i]->stair.from = levelIndex;
            rooms[i]->stair.to = levelIndex + 1;
        }
        if (i == roomsCounts - 1 && levelIndex == 3)
        {
            rooms[i]->winCount = 1;
            rooms[i]->win.cord.x = rooms[i]->cord.x + rooms[i]->width - 2;
            rooms[i]->win.cord.y = rooms[i]->cord.y + 2;
            rooms[i]->win.from = levelIndex;
            rooms[i]->win.to = levelIndex + 1;
        }
    }
    int roomIndex = rand() % roomsCounts;
    rooms[roomIndex]->keyCount = 1;
    level->key.x = rooms[roomIndex]->cord.x + rooms[roomIndex]->width - 2;
    level->key.y = rooms[roomIndex]->cord.y + rooms[roomIndex]->height - 3;

    level->level = levelIndex;
    level->rooms = rooms;
    level->passways = passways;
    level->roomsCount = roomsCounts;
}

void showLevel(Level *level)
{
    for (int i = 0; i < level->roomsCount; i++)
    {
        printRoom(level->rooms[i]);
    }

    for (int i = 0; i < level->roomsCount - 1; i++)
    {
        showPass(level->passways[i]);
    }

    mvprintw(game->player->cord.y, game->player->cord.x, "@");

    showPlayeInfo(*game->player);
    refresh();
}

bool hasOverlap(Room a, Room b)
{
    return !(a.cord.x + a.width < b.cord.x - 4 || b.cord.x + b.width < a.cord.x - 4 || a.cord.y + a.height < b.cord.y - 1 || b.cord.y + b.height < a.cord.y - 1);
}

void createPassway(Passway **passways, Room **rooms, int roomCount)
{
    if (roomCount == 6)
    {
        {
            passways[0]->from = 0;
            passways[0]->to = 1;
            passways[0]->count = 0;
            passways[0]->points = (Point *)malloc(1000 * sizeof(Point));
            Point a = rooms[0]->doors[0].cord;
            Point b = rooms[1]->doors[0].cord;
            Point current = a;
            while (current.y != (maxY / 2) - 2)
            {
                current.y += ((maxY / 2) - 2 > current.y) ? 1 : -1;
                passways[0]->points[passways[0]->count++] = current;
            }
            while (current.x != b.x)
            {
                current.x += (b.x > current.x) ? 1 : -1;
                passways[0]->points[passways[0]->count++] = current;
            }
            while (current.y != b.y)
            {
                current.y += (b.y > current.y) ? 1 : -1;
                passways[0]->points[passways[0]->count++] = current;
            }
        }
        {
            passways[1]->from = 0;
            passways[1]->to = 1;
            passways[1]->count = 0;
            passways[1]->points = (Point *)malloc(1000 * sizeof(Point));
            Point a = rooms[1]->doors[1].cord;
            Point b = rooms[2]->doors[1].cord;
            Point current = a;
            while (current.y != 4)
            {
                current.y += (4 > current.y) ? 1 : -1;
                passways[1]->points[passways[1]->count++] = current;
            }
            while (current.x != b.x)
            {
                current.x += (b.x > current.x) ? 1 : -1;
                passways[1]->points[passways[1]->count++] = current;
            }
            while (current.y != b.y)
            {
                current.y += (b.y > current.y) ? 1 : -1;
                passways[1]->points[passways[1]->count++] = current;
            }
        }
        {
            passways[2]->from = 0;
            passways[2]->to = 1;
            passways[2]->count = 0;
            passways[2]->points = (Point *)malloc(1000 * sizeof(Point));
            Point a = rooms[2]->doors[0].cord;
            Point b = rooms[3]->doors[1].cord;
            Point current = a;
            while (current.y != maxY / 2)
            {
                current.y += (maxY / 2 > current.y) ? 1 : -1;
                passways[2]->points[passways[2]->count++] = current;
            }
            while (current.x != b.x)
            {
                current.x += (b.x > current.x) ? 1 : -1;
                passways[2]->points[passways[2]->count++] = current;
            }
            while (current.y != b.y)
            {
                current.y += (b.y > current.y) ? 1 : -1;
                passways[2]->points[passways[2]->count++] = current;
            }
        }
        {
            passways[3]->from = 0;
            passways[3]->to = 1;
            passways[3]->count = 0;
            passways[3]->points = (Point *)malloc(1000 * sizeof(Point));
            Point a = rooms[3]->doors[0].cord;
            Point b = rooms[4]->doors[0].cord;
            Point current = a;
            while (current.y != maxY - 2)
            {
                current.y += (maxY - 2 > current.y) ? 1 : -1;
                passways[3]->points[passways[3]->count++] = current;
            }
            while (current.x != b.x)
            {
                current.x += (b.x > current.x) ? 1 : -1;
                passways[3]->points[passways[3]->count++] = current;
            }
            while (current.y != b.y)
            {
                current.y += (b.y > current.y) ? 1 : -1;
                passways[3]->points[passways[3]->count++] = current;
            }
        }
        {
            passways[4]->from = 0;
            passways[4]->to = 1;
            passways[4]->count = 0;
            passways[4]->points = (Point *)malloc(1000 * sizeof(Point));
            Point a = rooms[4]->doors[1].cord;
            Point b = rooms[5]->doors[0].cord;
            Point current = a;
            while (current.y != maxY / 2 + 2)
            {
                current.y += (maxY / 2 + 2 > current.y) ? 1 : -1;
                passways[4]->points[passways[4]->count++] = current;
            }
            while (current.x != maxX - 4)
            {
                current.x += (maxX - 4 > current.x) ? 1 : -1;
                passways[4]->points[passways[4]->count++] = current;
            }
            while (current.y != b.y + 1)
            {
                current.y += (b.y + 1 > current.y) ? 1 : -1;
                passways[4]->points[passways[4]->count++] = current;
            }
            while (current.x != b.x)
            {
                current.x += (b.x > current.x) ? 1 : -1;
                passways[4]->points[passways[4]->count++] = current;
            }
            current.y -= 1;
            passways[4]->points[passways[4]->count++] = current;
        }
    }
    else
    {
        {
            passways[0]->from = 0;
            passways[0]->to = 1;
            passways[0]->count = 0;
            passways[0]->points = (Point *)malloc(1000 * sizeof(Point));
            Point a = rooms[0]->doors[0].cord;
            Point b = rooms[1]->doors[0].cord;
            Point current = a;
            while (current.y != (maxY / 2) - 2)
            {
                current.y += ((maxY / 2) - 2 > current.y) ? 1 : -1;
                passways[0]->points[passways[0]->count++] = current;
            }
            while (current.x != b.x)
            {
                current.x += (b.x > current.x) ? 1 : -1;
                passways[0]->points[passways[0]->count++] = current;
            }
            while (current.y != b.y)
            {
                current.y += (b.y > current.y) ? 1 : -1;
                passways[0]->points[passways[0]->count++] = current;
            }
        }
        {
            passways[1]->from = 0;
            passways[1]->to = 1;
            passways[1]->count = 0;
            passways[1]->points = (Point *)malloc(1000 * sizeof(Point));
            Point a = rooms[1]->doors[1].cord;
            Point b = rooms[2]->doors[1].cord;
            Point current = a;
            while (current.y != 4)
            {
                current.y += (4 > current.y) ? 1 : -1;
                passways[1]->points[passways[1]->count++] = current;
            }
            while (current.x != b.x)
            {
                current.x += (b.x > current.x) ? 1 : -1;
                passways[1]->points[passways[1]->count++] = current;
            }
            while (current.y != b.y)
            {
                current.y += (b.y > current.y) ? 1 : -1;
                passways[1]->points[passways[1]->count++] = current;
            }
        }
        {
            passways[2]->from = 0;
            passways[2]->to = 1;
            passways[2]->count = 0;
            passways[2]->points = (Point *)malloc(1000 * sizeof(Point));
            Point a = rooms[2]->doors[0].cord;
            Point b = rooms[3]->doors[0].cord;
            Point current = a;
            while (current.y != maxY / 2 - 3)
            {
                current.y += (maxY / 2 - 3 > current.y) ? 1 : -1;
                passways[2]->points[passways[2]->count++] = current;
            }
            while (current.x != b.x)
            {
                current.x += (b.x > current.x) ? 1 : -1;
                passways[2]->points[passways[2]->count++] = current;
            }
            while (current.y != b.y)
            {
                current.y += (b.y > current.y) ? 1 : -1;
                passways[2]->points[passways[2]->count++] = current;
            }
        }
        {
            passways[3]->from = 0;
            passways[3]->to = 1;
            passways[3]->count = 0;
            passways[3]->points = (Point *)malloc(1000 * sizeof(Point));
            Point a = rooms[3]->doors[1].cord;
            Point b = rooms[4]->doors[1].cord;
            Point current = a;
            current.y -= 1;
            passways[3]->points[passways[3]->count++] = current;
            while (current.x != maxX - 5)
            {
                current.x += (maxX - 5 > current.x) ? 1 : -1;
                passways[3]->points[passways[3]->count++] = current;
            }
            while (current.y != maxY / 2)
            {
                current.y += (maxY / 2 > current.y) ? 1 : -1;
                passways[3]->points[passways[3]->count++] = current;
            }
            while (current.x != b.x)
            {
                current.x += (b.x > current.x) ? 1 : -1;
                passways[3]->points[passways[3]->count++] = current;
            }
            while (current.y != b.y)
            {
                current.y += (b.y > current.y) ? 1 : -1;
                passways[3]->points[passways[3]->count++] = current;
            }
        }
        {
            passways[4]->from = 0;
            passways[4]->to = 1;
            passways[4]->count = 0;
            passways[4]->points = (Point *)malloc(1000 * sizeof(Point));
            Point a = rooms[4]->doors[0].cord;
            Point b = rooms[5]->doors[0].cord;
            Point current = a;
            while (current.y != maxY - 2)
            {
                current.y += (maxY - 2 > current.y) ? 1 : -1;
                passways[4]->points[passways[4]->count++] = current;
            }
            while (current.x != b.x)
            {
                current.x += (b.x > current.x) ? 1 : -1;
                passways[4]->points[passways[4]->count++] = current;
            }
            while (current.y != b.y)
            {
                current.y += (b.y > current.y) ? 1 : -1;
                passways[4]->points[passways[4]->count++] = current;
            }
        }
        {
            passways[5]->from = 0;
            passways[5]->to = 1;
            passways[5]->count = 0;
            passways[5]->points = (Point *)malloc(1000 * sizeof(Point));
            Point a = rooms[5]->doors[1].cord;
            Point b = rooms[6]->doors[1].cord;
            Point current = a;
            while (current.y != maxY / 2 + 2)
            {
                current.y += (maxY / 2 + 2 > current.y) ? 1 : -1;
                passways[5]->points[passways[5]->count++] = current;
            }
            while (current.x != b.x)
            {
                current.x += (b.x > current.x) ? 1 : -1;
                passways[5]->points[passways[5]->count++] = current;
            }
            while (current.y != b.y)
            {
                current.y += (b.y > current.y) ? 1 : -1;
                passways[5]->points[passways[5]->count++] = current;
            }
        }
        {
            passways[6]->from = 0;
            passways[6]->to = 1;
            passways[6]->count = 0;
            passways[6]->points = (Point *)malloc(1000 * sizeof(Point));
            Point a = rooms[6]->doors[0].cord;
            Point b = rooms[7]->doors[0].cord;
            Point current = a;
            while (current.y != maxY - 2)
            {
                current.y += (maxY - 2 > current.y) ? 1 : -1;
                passways[6]->points[passways[6]->count++] = current;
            }
            while (current.x != b.x)
            {
                current.x += (b.x > current.x) ? 1 : -1;
                passways[6]->points[passways[6]->count++] = current;
            }
            while (current.y != b.y)
            {
                current.y += (b.y > current.y) ? 1 : -1;
                passways[6]->points[passways[6]->count++] = current;
            }
        }
    }
}

int distance(Point a, Point b)
{
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

void printDoors(Room *room)
{
    WINDOW *screen = mapMode ? mapWin : stdscr;
    for (int i = 0; i < room->doorCount; i++)
    {
        if (room->doors[i].type != 'l')
        {
            mvwprintw(screen, room->doors[i].cord.y, room->doors[i].cord.x, "+");
        }
        else
        {
            attron(COLOR_PAIR(1));
            mvwprintw(screen, room->doors[i].cord.y, room->doors[i].cord.x, "+");
            attroff(COLOR_PAIR(1));
            attron(COLOR_PAIR(2));
            mvwprintw(screen, room->cord.y + 3, room->cord.x + 1, "&");
            attroff(COLOR_PAIR(2));
        }
    }
}

Room *createRoom(Room **rooms, int roomsCount, int min_x, int min_y, int max_x, int max_y)
{
    Room *room = (Room *)malloc(sizeof(Room));

    bool validPlacement;
    do
    {
        validPlacement = true;
        room->width = randomNumber(MIN_WIDTH, MAX_WIDTH);
        room->height = randomNumber(MIN_HEIGHT, MAX_HEIGHT);
        room->cord.x = randomNumber(min_x, max_x - room->width);
        room->cord.y = randomNumber(min_y, max_y - room->height);
        for (int i = 0; i < roomsCount; i++)
        {
            if (hasOverlap(*room, *rooms[i]))
            {
                validPlacement = false;
                break;
            }
        }
    } while (!validPlacement);
    room->isVisible = false;
    room->doorCount = 2;
    room->doors = (Door *)malloc(sizeof(Door) * room->doorCount);
    int num = rand();
    for (int i = 0; i < room->doorCount; i++)
    {
        if (num % 5 == 3)
        {
            if (i % 2 == 0)
            {
                room->doors[i].cord.x = room->cord.x + 1 + randomNumber(0, room->width - 2);
                room->doors[i].cord.y = room->cord.y + room->height - 1;
            }
            else
            {
                room->doors[i].cord.x = room->cord.x + 1 + randomNumber(0, room->width - 2);
                room->doors[i].cord.y = room->cord.y + 1;
            }
            room->doors[i].type = 'l';
            room->doors[i].password = 0;
        }
        else
        {
            if (i % 2 == 0)
            {
                room->doors[i].cord.x = room->cord.x + 1 + randomNumber(0, room->width - 2);
                room->doors[i].cord.y = room->cord.y + room->height - 1;
                room->doors[i].type = 'n';
            }
            else
            {
                room->doors[i].cord.x = room->cord.x + 1 + randomNumber(0, room->width - 2);
                room->doors[i].cord.y = room->cord.y + 1;
                room->doors[i].type = 'n';
            }
        }
    }
    return room;
}

bool isInRoom(Room *room, Point p)
{
    return p.x > room->cord.x && p.x < (room->cord.x + room->width) && p.y > room->cord.y && p.y < (room->cord.y + room->height);
}

void showPass(Passway *passway)
{
    for (int i = 0; i < passway->count; i++)
    {
        attron(COLOR_PAIR(5));
        mvprintw(passway->points[i].y, passway->points[i].x, "#");
        attroff(COLOR_PAIR(5));
    }
}

int inRoom(Room **rooms, int roomsCount, Point p)
{
    for (int i = 0; i < roomsCount; i++)
    {
        if (isInRoom(rooms[i], p))
        {
            return i;
        }
    }
    return -1;
}

void printRoom(Room *room)
{
    int x, y, width, height;
    x = room->cord.x;
    y = room->cord.y;
    height = room->height;
    width = room->width;
    WINDOW *screen = mapMode ? mapWin : stdscr;
    for (int j = x + 1; j < x + width; j++)
    {
        attron(COLOR_PAIR(1));
        mvwprintw(screen, y + 1, j, "-");
        mvwprintw(screen, y + height - 1, j, "-");
        attroff(COLOR_PAIR(1));
    }
    for (int j = y + 2; j < y + height - 1; j++)
    {
        attron(COLOR_PAIR(1));
        mvwprintw(screen, j, x, "|");
        mvwprintw(screen, j, x + width, "|");
        attroff(COLOR_PAIR(1));

        for (int k = x + 1; k < x + width; k++)
        {
            attron(COLOR_PAIR(6));
            mvwprintw(screen, j, k, ".");
            attron(COLOR_PAIR(6));
        }
    }
    printDoors(room);
    printStair(room);
    printFoods(room);
    printTraps(room);
    printWin(room);
    refresh();
}

void showPlayeInfo(Player player)
{
    mvprintw(1, maxX / 2, "                                                                                                   ");
    refresh();
    mvprintw(1, maxX / 2, ">>>>>-----------------------> LEVEL : %d  FOODSCOUNT : %d  HEALTH : %d  ", player.level + 1, player.foodCount, player.health);
    refresh();
}

int trapMode = 0;
void movePlayer(Player *player, Room **rooms, Passway **passways, int roomsCount, int x, int y)
{
    char c = mvinch(y, x);
    Point cur;
    cur.x = x;
    cur.y = y;
    if (trapMode == 1)
    {
        mvprintw(player->cord.y, player->cord.x, "^");
        refresh();
    }
    if (game->levels[game->currentLevel]->key.x == x && game->levels[game->currentLevel]->key.y == y)
    {
        player->acientKey += 1;
        player->room->keyCount = 0;
        game->levels[game->currentLevel]->key.x = 0;
        game->levels[game->currentLevel]->key.y = 0;
        c = '.';
        showPlayeInfo(*player);
    }
    if (c == '&')
    {
        int pass = randomNumber(1000, 9999);

        player->room->doors[0].password = pass;
        player->room->doors[1].password = pass;

        mvprintw(1, 1, "password : %d", pass);
        refresh();
        sleep(5);
        mvprintw(1, 1, "                       ");
        refresh();
    }
    if (c == '>')
    {
        Stair stair = player->room->stair;
        if (changeLevel(stair))
        {
            trapMode = 0;
            return;
        }
    }
    if (c == '+')
    {
        if (player->passway == NULL)
        {
            player->passway = passways[0];
            player->passway->visiblePoint = 4;
            showPass(player->passway);
            player->passway->visiblePoint += 2;
        }
        else
        {
            if (player->passway->index != roomsCount - 2)
            {
                player->passway = passways[player->passway->index + 1];
                player->passway->visiblePoint = 4;
                showPass(player->passway);
                player->passway->visiblePoint += 1;
            }
        }
        refresh();
    }
    if (c == '#')
    {
        showPass(player->passway);
        player->passway->visiblePoint += 1;
        if (player->passway->visiblePoint == player->passway->count)
        {
            if (player->room->index != roomsCount - 1)
            {
                rooms[player->room->index + 1]->isVisible = true;
                printRoom(rooms[player->room->index + 1]);
            }
        }
    }
    if (c == '$')
    {
        {
            if (player->foodCount < 5)
            {
                findFood(cur, player->room);
                player->foodCount++;
                c = '.';
                showPlayeInfo(*player);
            }
        }
    }
    if (c == '%')
    {
        int max_y, max_x;
        getmaxyx(stdscr, max_y, max_x);
        int menu_height = 20;
        int menu_width = 60;
        int start_y = (max_y - menu_height) / 2;
        int start_x = (max_x - menu_width) / 2;
        WINDOW *menuWin = newwin(start_y, start_x, menu_height, menu_width);
        curs_set(0);
        refresh();
        box(menuWin, 0, 0);
        wrefresh(menuWin);
        mvwprintw(menuWin, 1, 16.5, "THE GAME ENDED SUCCESSFULLY");
        napms(3000);
        clear();
        show_menu();
    }
    if ((c == '.' || c == '+' || c == '#') && x >= 0 && x <= maxX && y > 3 && y <= maxY)
    {
        if (!trapMode)
        {
            mvprintw(player->cord.y, player->cord.x, c == '.' ? "." : c == '+' ? "."
                                                                               : "#");
        }
        else
        {
            trapMode = 0;
        }
        player->cord.x = x;
        player->cord.y = y;
        mvprintw(player->cord.y, player->cord.x, "@");
        refresh();
    }
    int trapIndex = isTrap(player->room, cur);
    if (trapIndex != -1)
    {
        player->room->traps[trapIndex].isVisible = true;
        player->health -= 3;
        trapMode = 1;
        showPlayeInfo(*player);
    }
    int index = inRoom(rooms, roomsCount, player->cord);
    if (index != -1)
    {
        player->room = rooms[index];
        rooms[index]->isVisible = true;
        player->state = 1;
    }
}

int changeLevel(Stair stair)
{
    int sourceIndex = stair.from;
    int destIndex = stair.to;
    if (destIndex > sourceIndex)
    {
        if (game->levels[destIndex] == NULL)
        {
            return 0;
        }
        game->currentLevel = destIndex;
        player->cord.x = game->levels[game->currentLevel]->rooms[0]->cord.x + 2;
        player->cord.y = game->levels[game->currentLevel]->rooms[0]->cord.y + 2;
        player->room = game->levels[game->currentLevel]->rooms[0];
        player->passway = NULL;
        player->level = game->currentLevel;
        clear();
        refresh();
        showLevel(game->levels[game->currentLevel]);
    }
    else
    {
        if (game->levels[destIndex] == NULL)
        {
            return 0;
        }
        game->currentLevel = destIndex;
        player->cord.x = game->levels[game->currentLevel]->rooms[game->levels[game->currentLevel]->roomsCount - 1]->cord.x + 2;
        player->cord.y = game->levels[game->currentLevel]->rooms[game->levels[game->currentLevel]->roomsCount - 1]->cord.y + 2;
        player->room = game->levels[game->currentLevel]->rooms[0];
        player->passway = game->levels[game->currentLevel]->passways[game->levels[game->currentLevel]->roomsCount - 2];
        player->level = game->currentLevel;
        clear();
        refresh();
        showLevel(game->levels[game->currentLevel]);
    }
    return 1;
}

int isTrap(Room *room, Point p)
{
    if (room == NULL)
        return -1;
    for (int i = 0; i < room->trapCount; i++)
    {
        if (room->traps[i].cord.x == p.x && room->traps[i].cord.y == p.y && !room->traps[i].isVisible)
        {
            return i;
        }
    }
    return -1;
}

void printStair(Room *room)
{
    if (room->stairCount)
        mvprintw(room->stair.cord.y, room->stair.cord.x, ">");
    refresh();
}

void printWin(Room *room)
{
    if (room->winCount)
        mvprintw(room->win.cord.y, room->win.cord.x, "%");
    refresh();
}

void printFoods(Room *room)
{
    for (int i = 0; i < room->foodCount; i++)
    {
        if (!room->foods[i].isUsed)
        {
            attron(COLOR_PAIR(3));
            mvprintw(room->foods[i].cord.y, room->foods[i].cord.x, "$");
            attroff(COLOR_PAIR(3));
        }
    }
    refresh();
}
Food findFood(Point p, Room *room)
{
    for (int i = 0; i < room->foodCount; i++)
    {
        if (room->foods[i].cord.x == p.x && room->foods[i].cord.y == p.y && !room->foods[i].isUsed)
        {
            room->foods[i].isUsed = true;
            return room->foods[i];
        }
    }
}

void printTraps(Room *room)
{
    for (int i = 0; i < room->trapCount; i++)
    {
        if (room->traps[i].isVisible)
            mvprintw(room->traps[i].cord.y, room->traps[i].cord.x, "^");
    }
    refresh();
}

void show_end_game()
{

    curs_set(0);
    keypad(stdscr, FALSE);
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    int win_height = 10, win_width = 90;
    int start_y = (max_y - win_height) / 2;
    int start_x = (max_x - win_width) / 2;

    WINDOW *win = newwin(win_height, win_width, start_y, start_x);
    box(win, 0, 0);
    wbkgd(win, COLOR_PAIR(2));
    mvwprintw(win, 2, 38.5, "GAME OVER...!");
    wrefresh(win);

    WINDOW *win1 = newwin(3, 10, start_y + 5, start_x + 30);
    box(win1, 0, 0);
    mvwprintw(win1, 1, 3, "MENU");
    wrefresh(win1);

    WINDOW *win2 = newwin(3, 10, start_y + 5, start_x + 50);
    box(win2, 0, 0);
    mvwprintw(win2, 1, 1, "NEW GAME");
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
                        new_game();
                        return;
                    }
                    else if (event.y == start_y + 6 && event.x >= start_x + 33 && event.x <= start_x + 37)
                    {
                        delwin(win);
                        clear();
                        refresh();
                        show_menu();
                        return;
                    }
                }
            }
        }
        else
            show_menu();
    }
    show_menu();
}
void exit_game()
{

    curs_set(0);
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    int win_height = 10, win_width = 70;
    int start_y = (max_y - win_height) / 2;
    int start_x = (max_x - win_width) / 2;

    WINDOW *win = newwin(win_height, win_width, start_y, start_x);
    box(win, 0, 0);
    wbkgd(win, COLOR_PAIR(3));
    wrefresh(win);

    WINDOW *win1 = newwin(3, 10, start_y + 5, start_x + 10);
    box(win1, 0, 0);
    wbkgd(win, COLOR_PAIR(4));
    mvwprintw(win1, 1, 3, "MENU");
    wrefresh(win1);

    WINDOW *win2 = newwin(3, 10, start_y + 5, start_x + 30);
    box(win2, 0, 0);
    wbkgd(win, COLOR_PAIR(4));
    mvwprintw(win2, 1, 1, "NEW GAME");
    wrefresh(win2);

    WINDOW *win3 = newwin(3, 10, start_y + 5, start_x + 50);
    box(win3, 0, 0);
    wbkgd(win, COLOR_PAIR(4));
    mvwprintw(win3, 1, 1, "CONTINUE");
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
                        new_game();
                        return;
                    }
                    else if (event.y == start_y + 6 && event.x >= start_x + 13 && event.x <= start_x + 17)
                    {
                        delwin(win);
                        clear();
                        refresh();
                        show_menu();
                        return;
                    }
                    else if (event.y == start_y + 6 && event.x >= start_x + 51 && event.x <= start_x + 59)
                    {
                        delwin(win);
                        refresh();
                        return;
                    }
                }
            }
        }
    }
    delwin(win);
}
