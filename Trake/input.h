#ifndef INPUT_H
#define INPUT_H

#include <allegro5/allegro.h>

class Snake;

namespace Input
{
  struct Input_Thread_Data
  {
    Snake *snake1;
    Snake *snake2;
    ALLEGRO_EVENT_QUEUE *event;
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
  void read_input(Snake *snake1, Snake *snake2, ALLEGRO_EVENT_QUEUE *event, bool *quit);
}

#endif

