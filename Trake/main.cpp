#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_color.h>
#include <string>
#include "input.h"
#include "snake.h"
#include "pellet.h"
#include "collision_table.h"

int main(int argc, char **argv){

  bool tron = false;
  if (argc > 1)
  {
    if (strcmp(argv[1], "1") == 0)
    {
      tron = true;
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
  int max_spawn_time = 100;

  Collision_Table *collision_table = new Collision_Table();
  Pellet *pellet = new Pellet(snake_width, max_x, max_y, max_spawn_time, collision_table, tron);
  Snake *snake1 = new Snake(player_1_start_x, 0, Input::DOWN, snake_length, al_color_name("lawngreen"), snake_width, max_x, max_y, true, collision_table, tron);
  Snake *snake2 = new Snake(player_2_start_x, 0, Input::DOWN, snake_length, al_color_name("blue"), snake_width, max_x, max_y, true, collision_table, tron);

  al_flip_display();
  bool quit = false;
  float wait_time = 0.05;

  Input::Input_Thread_Data data;
  data.snake1 = snake1;
  data.snake2 = snake2;
  data.event = event;
  data.quit = &quit;

  //Prepare input thread
  ALLEGRO_THREAD *thread_1 = al_create_thread(Input::Input_Thread, &data);
  al_start_thread(thread_1);
  
  while (!quit)
  {
    snake1->move();
    snake2->move();
    pellet->handle_state();
    if (wait_time > 0)
    {
      al_rest(wait_time);
      wait_time -= 0.00001; //slowly get faster
    }
    al_flip_display();
  }

  //al_clear_to_color(Color::Black());
  //al_flip_display();
  //al_rest(1);
  al_destroy_display(display);

  return 0;
}

