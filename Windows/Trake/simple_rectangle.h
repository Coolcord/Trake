#ifndef SIMPLE_RECTANGLE_H
#define SIMPLE_RECTANGLE_H

#include <allegro5/allegro.h>

class Simple_Rectangle
{
  public:
    Simple_Rectangle(float x, float y, float width, float height, bool filled, ALLEGRO_COLOR color);
    Simple_Rectangle(float x, float y, float width, float height, bool filled, ALLEGRO_COLOR color, float thickness);
    ~Simple_Rectangle() {}
    void draw();
    float get_x();
    float get_y();
    float get_width();
    float get_height();
    ALLEGRO_COLOR get_color();
    void set_x(float x);
    void set_y(float y);
    void set_color(ALLEGRO_COLOR color);
    void set_filled(bool filled);
  private:
    float m_x;
    float m_y;
    float m_width;
    float m_height;
    float m_thickness;
    bool m_filled;
    ALLEGRO_COLOR m_color;
};

#endif

