#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <string>
#include "menu.h"

int main(int argc, char **argv){

  srand(time(NULL));
  
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
  al_init_font_addon();
  al_init_ttf_addon();
  al_reserve_samples(32);
  ALLEGRO_EVENT_QUEUE *event = NULL;
  event = al_create_event_queue();
  al_register_event_source(event, al_get_keyboard_event_source());
  al_register_event_source(event, al_get_display_event_source(display));
  float snake_width = screen_width/100;

  //Show start menu
  Menu *menu = new Menu(event, screen_width, screen_height, snake_width);
  menu->show();

  delete menu;
  al_destroy_event_queue(event);
  al_destroy_display(display);
  return 0;
}

