#ifndef INPUT_H
#define INPUT_H

#include <allegro5/allegro.h>

class AI;
class Snake;

namespace Input
{
  struct Input_Thread_Data
  {
    AI *ai[4];
    Snake *snakes[4];
    ALLEGRO_EVENT_QUEUE *event;
    bool *paused;
    bool *quit;
  };

  enum Direction
  {
    NONE = 0,
    LEFT = 1,
    RIGHT = 2,
    DOWN = 4,
    UP = 8
  };

  void *Input_Thread(ALLEGRO_THREAD *thread, void *arg);
  void read_input(AI *ai[], Snake *snakes[], ALLEGRO_EVENT_QUEUE *event, bool *paused, bool *quit);
}

#endif

