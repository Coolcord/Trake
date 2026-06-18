#ifndef SNAKE_H
#define SNAKE_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <vector>
#include "input.h"
class Snake_Piece;
class Rectangle;
class Scoreboard;
class Collision_Table;

class Snake
{
  public:
    Snake(int player_num, int x, int y, int volume, Input::Direction direction, int size, ALLEGRO_COLOR color, int width, int max_x, int max_y, bool wrap, Collision_Table *collision_table, bool tron);
    ~Snake();
    void move();
    Rectangle *create_rectangle(Input::Direction direction, int x, int y, int posiiton);
    void draw();
    void change_direction(Input::Direction direction);
    int get_x();
    int get_y();
    int get_width();
    Input::Direction get_direction();
    int get_max_x();
    int get_max_y();
    void grow(int value);
    bool is_dead();
    void kill();
    float get_eat_sound_speed();
    int get_player_num();
    void set_scoreboard(Scoreboard *scoreboard);
  private:
    int m_player_num;
    int m_width; //width and height of each piece
    int m_size; //number of pieces
    ALLEGRO_COLOR m_color;
    std::vector<Snake_Piece*> *m_pieces;
    int m_grow; //the amount left for the snake to grow
    int m_max_x;
    int m_max_y;
    int m_volume;
    bool m_wrap;
    Scoreboard *m_scoreboard;
    Collision_Table *m_collision_table;
    bool m_changing_direction;
    Input::Direction m_next_direction;
    bool m_dead;
    bool m_tron;
    float m_eat_sound_speed;
    ALLEGRO_SAMPLE *m_dead_sound;
};

#endif

