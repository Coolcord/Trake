#ifndef HIGH_SCORES_H
#define HIGH_SCORES_H

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <array>
#include <string>

class High_Scores
{
  public:
    High_Scores(float font_small_incrementor, float font_large_incrementor, ALLEGRO_FONT *font_small, ALLEGRO_FONT *font_large);
    High_Scores(std::array<std::pair<std::string, int>, 10> *snake_high_scores, std::array<std::pair<std::string, int>, 10> *tron_high_scores, float font_small_incrementor, float font_large_incrementor, ALLEGRO_FONT *font_small, ALLEGRO_FONT *font_large);
    ~High_Scores();
    void show_high_scores(bool tron);
    int get_highest_score(bool tron);
    void set_high_scores(std::array<std::pair<std::string, int>, 10> *high_scores, bool tron);
  private:
    std::array<std::pair<std::string, int>, 10> *m_snake_high_scores;
    std::array<std::pair<std::string, int>, 10> *m_tron_high_scores;
    float m_font_small_incrementor;
    float m_font_large_incrementor;
    ALLEGRO_FONT *m_font_small;
    ALLEGRO_FONT *m_font_large;
};

#endif

