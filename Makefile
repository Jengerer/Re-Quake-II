CC=g++
AR=ar
CFLAGS=-Wall -g -std=c++11
INCLUDES=-I/usr/include/SDL2
LIBDIR=-L./
LIBS=-lSDL2 -lGL -lGLEW -lm -lengine -lopengl_renderer -lplatformer -lcommon

all: jengine

jengine: main.o libengine.a libopengl_renderer.a libplatformer.a libcommon.a
	$(CC) -o $@ $^ $(INCLUDES) $(LIBDIR) $(LIBS)

libengine.a: engine.o window.o renderer.o game.o keyboard_manager.o
	$(AR) rcs $@ $^

libopengl_renderer.a: opengl_renderer.o opengl_model.o polygon.o indexed_mesh.o mesh.o
	$(AR) rcs $@ $^

libplatformer.a: platformer.o map.o player.o player_move.o
	$(AR) rcs $@ $^

libcommon.a: vector3d.o vector2d.o file.o
	$(AR) rcs $@ $^

%.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS) $(INCLUDES)

clean:
	rm -f *.o *.a
