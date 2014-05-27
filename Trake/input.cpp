#include "input.h"
#include "ai.h"
#include "snake.h"
#include "music.h"
#include "pause_menu.h"

namespace Input
{
  void *Input_Thread(ALLEGRO_THREAD *thread, void *arg)
  {
    Input::Input_Thread_Data *data = (Input::Input_Thread_Data*)arg;

    while (!(*(data->quit)))
    {
      Input::read_input(data->ai, data->snakes, data->event, data->paused);
    }
    return NULL;
  }

  void read_input(AI *ai[], Snake *snakes[], ALLEGRO_EVENT_QUEUE *event, bool *paused)
  {
    if (*paused)
    {
      al_rest(0.1);
      return;
    }
    ALLEGRO_EVENT e;
    al_wait_for_event_timed(event, &e, 3);
    if (e.type == ALLEGRO_EVENT_KEY_DOWN)
    {
      switch(e.keyboard.keycode)
      {
        case ALLEGRO_KEY_ESCAPE:
          *paused = !(*paused);
          break;
        case ALLEGRO_KEY_LEFT:
          if (snakes[0] && !ai[0]) snakes[0]->change_direction(Direction::LEFT);
          break;
        case ALLEGRO_KEY_RIGHT:
          if (snakes[0] && !ai[0]) snakes[0]->change_direction(Direction::RIGHT);
          break;
        case ALLEGRO_KEY_DOWN:
          if (snakes[0] && !ai[0]) snakes[0]->change_direction(Direction::DOWN);
          break;
        case ALLEGRO_KEY_UP:
          if (snakes[0] && !ai[0]) snakes[0]->change_direction(Direction::UP);
          break;
        case ALLEGRO_KEY_A:
          if (snakes[1] && !ai[1]) snakes[1]->change_direction(Direction::LEFT);
          break;
        case ALLEGRO_KEY_D:
          if (snakes[1] && !ai[1]) snakes[1]->change_direction(Direction::RIGHT);
          break;
        case ALLEGRO_KEY_S:
          if (snakes[1] && !ai[1]) snakes[1]->change_direction(Direction::DOWN);
          break;
        case ALLEGRO_KEY_W:
          if (snakes[1] && !ai[1]) snakes[1]->change_direction(Direction::UP);
          break;
        default:
          break;
      }
    }
  }
}

