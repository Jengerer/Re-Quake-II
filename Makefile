CC=gcc
CFLAGS=-Wall -Werror -g
LIBS=-lSDL2 -lGL -lGLEW -lm
INCLUDE=-I/usr/include/SDL2

all: jengine

jengine: main.o engine.o window.o keyboard_manager.o renderer.o opengl_renderer.o opengl_model.o indexed_mesh.o mesh.o game.o platformer.o player.o player_move.o file.o world.o map.o polygon.o vector3d.o vector2d.o
	$(CC) -o $@ $^ $(LIBS)

%.o: %.c
	$(CC) -c -o $@ $< $(INCLUDE) $(CFLAGS)

clean:
	rm -f *.o
