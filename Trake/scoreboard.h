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
    Scoreboard(float screen_width, float screen_height, float snake_width, float y, int num_snakes, Snake *snakes[]);
    ~Scoreboard();
    void draw();
    void draw_player_score(Snake *snake);
    void increment_score_by_one(Snake *snake);
  private:
    float m_screen_width;
    float m_screen_height;
    float m_snake_width;
    float m_y;
    float m_width;
    float m_height;
    float m_text_x[4];
    float m_text_y;
    int m_num_snakes;
    int m_player_score[4];
    Snake *m_snakes[4];
    ALLEGRO_FONT *m_font;
    Rectangle *m_backgrounds[4];
};

#endif

