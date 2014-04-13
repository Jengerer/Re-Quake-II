CC=gcc
CFLAGS=-Wall -g
LIBS=-lSDL2 -lGL -lGLEW

all: jengine

jengine: main.o window.o opengl_renderer.o file.o world.o map.o polygon.o vector3d.o
	$(CC) -o $@ $^ $(LIBS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -f *.o
