#ifndef INPUT_H
#define INPUT_H

#include <allegro5/allegro.h>

class AI;
class Controls;
class Snake;
class Music;
class Pause_Menu;

namespace Input
{
  struct Input_Thread_Data
  {
    AI *ai[4];
    Snake *snakes[4];
    Controls *controls;
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
  void read_input(AI *ai[], Snake *snakes[], Controls *controls, ALLEGRO_EVENT_QUEUE *event, bool *paused);
}

#endif

