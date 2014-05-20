#include "input.h"
#include "snake.h"

namespace Input
{
  void *Input_Thread(ALLEGRO_THREAD *thread, void *arg)
  {
    Input::Input_Thread_Data *data = (Input::Input_Thread_Data*)arg;

    while (!(*(data->quit)))
    {
      Input::read_input(data->snakes, data->event, data->quit);
    }
    return NULL;
  }

  void read_input(Snake *snakes[], ALLEGRO_EVENT_QUEUE *event, bool *quit)
  {
    ALLEGRO_EVENT e;
    al_wait_for_event(event, &e);
    if (e.type == ALLEGRO_EVENT_KEY_DOWN)
    {
      if (e.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
      {
        *quit = true;
      }
      else
      {
        switch(e.keyboard.keycode)
        {
          case ALLEGRO_KEY_LEFT:
            snakes[0]->change_direction(Direction::LEFT);
            break;
          case ALLEGRO_KEY_RIGHT:
            snakes[0]->change_direction(Direction::RIGHT);
            break;
          case ALLEGRO_KEY_DOWN:
            snakes[0]->change_direction(Direction::DOWN);
            break;
          case ALLEGRO_KEY_UP:
            snakes[0]->change_direction(Direction::UP);
            break;
          case ALLEGRO_KEY_A:
            if (snakes[1]) snakes[1]->change_direction(Direction::LEFT);
            break;
          case ALLEGRO_KEY_D:
            if (snakes[1]) snakes[1]->change_direction(Direction::RIGHT);
            break;
          case ALLEGRO_KEY_S:
            if (snakes[1]) snakes[1]->change_direction(Direction::DOWN);
            break;
          case ALLEGRO_KEY_W:
            if (snakes[1]) snakes[1]->change_direction(Direction::UP);
            break;
          default:
            break;
        }
      }
    }
  }
}

