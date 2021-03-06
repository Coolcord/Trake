#ifndef SNAKE_H
#define SNAKE_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <vector>
#include "input.h"
class Snake_Piece;
class Simple_Rectangle;
class Scoreboard;
class Collision_Table;

class Snake
{
  public:
    Snake(int player_num, float x, float y, float volume, Input::Direction direction, int size, ALLEGRO_COLOR color, float width, float max_x, float max_y, bool wrap, Collision_Table *collision_table, bool tron);
    ~Snake();
    void move();
    Simple_Rectangle *create_rectangle(Input::Direction direction, float x, float y, int posiiton);
    void draw();
    void change_direction(Input::Direction direction);
    float get_x();
    float get_y();
    float get_width();
    Input::Direction get_direction();
    float get_max_x();
    float get_max_y();
    void grow(int value);
    bool is_dead();
    void kill();
    float get_eat_sound_speed();
    int get_player_num();
    void set_scoreboard(Scoreboard *scoreboard);
  private:
    int m_player_num;
    float m_width; //width and height of each piece
    int m_size; //number of pieces
    ALLEGRO_COLOR m_color;
    std::vector<Snake_Piece*> *m_pieces;
    int m_grow; //the amount left for the snake to grow
    float m_max_x;
    float m_max_y;
    float m_volume;
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

