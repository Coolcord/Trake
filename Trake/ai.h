#ifndef AI_H
#define AI_H

class Snake;

class AI
{
  public:
    AI(Snake *snake, bool tron);
    ~AI();
  private:
    Snake *m_snake;
    bool m_tron;
};

#endif

