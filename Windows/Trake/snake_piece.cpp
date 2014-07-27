#include "snake_piece.h"
#include "simple_rectangle.h"

Snake_Piece::Snake_Piece(Input::Direction direction, Type type, Simple_Rectangle *rectangle)
{
  assert(rectangle);
  m_direction = direction;
  m_type = type;
  m_rectangle = rectangle;
}

Snake_Piece::~Snake_Piece()
{
  delete m_rectangle;
}

void Snake_Piece::draw()
{
  m_rectangle->draw();
}

float Snake_Piece::get_x()
{
  return m_rectangle->get_x();
}

float Snake_Piece::get_y()
{
  return m_rectangle->get_y();
}

Input::Direction Snake_Piece::get_direction()
{
  return m_direction;
}

Snake_Piece::Type Snake_Piece::get_type()
{
  return m_type;
}

ALLEGRO_COLOR Snake_Piece::get_color()
{
  return m_rectangle->get_color();
}

void Snake_Piece::set_x(float x)
{
  m_rectangle->set_x(x);
}

void Snake_Piece::set_y(float y)
{
  m_rectangle->set_y(y);
}

void Snake_Piece::set_direction(Input::Direction direction)
{
  m_direction = direction;
}

void Snake_Piece::set_type(Snake_Piece::Type type)
{
  m_type = type;
}

void Snake_Piece::set_color(ALLEGRO_COLOR color)
{
  m_rectangle->set_color(color);
}

void Snake_Piece::set_filled(bool filled)
{
  m_rectangle->set_filled(filled);
}

