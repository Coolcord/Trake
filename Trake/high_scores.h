#ifndef HIGH_SCORES_H
#define HIGH_SCORES_H

#include <list>

class High_Scores
{
  public:
    High_Scores();
    High_Scores(std::list<int> *snake_high_scores, std::list<int> *tron_high_scores);
    ~High_Scores();
    void show_high_scores(bool tron);
    int get_highest_score(bool tron);
    void set_high_scores(std::list<int> *high_scores, bool tron);
  private:
    std::list<int> *m_snake_high_scores;
    std::list<int> *m_tron_high_scores;
};

#endif

