CXX=g++
CFLAGS=-std=c++11 -g -Wall
INCLUDE=`pkg-config --cflags --libs allegro-5 allegro_acodec-5 allegro_audio-5 allegro_color-5 allegro_dialog-5 allegro_font-5 allegro_image-5 allegro_main-5 allegro_memfile-5 allegro_physfs-5 allegro_primitives-5 allegro_ttf-5`

SOURCES=ai.cpp controls.cpp save_file_manager.cpp music.cpp collision_table.cpp pellet.cpp input.cpp rectangle.cpp snake_piece.cpp snake.cpp scoreboard.cpp menu.cpp pause_menu.cpp game.cpp main.cpp
OBJS=$(SOURCES:.cpp=.o)
OUT=Trake

make: $(OBJS)
	$(CXX) $(CFLAGS) -o $(OUT) $(OBJS) $(INCLUDE)

.cpp.o:
	$(CXX) $(CFLAGS) -c $< $(INCLUDE)

clean:
	rm -rf $(OUT) *.o

