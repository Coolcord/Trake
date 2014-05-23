#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <string>
#include "input.h"
#include "ai.h"
#include "snake.h"
#include "pellet.h"
#include "music.h"
#include "collision_table.h"

/* trake 0 1 1
         ^ ^ ^
         | | |
1 for tron | |
  Num snakes |
        Num AI
*/
int main(int argc, char **argv){

  srand(time(NULL));
  bool tron = false;
  int num_snakes = 4;
  int num_ai = 2;
  if (argc > 1)
  {
    if (strcmp(argv[1], "1") == 0)
    {
      tron = true;
    }
  }
  if (argc > 2)
  {
    if (strcmp(argv[2], "1") == 0)
    {
      num_snakes = 1;
    }
    else if (strcmp(argv[2], "2") == 0)
    {
      num_snakes = 2;
    }
    else if (strcmp(argv[2], "3") == 0)
    {
      num_snakes = 3;
    }
    else if (strcmp(argv[2], "4") == 0)
    {
      num_snakes = 4;
    }
  }
  if (argc > 3)
  {
    if (strcmp(argv[3], "0") == 0)
    {
      num_ai = 0;
    }
    else if (strcmp(argv[3], "1") == 0)
    {
      num_ai = 1;
    }
    else if (strcmp(argv[3], "2") == 0)
    {
      num_ai = 2;
    }
    else if (strcmp(argv[3], "3") == 0)
    {
      num_ai = 3;
    }
    else if (strcmp(argv[3], "4") == 0)
    {
      num_ai = 4;
    }
  }
  ALLEGRO_DISPLAY *display = NULL;
  ALLEGRO_DISPLAY_MODE disp_data;

  if(!al_init()) {
     fprintf(stderr, "failed to initialize allegro!\n");
     return -1;
  }
 
  al_get_display_mode(al_get_num_display_modes() - 1, &disp_data);
  al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
  al_set_new_display_option(ALLEGRO_VSYNC, 1, ALLEGRO_DONTCARE);
  display = al_create_display(disp_data.width, disp_data.height);
  al_hide_mouse_cursor(display);
  if(!display) {
    fprintf(stderr, "failed to create display!\n");
    return -1;
  }

  int screen_width = al_get_display_width(display);
  int screen_height = al_get_display_height(display);
 
  al_clear_to_color(al_color_name("black"));
  al_flip_display();

  //Events
  al_install_keyboard();
  al_install_audio();
  al_init_acodec_addon();
  al_reserve_samples(32);
  ALLEGRO_EVENT_QUEUE *event = NULL;
  event = al_create_event_queue();
  al_register_event_source(event, al_get_keyboard_event_source());
  al_register_event_source(event, al_get_display_event_source(display));

  int snake_length = 5;
  float snake_width = screen_width/100;
  float max_x = 0;
  float max_y = 0;
  while (max_x < screen_width-(snake_width*2))
  {
    max_x += snake_width;
  }
  while (max_y < screen_height-(snake_width*2))
  {
    max_y += snake_width;
  }
  float player_1_start_x = max_x;
  float player_2_start_x = 0;
  float player_3_start_x = 0;
  float player_4_start_x = max_x;
  float player_1_start_y = 0;
  float player_2_start_y = 0;
  float player_3_start_y = max_y;
  float player_4_start_y = max_y;
  int max_spawn_time = 100;

  Collision_Table *collision_table = new Collision_Table();
  Pellet *pellet = new Pellet(snake_width, max_x, max_y, max_spawn_time, collision_table, tron);

  AI *ai[4];
  Snake *snakes[4];
  for (int i = 0; i < 4; i++)
  {
    ai[i] = NULL;
    snakes[i] = NULL;
  }
  snakes[0] = new Snake(0, player_1_start_x, player_1_start_y, Input::LEFT, snake_length, al_color_name("lawngreen"), snake_width, max_x, max_y, true, collision_table, tron);
  if (num_snakes >= 2) snakes[1] = new Snake(1, player_2_start_x, player_2_start_y, Input::DOWN, snake_length, al_color_name("blue"), snake_width, max_x, max_y, true, collision_table, tron);
  if (num_snakes >= 3) snakes[2] = new Snake(2, player_3_start_x, player_3_start_y, Input::RIGHT, snake_length, al_color_name("red"), snake_width, max_x, max_y, true, collision_table, tron);
  if (num_snakes >= 4) snakes[3] = new Snake(3, player_4_start_x, player_4_start_y, Input::UP, snake_length, al_color_name("yellow"), snake_width, max_x, max_y, true, collision_table, tron);

  //AI
  for (int i = 0; i < 4; i++)
  {
    if (num_snakes >= i+1 && num_snakes - num_ai < i+1)
      ai[i] = new AI(snakes, i, pellet, collision_table, tron);
  }

  //Start Music
  Music *music = new Music(tron);
  music->play();

  al_flip_display();
  bool paused = false;
  bool quit = false;
  float wait_time = 0.05;

  Input::Input_Thread_Data data;
  for (int i = 0; i < 4; i++)
  {
    data.ai[i] = ai[i];
    data.snakes[i] = snakes[i];
  }
  data.event = event;
  data.paused = &paused;
  data.quit = &quit;

  //Prepare input thread
  ALLEGRO_THREAD *input_thread = al_create_thread(Input::Input_Thread, &data);
  al_start_thread(input_thread);

  while (!quit)
  {
    if (paused)
    {
      al_rest(0.1);
      continue;
    }

    pellet->handle_state();
    for (int i = 0; i < 4; i++)
    {
      if (ai[i]) ai[i]->read_input();
      if (snakes[i]) snakes[i]->move();
    }
    if (wait_time > 0)
    {
      al_rest(wait_time);
      wait_time -= 0.00001; //slowly get faster
    }
    al_flip_display();
  }
  music->slow_to_stop();

  //al_destroy_sample(soundEffect);
  for (int i = 0; i < 4; i++)
  {
    delete snakes[i];
    delete ai[i];
  }
  delete pellet;
  delete collision_table;
  delete music;
  al_destroy_display(display);

  return 0;
}

