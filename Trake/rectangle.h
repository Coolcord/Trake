#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <allegro5/allegro.h>

class Rectangle
{
  public:
    Rectangle(float x, float y, float width, float height, bool filled, ALLEGRO_COLOR color);
    ~Rectangle() {}
    void draw();
    float get_x();
    float get_y();
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

