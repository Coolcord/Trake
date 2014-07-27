#include "simple_rectangle.h"
#include <allegro5/allegro_primitives.h>

Simple_Rectangle::Simple_Rectangle(float x, float y, float width, float height, bool filled, ALLEGRO_COLOR color)
{
  m_x = x;
  m_y = y;
  m_width = width;
  m_height = height;
  m_filled = filled;
  m_color = color;
  m_thickness = 1;
}

Simple_Rectangle::Simple_Rectangle(float x, float y, float width, float height, bool filled, ALLEGRO_COLOR color, float thickness)
{
  m_x = x;
  m_y = y;
  m_width = width;
  m_height = height;
  m_filled = filled;
  m_color = color;
  m_thickness = thickness;
}

void Simple_Rectangle::draw()
{
  if (m_filled)
  {
    al_draw_filled_rectangle(m_x, m_y, m_x + m_width, m_y + m_height, m_color);
  }
  else
  {
    al_draw_rectangle(m_x+m_thickness, m_y+m_thickness, m_x + m_width-m_thickness, m_y + m_height-m_thickness, m_color, m_thickness);
  }
}

float Simple_Rectangle::get_x()
{
  return m_x;
}

float Simple_Rectangle::get_y()
{
  return m_y;
}

float Simple_Rectangle::get_width()
{
  return m_width;
}

float Simple_Rectangle::get_height()
{
  return m_height;
}

ALLEGRO_COLOR Simple_Rectangle::get_color()
{
  return m_color;
}

void Simple_Rectangle::set_x(float x)
{
  m_x = x;
}

void Simple_Rectangle::set_y(float y)
{
  m_y = y;
}

void Simple_Rectangle::set_color(ALLEGRO_COLOR color)
{
  m_color = color;
}

void Simple_Rectangle::set_filled(bool filled)
{
  m_filled = filled;
}

