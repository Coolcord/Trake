#include "high_scores.h"
#include <allegro5/allegro5.h>
#include <assert.h>

High_Scores::High_Scores()
{
  m_snake_high_scores = NULL;
  m_tron_high_scores = NULL;
}

High_Scores::High_Scores(std::list<int> *snake_high_scores, std::list<int> *tron_high_scores)
{
  m_snake_high_scores = snake_high_scores;
  m_tron_high_scores = tron_high_scores;
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
    return m_tron_high_scores->front();
  }
  else
  {
    assert(m_snake_high_scores);
    assert(m_snake_high_scores->size() == 10);
    return m_snake_high_scores->front();
  }
}


