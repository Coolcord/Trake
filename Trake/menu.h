#ifndef MENU_H
#define MENU_H

#include "rectangle.h"
#include <vector>

class Menu
{
  public:
    Menu(float screen_width, float screen_height, float snake_width);
    ~Menu();
    void show();
  private:
    void draw();
    void create_t(float x, float y);
    void create_r(float x, float y);
    void create_title(float x, float y);
    float m_screen_width;
    float m_screen_height;
    float m_snake_width;
    std::vector<Rectangle*> *m_title;
};

#endif

