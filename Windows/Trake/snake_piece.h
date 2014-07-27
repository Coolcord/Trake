#ifndef SNAKE_PIECE_H
#define SNAKE_PIECE_H

#include <allegro5/allegro.h>
#include "input.h"

class Simple_Rectangle;

class Snake_Piece
{
  public:
    enum Type
    {
      HEAD = 1,
      BODY = 2,
      TAIL = 3
    };
    Snake_Piece(Input::Direction direction, Type type, Simple_Rectangle *rectangle);
    ~Snake_Piece();
    void draw();
    float get_x();
    float get_y();
    Input::Direction get_direction();
    Type get_type();
    ALLEGRO_COLOR get_color();
    void set_x(float x);
    void set_y(float y);
    void set_direction(Input::Direction direction);
    void set_type(Type type);
    void set_color(ALLEGRO_COLOR color);
    void set_filled(bool filled);
  private:
    Input::Direction m_direction;
    Type m_type;
	Simple_Rectangle *m_rectangle;
};


#endif

