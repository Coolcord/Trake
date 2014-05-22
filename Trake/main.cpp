#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_audio.h>
#include <string>
#include "input.h"
#include "ai.h"
#include "snake.h"
#include "pellet.h"
#include "collision_table.h"

/* trake 0 1 1
         ^ ^ ^
         | | |
1 for tron | |
  Num snakes |
        Num AI
*/
int main(int argc, char **argv){

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
    if (strcmp(argv[3], "1") == 0)
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

  Snake *snakes[4];
  for (int i = 0; i < 4; i++)
  {
    snakes[i] = NULL;
  }
  snakes[0] = new Snake(player_1_start_x, player_1_start_y, Input::LEFT, snake_length, al_color_name("lawngreen"), snake_width, max_x, max_y, true, collision_table, tron);
  if (num_snakes >= 2) snakes[1] = new Snake(player_2_start_x, player_2_start_y, Input::DOWN, snake_length, al_color_name("blue"), snake_width, max_x, max_y, true, collision_table, tron);
  if (num_snakes >= 3) snakes[2] = new Snake(player_3_start_x, player_3_start_y, Input::RIGHT, snake_length, al_color_name("red"), snake_width, max_x, max_y, true, collision_table, tron);
  if (num_snakes >= 4) snakes[3] = new Snake(player_4_start_x, player_4_start_y, Input::UP, snake_length, al_color_name("yellow"), snake_width, max_x, max_y, true, collision_table, tron);

  //AI
  AI *ai1 = NULL;
  AI *ai2 = NULL;
  AI *ai3 = NULL;
  AI *ai4 = NULL;

  if (num_snakes >= 1 && num_ai > 3)
    ai3 = new AI(snakes, 0, pellet, collision_table, tron);
  if (num_snakes > 1 && num_ai > 2)
    ai4 = new AI(snakes, 1, pellet, collision_table, tron);

  if (num_snakes > 2 && num_ai > 1)
    ai1 = new AI(snakes, 2, pellet, collision_table, tron);
  if (num_snakes > 3 && num_ai >= 1)
    ai2 = new AI(snakes, 3, pellet, collision_table, tron);

  al_flip_display();
  bool quit = false;
  float wait_time = 0.05;

  Input::Input_Thread_Data data;
  for (int i = 0; i < 4; i++)
  {
    data.snakes[i] = snakes[i];
  }
  data.event = event;
  data.quit = &quit;

  //Prepare input thread
  ALLEGRO_THREAD *input_thread = al_create_thread(Input::Input_Thread, &data);
  al_start_thread(input_thread);

  //Start music
  
  
  while (!quit)
  {
    pellet->handle_state();
    if (ai3) ai3->read_input();
    snakes[0]->move();
    if (ai4) ai4->read_input();
    if (snakes[1]) snakes[1]->move();
    if (ai1) ai1->read_input();
    if (snakes[2]) snakes[2]->move();
    if (ai2) ai2->read_input();
    if (snakes[3]) snakes[3]->move();
    if (wait_time > 0)
    {
      al_rest(wait_time);
      wait_time -= 0.00001; //slowly get faster
    }
    al_flip_display();
  }

  for (int i = 0; i < 4; i++)
  {
    delete snakes[i];
  }
  delete pellet;
  delete ai1;
  delete ai2;
  delete collision_table;
  al_destroy_display(display);

  return 0;
}

