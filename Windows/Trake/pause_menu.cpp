#include "pause_menu.h"
#include "simple_rectangle.h"
#include "controls.h"
#include <allegro5/allegro_color.h>
#include <assert.h>
#include <string>

Pause_Menu::Pause_Menu(ALLEGRO_EVENT_QUEUE *event, Controls *controls, float screen_width, float screen_height, float font_large_incrementor, ALLEGRO_FONT *font_large, ALLEGRO_SAMPLE *move_sound_down, ALLEGRO_SAMPLE *move_sound_up, float sound_effects_level, bool *quit, bool *hide_standing, int *rounds, int total_rounds)
{
  assert(event);
  assert(controls);
  assert(move_sound_down);
  assert(move_sound_up);
  m_event = event;
  m_controls = controls;
  m_screen_width = screen_width;
  m_screen_height = screen_height;
  m_selection = 0;
  m_font_large_incrementor = font_large_incrementor;
  m_font = font_large;
  m_move_sound_down = move_sound_down;
  m_move_sound_up = move_sound_up;
  m_sound_effects_level = sound_effects_level;
  m_rectangle = new Simple_Rectangle(m_screen_width/4, m_screen_height/4, m_screen_width/2, m_screen_height/2, true, al_color_name("darkgray"));
  m_quit = quit;
  m_hide_standing = hide_standing;
  m_rounds = rounds;
  m_total_rounds = total_rounds;
}

Pause_Menu::~Pause_Menu()
{
  delete m_rectangle;
}

void Pause_Menu::draw()
{
  m_rectangle->draw();
  ALLEGRO_COLOR color;
  float x = m_screen_width / 2;
  float y = m_rectangle->get_y() + (m_rectangle->get_height()/4);
  al_draw_text(m_font, al_color_name("black"), x, y, ALLEGRO_ALIGN_CENTER, "Quit?");
  std::string items[2] = { "No", "Yes" };
  y = m_rectangle->get_y() + (m_rectangle->get_height()/2);
  for (int i = 0; i < 2; i++)
  {
    if (m_selection == i)
      color = al_color_name("lawngreen");
    else
      color = al_color_name("black");
    if (i == 0)
      x = m_rectangle->get_x() + (m_rectangle->get_width()/4);
    else
      x = m_rectangle->get_x() + ((m_rectangle->get_width()/4)*3);
    al_draw_text(m_font, color, x, y, ALLEGRO_ALIGN_CENTER, items[i].c_str());
  }
}

void Pause_Menu::show()
{
  bool paused = true;
  while (paused)
  {
    this->draw();
	al_flip_display();

    ALLEGRO_EVENT e;
    al_wait_for_event(m_event, &e);
    if (e.type == ALLEGRO_EVENT_KEY_DOWN)
    {
      switch(m_controls->get_control(e.keyboard.keycode))
      {
        case Controls::NONE:
          break;
        case Controls::PLAYER_1_LEFT:
        case Controls::PLAYER_2_LEFT:
        case Controls::PLAYER_3_LEFT:
        case Controls::PLAYER_4_LEFT:
          if (m_selection == 1)
          {
            if (m_move_sound_down) al_play_sample(m_move_sound_down, 2.5*m_sound_effects_level*0.1, 0.0, 1.2, ALLEGRO_PLAYMODE_ONCE, NULL);
            m_selection = 0;
          }
          break;
        case Controls::PLAYER_1_RIGHT:
        case Controls::PLAYER_2_RIGHT:
        case Controls::PLAYER_3_RIGHT:
        case Controls::PLAYER_4_RIGHT:
          if (m_selection == 0)
          {
            if (m_move_sound_up) al_play_sample(m_move_sound_down, 2.5*m_sound_effects_level*0.1, 0.0, 1.2, ALLEGRO_PLAYMODE_ONCE, NULL);
            m_selection = 1;
          }
          break;
        case Controls::PLAYER_1_CONFIRM:
        case Controls::PLAYER_2_CONFIRM:
        case Controls::PLAYER_3_CONFIRM:
        case Controls::PLAYER_4_CONFIRM:
          if (m_move_sound_up) al_play_sample(m_move_sound_down, 2.5*m_sound_effects_level*0.1, 0.0, 1.5, ALLEGRO_PLAYMODE_ONCE, NULL);
          if (m_selection == 0) //No
            paused = false;
          else //Yes
          {
            paused = false;
            *m_quit = true;
            *m_hide_standing = true;
            *m_rounds = m_total_rounds;
          }
          break;
        case Controls::PLAYER_1_CANCEL:
        case Controls::PLAYER_2_CANCEL:
        case Controls::PLAYER_3_CANCEL:
        case Controls::PLAYER_4_CANCEL:
          paused = false;
          break;
        default:
          break;
      }
    }
  }
}


