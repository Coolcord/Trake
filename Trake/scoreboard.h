#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

class Snake;
class Rectangle;

class Scoreboard
{
  public:
    Scoreboard(float screen_width, float screen_height, float max_x, float max_y, int num_snakes, Snake *snakes[]);
    ~Scoreboard();
    void draw();
  private:
    float m_screen_width;
    float m_screen_height;
    float m_max_x;
    float m_max_y;
    float m_width;
    float m_height;
    int m_num_snakes;
    Snake *m_snakes[4];
    ALLEGRO_FONT *m_font;
    Rectangle *m_backgrounds[4];
};

#endif

