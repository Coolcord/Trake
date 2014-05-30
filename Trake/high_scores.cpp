#include "high_scores.h"
#include <assert.h>

High_Scores::High_Scores(float font_small_incrementor, float font_large_incrementor, ALLEGRO_FONT *font_small, ALLEGRO_FONT *font_large)
{
  m_snake_high_scores = new std::array<std::pair<std::string, int>, 10>();
  m_tron_high_scores = new std::array<std::pair<std::string, int>, 10>();
  m_font_small_incrementor = font_small_incrementor;
  m_font_large_incrementor = font_large_incrementor;
  m_font_small = font_small;
  m_font_large = font_large;
}

High_Scores::High_Scores(std::array<std::pair<std::string, int>, 10> *snake_high_scores, std::array<std::pair<std::string, int>, 10> *tron_high_scores, float font_small_incrementor, float font_large_incrementor, ALLEGRO_FONT *font_small, ALLEGRO_FONT *font_large)
{
  m_snake_high_scores = snake_high_scores;
  m_tron_high_scores = tron_high_scores;
  m_font_small_incrementor = font_small_incrementor;
  m_font_large_incrementor = font_large_incrementor;
  m_font_small = font_small;
  m_font_large = font_large;
}

High_Scores::~High_Scores()
{
  delete m_snake_high_scores;
  delete m_tron_high_scores;
}

void High_Scores::show_high_scores(bool tron)
{

}

int High_Scores::get_highest_score(bool tron)
{
  if (tron)
  {
    assert(m_tron_high_scores);
    assert(m_tron_high_scores->size() == 10);
    return m_tron_high_scores->front().second;
  }
  else
  {
    assert(m_snake_high_scores);
    assert(m_snake_high_scores->size() == 10);
    return m_snake_high_scores->front().second;
  }
}


