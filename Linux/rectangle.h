#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <allegro5/allegro.h>

class Rectangle
{
  public:
    Rectangle(int x, int y, int width, int height, bool filled, ALLEGRO_COLOR color);
    Rectangle(int x, int y, int width, int height, bool filled, ALLEGRO_COLOR color, int thickness);
    ~Rectangle() {}
    void draw();
    int get_x();
    int get_y();
    int get_width();
    int get_height();
    ALLEGRO_COLOR get_color();
    void set_x(int x);
    void set_y(int y);
    void set_color(ALLEGRO_COLOR color);
    void set_filled(bool filled);
  private:
    int m_x;
    int m_y;
    int m_width;
    int m_height;
    int m_thickness;
    bool m_filled;
    ALLEGRO_COLOR m_color;
};

#endif

