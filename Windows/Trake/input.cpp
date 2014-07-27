#include "input.h"
#include "controls.h"
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
      Input::read_input(data->ai, data->snakes, data->controls, data->event, data->paused);
    }
    return NULL;
  }

  void read_input(AI *ai[], Snake *snakes[], Controls *controls, ALLEGRO_EVENT_QUEUE *event, bool *paused)
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
      switch(controls->get_control(e.keyboard.keycode))
      {
        case NONE:
          break;
        case Controls::PLAYER_1_CANCEL:
        case Controls::PLAYER_2_CANCEL:
        case Controls::PLAYER_3_CANCEL:
        case Controls::PLAYER_4_CANCEL:
          *paused = !(*paused);
          break;
        case Controls::PLAYER_1_LEFT:
          if (snakes[0] && !ai[0]) snakes[0]->change_direction(LEFT);
          break;
        case Controls::PLAYER_1_RIGHT:
          if (snakes[0] && !ai[0]) snakes[0]->change_direction(RIGHT);
          break;
        case Controls::PLAYER_1_DOWN:
          if (snakes[0] && !ai[0]) snakes[0]->change_direction(DOWN);
          break;
        case Controls::PLAYER_1_UP:
          if (snakes[0] && !ai[0]) snakes[0]->change_direction(UP);
          break;
        case Controls::PLAYER_2_LEFT:
          if (snakes[1] && !ai[1]) snakes[1]->change_direction(LEFT);
          break;
        case Controls::PLAYER_2_RIGHT:
          if (snakes[1] && !ai[1]) snakes[1]->change_direction(RIGHT);
          break;
        case Controls::PLAYER_2_DOWN:
          if (snakes[1] && !ai[1]) snakes[1]->change_direction(DOWN);
          break;
        case Controls::PLAYER_2_UP:
          if (snakes[1] && !ai[1]) snakes[1]->change_direction(UP);
          break;
        case Controls::PLAYER_3_LEFT:
          if (snakes[2] && !ai[2]) snakes[2]->change_direction(LEFT);
          break;
        case Controls::PLAYER_3_RIGHT:
          if (snakes[2] && !ai[2]) snakes[2]->change_direction(RIGHT);
          break;
        case Controls::PLAYER_3_DOWN:
          if (snakes[2] && !ai[2]) snakes[2]->change_direction(DOWN);
          break;
        case Controls::PLAYER_3_UP:
          if (snakes[2] && !ai[2]) snakes[2]->change_direction(UP);
          break;
        case Controls::PLAYER_4_LEFT:
          if (snakes[3] && !ai[3]) snakes[3]->change_direction(LEFT);
          break;
        case Controls::PLAYER_4_RIGHT:
          if (snakes[3] && !ai[3]) snakes[3]->change_direction(RIGHT);
          break;
        case Controls::PLAYER_4_DOWN:
          if (snakes[3] && !ai[3]) snakes[3]->change_direction(DOWN);
          break;
        case Controls::PLAYER_4_UP:
          if (snakes[3] && !ai[3]) snakes[3]->change_direction(UP);
          break;
        default:
          break;
      }
    }
  }
}

