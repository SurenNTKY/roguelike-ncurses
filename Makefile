CC = gcc
CFLAGS = -Wall -Wextra -fsanitize=address -g `sdl2-config --cflags`
LDFLAGS = `sdl2-config --libs` -lSDL2_mixer
LIBS = -lSDL2 -lSDL2_mixer -lncurses

TARGET = game
SOURCES = main.c create_user.c guest.c register_user.c login.c new_game.c continue_game.c scoreboard.c settings.c profile.c
OBJECTS = $(SOURCES:.c=.o)

$(TARGET): $(OBJECTS) Makefile
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) $(LDFLAGS) $(LIBS)

%.o: %.c Makefile
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)
