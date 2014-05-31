#include "high_scores.h"
#include "controls.h"
#include <allegro5/allegro_color.h>
#include <assert.h>

High_Scores::High_Scores(ALLEGRO_EVENT_QUEUE *event, Controls *controls, float font_small_incrementor, float font_medium_incrementor, float font_large_incrementor, ALLEGRO_FONT *font_small, ALLEGRO_FONT *font_medium, ALLEGRO_FONT *font_large, ALLEGRO_SAMPLE *sound_down, ALLEGRO_SAMPLE *sound_up, float sound_effects_level)
{
  assert(event);
  assert(controls);
  m_event = event;
  m_controls = controls;
  m_snake_high_scores = new std::array<std::pair<std::string, int>, 10>();
  m_tron_high_scores = new std::array<std::pair<std::string, int>, 10>();
  for (int i = 0; i < 10; i++)
  {
    m_snake_high_scores->at(i).first = "";
    m_snake_high_scores->at(i).second = 0;
    m_tron_high_scores->at(i).first = "";
    m_tron_high_scores->at(i).second = 0;
  }
  m_font_small_incrementor = font_small_incrementor;
  m_font_medium_incrementor = font_medium_incrementor;
  m_font_large_incrementor = font_large_incrementor;
  m_font_small = font_small;
  m_font_medium = font_medium;
  m_font_large = font_large;
  m_move_sound_down = sound_down;
  m_move_sound_up = sound_up;
  m_sound_effects_level = sound_effects_level;
}

High_Scores::High_Scores(std::array<std::pair<std::string, int>, 10> *snake_high_scores, std::array<std::pair<std::string, int>, 10> *tron_high_scores, ALLEGRO_EVENT_QUEUE *event, Controls *controls, float font_small_incrementor, float font_medium_incrementor, float font_large_incrementor, ALLEGRO_FONT *font_small, ALLEGRO_FONT *font_medium, ALLEGRO_FONT *font_large, ALLEGRO_SAMPLE *sound_down, ALLEGRO_SAMPLE *sound_up, float sound_effects_level)
{
  assert(event);
  assert(controls);
  m_event = event;
  m_controls = controls;
  m_snake_high_scores = snake_high_scores;
  m_tron_high_scores = tron_high_scores;
  m_font_small_incrementor = font_small_incrementor;
  m_font_medium_incrementor = font_medium_incrementor;
  m_font_large_incrementor = font_large_incrementor;
  m_font_small = font_small;
  m_font_medium = font_medium;
  m_font_large = font_large;
  m_move_sound_down = sound_down;
  m_move_sound_up = sound_up;
  m_sound_effects_level = sound_effects_level;
}

High_Scores::~High_Scores()
{
  delete m_snake_high_scores;
  delete m_tron_high_scores;
}

void High_Scores::show_high_scores(bool tron)
{
  while (true);
  {
    al_clear_to_color(al_color_name("black"));
    if (tron) //show tron high scores
    {
      //FILL THIS IN
    }
    else //show snake high scores
    {
      //FILL THIS IN
    }
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
        case Controls::PLAYER_1_RIGHT:
        case Controls::PLAYER_2_RIGHT:
        case Controls::PLAYER_3_RIGHT:
        case Controls::PLAYER_4_RIGHT:
          tron = !tron;
          break;
        case Controls::PLAYER_1_CONFIRM:
        case Controls::PLAYER_2_CONFIRM:
        case Controls::PLAYER_3_CONFIRM:
        case Controls::PLAYER_4_CONFIRM:
        case Controls::PLAYER_1_CANCEL:
        case Controls::PLAYER_2_CANCEL:
        case Controls::PLAYER_3_CANCEL:
        case Controls::PLAYER_4_CANCEL:
          if (m_move_sound_down) al_play_sample(m_move_sound_down, 2.5*m_sound_effects_level*0.1, 0.0, 0.7, ALLEGRO_PLAYMODE_ONCE, NULL);
          return;
        default:
          break;
      }
    }
  }
}

int High_Scores::get_highest_score(bool tron)
{
  if (tron)
  {
    assert(m_tron_high_scores);
    assert(m_tron_high_scores->size() == 10);
    return m_tron_high_scores->front().second;
  }
  else
  {
    assert(m_snake_high_scores);
    assert(m_snake_high_scores->size() == 10);
    return m_snake_high_scores->front().second;
  }
}


