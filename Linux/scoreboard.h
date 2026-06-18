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
    Scoreboard(int screen_width, int screen_height, int snake_width, int font_small_incrementor, ALLEGRO_FONT *font_small, int y, int num_snakes, int player_scores[], Snake *snakes[]);
    ~Scoreboard();
    void draw();
    void draw_player_score(Snake *snake);
    void increment_score_by_one(Snake *snake);
    int get_player_score(int player_num);
  private:
    int m_screen_width;
    int m_screen_height;
    int m_snake_width;
    int m_font_small_incrementor;
    int m_y;
    int m_width;
    int m_height;
    int m_text_x[4];
    int m_text_y;
    int m_num_snakes;
    int m_player_scores[4];
    Snake *m_snakes[4];
    ALLEGRO_FONT *m_font;
    Rectangle *m_backgrounds[4];
};

#endif

