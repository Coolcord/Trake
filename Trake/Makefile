CXX=g++
CFLAGS=-std=c++11 -g -Wall
INCLUDE=`pkg-config --cflags --libs allegro-5.0 allegro_acodec-5.0 allegro_audio-5.0 allegro_color-5.0 allegro_dialog-5.0 allegro_font-5.0 allegro_image-5.0 allegro_main-5.0 allegro_memfile-5.0 allegro_physfs-5.0 allegro_primitives-5.0 allegro_ttf-5.0`

SOURCES=ai.cpp music.cpp collision_table.cpp pellet.cpp input.cpp rectangle.cpp snake_piece.cpp snake.cpp menu.cpp main.cpp
OBJS=$(SOURCES:.cpp=.o)
OUT=trake

all: clean make

make: $(OBJS)
	$(CXX) $(CFLAGS) -o $(OUT) $(OBJS) $(INCLUDE)

.cpp.o:
	$(CXX) $(CFLAGS) -c $< $(INCLUDE)

clean:
	rm -rf $(OUT) *.o

