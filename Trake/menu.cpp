#include "menu.h"
#include "controls.h"
#include "rectangle.h"
#include "game.h"
#include "music.h"
#include "save_file_manager.h"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_color.h>
#include <assert.h>
#include <string>
#include <iterator>


Menu::Menu(ALLEGRO_EVENT_QUEUE *event, float screen_width, float screen_height, float snake_width)
{
  assert(event);
  assert(screen_width > 0);
  assert(screen_height > 0);
  m_event = event;
  m_gametype_selection = 0;
  m_win_selection = 0;
  m_rounds = 1;
  m_human_players = 1;
  m_ai_players = 3;
  m_screen_width = screen_width;
  m_screen_height = screen_height;
  m_music_level = 10;
  m_sound_effects_level = 10;
  m_controls = NULL;
  Save_File_Manager save_file_manager = Save_File_Manager();
  save_file_manager.load(m_music_level, m_sound_effects_level, m_controls);
  m_font_small_incrementor = (m_screen_height/24);
  m_font_medium_incrementor = (m_screen_height/(128/7));
  m_font_large_incrementor = (m_screen_height/(32/3));
  m_font_small = al_load_font("./fonts/Sabo-Regular.ttf", (int)m_font_small_incrementor, 0);
  m_font_medium = al_load_font("./fonts/Sabo-Regular.ttf", (int)m_font_medium_incrementor, 0);
  m_font_large = al_load_font("./fonts/Sabo-Regular.ttf", (int)m_font_large_incrementor, 0);
  al_clear_to_color(al_color_name("black"));
  al_draw_text(m_font_large, al_color_name("yellow"), m_screen_width/2, m_screen_height/2, ALLEGRO_ALIGN_CENTER, "Loading...");
  al_flip_display();
  m_music = new Music(m_music_level);
  int i = 0;
  float title = 0.0;
  float x = 0.0;
  do
  {
    x = title;
    ++i;
    title = snake_width * 24 * (float)i;
  } while (title < m_screen_width);
  m_thickness = (float)i-1;
  m_snake_width = snake_width;
  m_title_snake_width = snake_width * m_thickness;
  m_move_sound_up = al_load_sample("./sounds/pop2.wav");
  m_move_sound_down = al_load_sample("./sounds/pop1.wav");
  m_game = NULL;
  m_title = new std::vector<Rectangle*>();
  x = (m_screen_width - x)/2;
  this->create_title(x, m_title_snake_width);
  m_music_fade_thread = NULL;

  //Assign controls
  m_controls = new Controls();
  m_controls->set_control(ALLEGRO_KEY_ENTER, Controls::PLAYER_1_CONFIRM);
  m_controls->set_control(ALLEGRO_KEY_ESCAPE, Controls::PLAYER_1_CANCEL);
  m_controls->set_control(ALLEGRO_KEY_LEFT, Controls::PLAYER_1_LEFT);
  m_controls->set_control(ALLEGRO_KEY_RIGHT, Controls::PLAYER_1_RIGHT);
  m_controls->set_control(ALLEGRO_KEY_DOWN, Controls::PLAYER_1_DOWN);
  m_controls->set_control(ALLEGRO_KEY_UP, Controls::PLAYER_1_UP);
  m_controls->set_control(ALLEGRO_KEY_A, Controls::PLAYER_2_LEFT);
  m_controls->set_control(ALLEGRO_KEY_D, Controls::PLAYER_2_RIGHT);
  m_controls->set_control(ALLEGRO_KEY_S, Controls::PLAYER_2_DOWN);
  m_controls->set_control(ALLEGRO_KEY_W, Controls::PLAYER_2_UP);
  m_controls->set_control(ALLEGRO_KEY_J, Controls::PLAYER_3_LEFT);
  m_controls->set_control(ALLEGRO_KEY_L, Controls::PLAYER_3_RIGHT);
  m_controls->set_control(ALLEGRO_KEY_K, Controls::PLAYER_3_DOWN);
  m_controls->set_control(ALLEGRO_KEY_I, Controls::PLAYER_3_UP);
  m_controls->set_control(ALLEGRO_KEY_PAD_4, Controls::PLAYER_4_LEFT);
  m_controls->set_control(ALLEGRO_KEY_PAD_6, Controls::PLAYER_4_RIGHT);
  m_controls->set_control(ALLEGRO_KEY_PAD_5, Controls::PLAYER_4_DOWN);
  m_controls->set_control(ALLEGRO_KEY_PAD_8, Controls::PLAYER_4_UP);
}

Menu::~Menu()
{
  for (std::vector<Rectangle*>::iterator iter = m_title->begin(); iter != m_title->end(); ++iter)
  {
    delete (*iter);
  }
  m_title->clear();
  delete m_music;
  delete m_controls;
  al_destroy_sample(m_move_sound_down);
  al_destroy_sample(m_move_sound_up);
  al_destroy_font(m_font_small);
  al_destroy_font(m_font_medium);
  al_destroy_font(m_font_large);
}

void Menu::show()
{
  m_music->play();
  this->show_title();
}

void Menu::show_title()
{
  int selection = 0;
  std::string items[3] = { "Start", "Options", "Exit" };
  while (true)
  {
    al_clear_to_color(al_color_name("black"));
    this->draw_title_logo();

    //Draw Selections
    for (int i = 0; i < 3; i++)
    {
      ALLEGRO_COLOR color;
      if (selection == i)
      {
        color = al_color_name("lawngreen");
      }
      else
      {
        color = al_color_name("lightgray");
      }
      al_draw_text(m_font_large, color, m_screen_width/2, ((m_screen_height/16)*9)+(i*m_font_large_incrementor), ALLEGRO_ALIGN_CENTER, items[i].c_str());
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
        case Controls::PLAYER_1_DOWN:
        case Controls::PLAYER_2_DOWN:
        case Controls::PLAYER_3_DOWN:
        case Controls::PLAYER_4_DOWN:
          if (m_move_sound_down) al_play_sample(m_move_sound_down, 2.5*m_sound_effects_level*0.1, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
          selection = (selection + 1) % 3;
          break;
        case Controls::PLAYER_1_UP:
        case Controls::PLAYER_2_UP:
        case Controls::PLAYER_3_UP:
        case Controls::PLAYER_4_UP:
          if (m_move_sound_up) al_play_sample(m_move_sound_up, 2.5*m_sound_effects_level*0.1, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
          selection -= 1;
          if (selection < 0) selection = 2;
          break;
        case Controls::PLAYER_1_CONFIRM:
        case Controls::PLAYER_2_CONFIRM:
        case Controls::PLAYER_3_CONFIRM:
        case Controls::PLAYER_4_CONFIRM:
          switch (selection)
          {
            case 0: //Start
              if (m_move_sound_up) al_play_sample(m_move_sound_down, 2.5*m_sound_effects_level*0.1, 0.0, 1.5, ALLEGRO_PLAYMODE_ONCE, NULL);
              this->show_game_setup();
              break;
            case 1: //Options
              if (m_move_sound_up) al_play_sample(m_move_sound_down, 2.5*m_sound_effects_level*0.1, 0.0, 1.5, ALLEGRO_PLAYMODE_ONCE, NULL);
              this->show_options();
              break;
            case 2: //Exit
              if (m_move_sound_down)
              {
                al_play_sample(m_move_sound_down, 2.5*m_sound_effects_level*0.1, 0.0, 0.7, ALLEGRO_PLAYMODE_ONCE, NULL);
                Save_File_Manager save_file_manager = Save_File_Manager();
                save_file_manager.save(m_music_level, m_sound_effects_level, m_controls);
                m_music->fade_to_stop();
              }
              return;
            default:
              assert(false);
          }
          break;
        case Controls::PLAYER_1_CANCEL:
        case Controls::PLAYER_2_CANCEL:
        case Controls::PLAYER_3_CANCEL:
        case Controls::PLAYER_4_CANCEL:
          if (m_move_sound_down)
          {
            al_play_sample(m_move_sound_down, 2.5*m_sound_effects_level*0.1, 0.0, 0.7, ALLEGRO_PLAYMODE_ONCE, NULL);
            Save_File_Manager save_file_manager = Save_File_Manager();
            save_file_manager.save(m_music_level, m_sound_effects_level, m_controls);
            m_music->fade_to_stop();
          }
          return;
        default:
          break;
      }
    }
  }
}

void Menu::show_game_setup()
{
  float menu_size = m_font_medium_incrementor * 16;
  int selection = 0;
  std::string items[7] = { "Play!", "Game Type:", "Win Condition:", "Rounds:", "Human Players:", "AI Players:", "Back" };
    std::string gametype_items[2] = { "<   Snake   >", "<   Tron   >" };
    std::string win_condition_items[2] = { "Score", "Survival" };
  while (true)
  {
    al_clear_to_color(al_color_name("black"));
    std::string text = "";
    
    //Draw Selections
    float y = (m_screen_height/2)-(menu_size/2);
    for (int i = 0; i < 7; i++)
    {
      ALLEGRO_COLOR color;
      if (selection == i)
      {
        color = al_color_name("lawngreen");
      }
      else
      {
        color = al_color_name("lightgray");
      }
      al_draw_text(m_font_medium, color, m_screen_width/2, y, ALLEGRO_ALIGN_CENTER, items[i].c_str());
      switch (i)
      {
        case 0: //Play!
          y += m_font_medium_incrementor*2;
          break;
        case 1: //GameType
          y += m_font_medium_incrementor;
          al_draw_text(m_font_medium, color, m_screen_width/2, y, ALLEGRO_ALIGN_CENTER, gametype_items[m_gametype_selection].c_str());
          y += m_font_medium_incrementor;
          y += m_font_medium_incrementor/2;
          break;
        case 2: //Win Condition
          y += m_font_medium_incrementor;
          if (m_ai_players + m_human_players == 1)
                text = "High Score";
          else
              text = "<   " + win_condition_items[m_win_selection] + "   >";
          al_draw_text(m_font_medium, color, m_screen_width/2, y, ALLEGRO_ALIGN_CENTER, text.c_str());
          y += m_font_medium_incrementor;
          y += m_font_medium_incrementor/2;
          break;
        case 3: //Rounds
          y += m_font_medium_incrementor;
          if (m_ai_players + m_human_players > 1)
            al_draw_text(m_font_medium, color, m_screen_width/2, y, ALLEGRO_ALIGN_CENTER, ("<   " + std::to_string(m_rounds) + "   >").c_str());
          else
            al_draw_text(m_font_medium, color, m_screen_width/2, y, ALLEGRO_ALIGN_CENTER, std::to_string(m_rounds).c_str());
          y += m_font_medium_incrementor;
          y += m_font_medium_incrementor/2;
          break;
        case 4: //Human Players
          y += m_font_medium_incrementor;
          al_draw_text(m_font_medium, color, m_screen_width/2, y, ALLEGRO_ALIGN_CENTER, ("<   " + std::to_string(m_human_players) + "   >").c_str());
          y += m_font_medium_incrementor;
          y += m_font_medium_incrementor/2;
          break;
        case 5: //AI Players
          y += m_font_medium_incrementor;
          al_draw_text(m_font_medium, color, m_screen_width/2, y, ALLEGRO_ALIGN_CENTER, ("<   " + std::to_string(m_ai_players) + "   >").c_str());
          y += m_font_medium_incrementor*2; //extra space before back button
          break;
      }
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
          switch (selection)
          {
            case 1: //GameType
              if (m_move_sound_down) al_play_sample(m_move_sound_down, 2.5*m_sound_effects_level*0.1, 0.0, 1.2, ALLEGRO_PLAYMODE_ONCE, NULL);
              --m_gametype_selection;
              if (m_gametype_selection < 0)
              m_gametype_selection = 1;
              break;
            case 2: //Win Condition
              if (m_ai_players + m_human_players > 1)
              {
                if (m_move_sound_down) al_play_sample(m_move_sound_down, 2.5*m_sound_effects_level*0.1, 0.0, 1.2, ALLEGRO_PLAYMODE_ONCE, NULL);
                --m_win_selection;
                if (m_win_selection < 0)
                  m_win_selection = 1;
              }
              break;
            case 3: //Rounds
              if (m_ai_players + m_human_players > 1)
              {
                if (m_move_sound_down) al_play_sample(m_move_sound_down, 2.5*m_sound_effects_level*0.1, 0.0, 1.2, ALLEGRO_PLAYMODE_ONCE, NULL);
                --m_rounds;
                if (m_rounds < 1)
                  m_rounds = 1;
              }
              break;
            case 4: //Human Players
              if (m_move_sound_down) al_play_sample(m_move_sound_down, 2.5*m_sound_effects_level*0.1, 0.0, 1.2, ALLEGRO_PLAYMODE_ONCE, NULL);
              --m_human_players;
              if (m_human_players < 0)
                m_human_players = 0;
              if (m_ai_players == 0 && m_human_players == 0)
                m_human_players = 1;
              while (m_ai_players + m_human_players > 4)
                --m_ai_players;
              if (m_ai_players + m_human_players == 1)
              {
                m_win_selection = 0;
                m_rounds = 1;
              }
              break;
            case 5: //AI Players
              if (m_move_sound_down) al_play_sample(m_move_sound_down, 2.5*m_sound_effects_level*0.1, 0.0, 1.2, ALLEGRO_PLAYMODE_ONCE, NULL);
              --m_ai_players;
              if (m_ai_players < 0)
                m_ai_players = 0;
              if (m_ai_players == 0 && m_human_players == 0)
                m_human_players = 1;
              while (m_ai_players + m_human_players > 4)
                --m_human_players;
              if (m_ai_players + m_human_players == 1)
              {
                m_win_selection = 0;
                m_rounds = 1;
              }
              break;
          }
          break;
        case Controls::PLAYER_1_RIGHT:
        case Controls::PLAYER_2_RIGHT:
        case Controls::PLAYER_3_RIGHT:
        case Controls::PLAYER_4_RIGHT:
          switch (selection)
          {
            case 1: //GameType
              if (m_move_sound_up) al_play_sample(m_move_sound_down, 2.5*m_sound_effects_level*0.1, 0.0, 1.2, ALLEGRO_PLAYMODE_ONCE, NULL);
              m_gametype_selection = (m_gametype_selection + 1) % 2;
              break;
            case 2: //Win Condition
              if (m_ai_players + m_human_players > 1)
              {
                if (m_move_sound_up) al_play_sample(m_move_sound_down, 2.5*m_sound_effects_level*0.1, 0.0, 1.2, ALLEGRO_PLAYMODE_ONCE, NULL);
                m_win_selection = (m_win_selection + 1) % 2;
              }
              break;
            case 3: //Rounds
              if (m_ai_players + m_human_players > 1)
              {
                if (m_move_sound_up) al_play_sample(m_move_sound_down, 2.5*m_sound_effects_level*0.1, 0.0, 1.2, ALLEGRO_PLAYMODE_ONCE, NULL);
                ++m_rounds;
                if (m_rounds > 10)
                  m_rounds = 10;
              }
              break;
            case 4: //Human Players
              if (m_move_sound_up) al_play_sample(m_move_sound_down, 2.5*m_sound_effects_level*0.1, 0.0, 1.2, ALLEGRO_PLAYMODE_ONCE, NULL);
              ++m_human_players;
              if (m_human_players > 4)
                m_human_players = 4;
              if (m_ai_players == 0 && m_human_players == 0)
                m_human_players = 1;
              while (m_ai_players + m_human_players > 4)
                --m_ai_players;
              if (m_ai_players + m_human_players == 1)
              {
                m_win_selection = 0;
                m_rounds = 1;
              }
              break;
            case 5: //AI Players
              if (m_move_sound_up) al_play_sample(m_move_sound_down, 2.5*m_sound_effects_level*0.1, 0.0, 1.2, ALLEGRO_PLAYMODE_ONCE, NULL);
              ++m_ai_players;
              if (m_ai_players > 4)
                m_ai_players = 4;
              if (m_ai_players == 0 && m_human_players == 0)
                m_human_players = 1;
              while (m_ai_players + m_human_players > 4)
                --m_human_players;
              if (m_ai_players + m_human_players == 1)
              {
                m_win_selection = 0;
                m_rounds = 1;
              }
              break;
          }
          break;
        case Controls::PLAYER_1_DOWN:
        case Controls::PLAYER_2_DOWN:
        case Controls::PLAYER_3_DOWN:
        case Controls::PLAYER_4_DOWN:
          if (m_move_sound_down) al_play_sample(m_move_sound_down, 2.5*m_sound_effects_level*0.1, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
          selection = (selection + 1) % 7;
          break;
        case Controls::PLAYER_1_UP:
        case Controls::PLAYER_2_UP:
        case Controls::PLAYER_3_UP:
        case Controls::PLAYER_4_UP:
          if (m_move_sound_up) al_play_sample(m_move_sound_up, 2.5*m_sound_effects_level*0.1, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
          selection -= 1;
          if (selection < 0) selection = 6;
          break;
        case Controls::PLAYER_1_CONFIRM:
        case Controls::PLAYER_2_CONFIRM:
        case Controls::PLAYER_3_CONFIRM:
        case Controls::PLAYER_4_CONFIRM:
          switch (selection)
          {
            case 0: //Play!
              if (m_move_sound_up) al_play_sample(m_move_sound_down, 2.5*m_sound_effects_level*0.1, 0.0, 1.5, ALLEGRO_PLAYMODE_ONCE, NULL);
              m_music_fade_thread = al_create_thread(Music_Thread::Music_Fade_Thread, m_music);
              al_start_thread(m_music_fade_thread);
              m_game = new Game(m_event, m_controls, m_music_fade_thread, m_screen_width, m_screen_height, m_snake_width, m_font_small_incrementor, m_font_medium_incrementor, m_font_large_incrementor, m_font_small, m_font_medium, m_font_large, m_music_level, m_sound_effects_level, m_human_players, m_ai_players, m_gametype_selection, m_win_selection, m_rounds, m_move_sound_down, m_move_sound_up);
              m_game->run();
              al_destroy_thread(m_music_fade_thread);
              m_music_fade_thread = NULL;
              delete m_game;
              m_game = NULL;
              m_music->set_volume(m_music_level);
              m_music->play();
              return;
            case 6: //Back
              if (m_move_sound_down) al_play_sample(m_move_sound_down, 2.5*m_sound_effects_level*0.1, 0.0, 0.7, ALLEGRO_PLAYMODE_ONCE, NULL);
              return;
          }
          break;
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

void Menu::show_options()
{
  float menu_size = m_font_medium_incrementor*8;
  int selection = 0;
  std::string items[5] = { "Music:", "Sound Effects:", "Controls", "Credits", "Back" };
  while (true)
  {
    al_clear_to_color(al_color_name("black"));
    float y = m_screen_height/50;
    al_draw_text(m_font_medium, al_color_name("lightgray"), m_screen_width/2, y, ALLEGRO_ALIGN_CENTER, "Options");
    y = (m_screen_height/2)-(menu_size/2);

    //Draw Selections
    for (int i = 0; i < 5; i++)
    {
      ALLEGRO_COLOR color;
      if (selection == i)
      {
        color = al_color_name("lawngreen");
      }
      else
      {
        color = al_color_name("lightgray");
      }
      al_draw_text(m_font_medium, color, m_screen_width/2, y, ALLEGRO_ALIGN_CENTER, items[i].c_str());
      switch (i)
      {
        case 0:
          y += m_font_medium_incrementor;
          al_draw_text(m_font_medium, color, m_screen_width/2, y, ALLEGRO_ALIGN_CENTER, ("<   " + std::to_string(m_music_level) + "   >").c_str());
          y += m_font_medium_incrementor;
          y += m_font_medium_incrementor/2;
          break;
        case 1:
          y += m_font_medium_incrementor;
          al_draw_text(m_font_medium, color, m_screen_width/2, y, ALLEGRO_ALIGN_CENTER, ("<   " + std::to_string(m_sound_effects_level) + "   >").c_str());
          y += m_font_medium_incrementor;
          y += m_font_medium_incrementor/2;
          break;
        case 3:
          y += m_font_medium_incrementor*2;
          break;
        default:
          y += m_font_medium_incrementor;
          break;
      }
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
          switch (selection)
          {
            case 0: //Music level
              if (m_move_sound_down) al_play_sample(m_move_sound_down, 2.5*m_sound_effects_level*0.1, 0.0, 1.2, ALLEGRO_PLAYMODE_ONCE, NULL);
              --m_music_level;
              if (m_music_level < 0)
              {
                m_music_level = 0;
                m_music->stop();
              }
              m_music->set_volume(m_music_level);
              break;
            case 1: //Sound effects level
              --m_sound_effects_level;
              if (m_sound_effects_level < 0)
                m_sound_effects_level = 0;
              if (m_move_sound_down) al_play_sample(m_move_sound_down, 2.5*m_sound_effects_level*0.1, 0.0, 1.2, ALLEGRO_PLAYMODE_ONCE, NULL);
              break;
          }
          break;
        case Controls::PLAYER_1_RIGHT:
        case Controls::PLAYER_2_RIGHT:
        case Controls::PLAYER_3_RIGHT:
        case Controls::PLAYER_4_RIGHT:
          switch (selection)
          {
            case 0: //Music level
              if (m_move_sound_up) al_play_sample(m_move_sound_down, 2.5*m_sound_effects_level*0.1, 0.0, 1.2, ALLEGRO_PLAYMODE_ONCE, NULL);
              ++m_music_level;
              if (m_music_level > 10)
                m_music_level = 10;
              m_music->set_volume(m_music_level);
              if (!m_music->is_playing())
                m_music->play();
              break;
            case 1: //Sound effects level
              if (m_move_sound_up) al_play_sample(m_move_sound_down, 2.5*m_sound_effects_level*0.1, 0.0, 1.2, ALLEGRO_PLAYMODE_ONCE, NULL);
              ++m_sound_effects_level;
              if (m_sound_effects_level > 10)
                m_sound_effects_level = 10;
              break;
          }
          break;
        case Controls::PLAYER_1_DOWN:
        case Controls::PLAYER_2_DOWN:
        case Controls::PLAYER_3_DOWN:
        case Controls::PLAYER_4_DOWN:
          if (m_move_sound_down) al_play_sample(m_move_sound_down, 2.5*m_sound_effects_level*0.1, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
          selection = (selection + 1) % 5;
          break;
        case Controls::PLAYER_1_UP:
        case Controls::PLAYER_2_UP:
        case Controls::PLAYER_3_UP:
        case Controls::PLAYER_4_UP:
          if (m_move_sound_up) al_play_sample(m_move_sound_up, 2.5*m_sound_effects_level*0.1, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
          --selection;
          if (selection < 0)
            selection = 4;
          break;
        case Controls::PLAYER_1_CONFIRM:
        case Controls::PLAYER_2_CONFIRM:
        case Controls::PLAYER_3_CONFIRM:
        case Controls::PLAYER_4_CONFIRM:
          switch (selection)
          {
            case 2: //Controls
              if (m_move_sound_up) al_play_sample(m_move_sound_down, 2.5*m_sound_effects_level*0.1, 0.0, 1.5, ALLEGRO_PLAYMODE_ONCE, NULL);
              this->show_controls();
              break;
            case 3: //Credits
              if (m_move_sound_up) al_play_sample(m_move_sound_down, 2.5*m_sound_effects_level*0.1, 0.0, 1.5, ALLEGRO_PLAYMODE_ONCE, NULL);
              this->show_credits();
              break;
            case 4: //Back
              if (m_move_sound_down) al_play_sample(m_move_sound_down, 2.5*m_sound_effects_level*0.1, 0.0, 0.7, ALLEGRO_PLAYMODE_ONCE, NULL);
              return;
          }
          break;
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

void Menu::show_controls()
{
  float menu_size = m_font_medium_incrementor*10;
  int selection = 0;
  std::string items[5] = { "Player 1", "Player 2", "Player 3", "Player 4", "Back" };
  while (true)
  {
    al_clear_to_color(al_color_name("black"));
    float y = m_screen_height/50;
    al_draw_text(m_font_medium, al_color_name("lightgray"), m_screen_width/2, y, ALLEGRO_ALIGN_CENTER, "Controls");
    y += (m_screen_height/2)-(menu_size/2);

    //Draw Selections
    for (int i = 0; i < 5; i++)
    {
      ALLEGRO_COLOR color;
      if (selection == i)
      {
        color = al_color_name("lawngreen");
      }
      else
      {
        color = al_color_name("lightgray");
      }
      al_draw_text(m_font_medium, color, m_screen_width/2, y, ALLEGRO_ALIGN_CENTER, items[i].c_str());
      switch (i)
      {
        case 0:
          color = al_color_name("lawngreen");
          break;
        case 1:
          color = al_color_name("blue");
          break;
        case 2:
          color = al_color_name("red");
          break;
        case 3:
          color = al_color_name("yellow");
          break;
      }
      if (i != 4)
      {
        al_draw_filled_rectangle(((m_screen_width/8)*3)-(m_font_medium_incrementor/2), y+5, ((m_screen_width/8)*3)+(m_font_medium_incrementor/2), y+5 + m_font_medium_incrementor, color);
        al_draw_filled_rectangle(((m_screen_width/8)*5)-(m_font_medium_incrementor/2), y+5, ((m_screen_width/8)*5)+(m_font_medium_incrementor/2), y+5 + m_font_medium_incrementor, color);
      }
      y += m_font_medium_incrementor*2;
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
        case Controls::PLAYER_1_DOWN:
        case Controls::PLAYER_2_DOWN:
        case Controls::PLAYER_3_DOWN:
        case Controls::PLAYER_4_DOWN:
          if (m_move_sound_down) al_play_sample(m_move_sound_down, 2.5*m_sound_effects_level*0.1, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
          selection = (selection + 1) % 5;
          break;
        case Controls::PLAYER_1_UP:
        case Controls::PLAYER_2_UP:
        case Controls::PLAYER_3_UP:
        case Controls::PLAYER_4_UP:
          if (m_move_sound_up) al_play_sample(m_move_sound_up, 2.5*m_sound_effects_level*0.1, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
          selection -= 1;
          if (selection < 0) selection = 4;
          break;
        case Controls::PLAYER_1_CONFIRM:
        case Controls::PLAYER_2_CONFIRM:
        case Controls::PLAYER_3_CONFIRM:
        case Controls::PLAYER_4_CONFIRM:
          if (selection == 4)
          {
            if (m_move_sound_down) al_play_sample(m_move_sound_down, 2.5*m_sound_effects_level*0.1, 0.0, 0.7, ALLEGRO_PLAYMODE_ONCE, NULL);
            return;
          }
          else
          {
            if (m_move_sound_up) al_play_sample(m_move_sound_down, 2.5*m_sound_effects_level*0.1, 0.0, 1.5, ALLEGRO_PLAYMODE_ONCE, NULL);
            this->show_control_setup(selection+1);
          }
          break;
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

void Menu::show_control_setup(int player_num)
{
  float menu_size = m_font_medium_incrementor * 12;
  int selection = 0;
  bool setting_key = false;
  int setting_key_selection = 0;
  std::string items[7] = { "Up:", "Down:", "Left:", "Right:", "Confirm:", "Cancel:", "Back" };
  while (true)
  {
    al_clear_to_color(al_color_name("black"));
    float y = m_screen_height/50;
    std::string text = "Player " + std::to_string(player_num) + " Controls";
    ALLEGRO_COLOR color;
    switch (player_num)
    {
      case 1:
        color = al_color_name("lawngreen");
        break;
      case 2:
        color = al_color_name("blue");
        break;
      case 3:
        color = al_color_name("red");
        break;
      case 4:
        color = al_color_name("yellow");
        break;
      default:
        assert(false);
    }
    al_draw_text(m_font_medium, al_color_name("lightgray"), m_screen_width/2, y, ALLEGRO_ALIGN_CENTER, text.c_str());
    al_draw_filled_rectangle((m_screen_width/4)-(m_font_medium_incrementor/2), y+5, (m_screen_width/4)+(m_font_medium_incrementor/2), y+5 + m_font_medium_incrementor, color);
    al_draw_filled_rectangle(((m_screen_width/4)*3)-(m_font_medium_incrementor/2), y+5, ((m_screen_width/4)*3)+(m_font_medium_incrementor/2), y+5 + m_font_medium_incrementor, color);
    y = (m_screen_height/2)-(menu_size/2);

    //Draw Selections
    for (int i = 0; i < 7; i++)
    {
      if (selection == i)
      {
        color = al_color_name("lawngreen");
      }
      else
      {
        color = al_color_name("lightgray");
      }
      if (i == 6)
      {
        al_draw_text(m_font_medium, color, m_screen_width/2, y, ALLEGRO_ALIGN_CENTER, items[i].c_str());
        continue;
      }
      al_draw_text(m_font_medium, color, (m_screen_width/4)-(m_font_medium_incrementor/2), y, ALLEGRO_ALIGN_LEFT, items[i].c_str());
      if (setting_key && setting_key_selection == i)
      {
        al_draw_text(m_font_medium, color, ((m_screen_width/4)*3)+(m_font_medium_incrementor/2), y, ALLEGRO_ALIGN_RIGHT, "Waiting...");
      y += m_font_medium_incrementor*2;
        continue;
      }
      int key = -1;
      switch (i)
      {
        case 0: //Up
          switch (player_num)
          {
            case 1:
              key = m_controls->get_key(Controls::PLAYER_1_UP);
              break;
            case 2:
              key = m_controls->get_key(Controls::PLAYER_2_UP);
              break;
            case 3:
              key = m_controls->get_key(Controls::PLAYER_3_UP);
              break;
            case 4:
              key = m_controls->get_key(Controls::PLAYER_4_UP);
              break;
            default:
              assert(false);
          }
          break;
        case 1: //Down
          switch (player_num)
          {
            case 1:
              key = m_controls->get_key(Controls::PLAYER_1_DOWN);
              break;
            case 2:
              key = m_controls->get_key(Controls::PLAYER_2_DOWN);
              break;
            case 3:
              key = m_controls->get_key(Controls::PLAYER_3_DOWN);
              break;
            case 4:
              key = m_controls->get_key(Controls::PLAYER_4_DOWN);
              break;
            default:
              assert(false);
          }
          break;
        case 2: //Left
          switch (player_num)
          {
            case 1:
              key = m_controls->get_key(Controls::PLAYER_1_LEFT);
              break;
            case 2:
              key = m_controls->get_key(Controls::PLAYER_2_LEFT);
              break;
            case 3:
              key = m_controls->get_key(Controls::PLAYER_3_LEFT);
              break;
            case 4:
              key = m_controls->get_key(Controls::PLAYER_4_LEFT);
              break;
            default:
              assert(false);
          }
          break;
        case 3: //Right
          switch (player_num)
          {
            case 1:
              key = m_controls->get_key(Controls::PLAYER_1_RIGHT);
              break;
            case 2:
              key = m_controls->get_key(Controls::PLAYER_2_RIGHT);
              break;
            case 3:
              key = m_controls->get_key(Controls::PLAYER_3_RIGHT);
              break;
            case 4:
              key = m_controls->get_key(Controls::PLAYER_4_RIGHT);
              break;
            default:
              assert(false);
          }
          break;
        case 4: //Confirm
          switch (player_num)
          {
            case 1:
              key = m_controls->get_key(Controls::PLAYER_1_CONFIRM);
              break;
            case 2:
              key = m_controls->get_key(Controls::PLAYER_2_CONFIRM);
              break;
            case 3:
              key = m_controls->get_key(Controls::PLAYER_3_CONFIRM);
              break;
            case 4:
              key = m_controls->get_key(Controls::PLAYER_4_CONFIRM);
              break;
            default:
              assert(false);
          }
          break;
        case 5: //Cancel
          switch (player_num)
          {
            case 1:
              key = m_controls->get_key(Controls::PLAYER_1_CANCEL);
              break;
            case 2:
              key = m_controls->get_key(Controls::PLAYER_2_CANCEL);
              break;
            case 3:
              key = m_controls->get_key(Controls::PLAYER_3_CANCEL);
              break;
            case 4:
              key = m_controls->get_key(Controls::PLAYER_4_CANCEL);
              break;
            default:
              assert(false);
          }
          break;
        default:
          break;
      }
      if (key == -1)
        text = "None";
      else
        text = al_keycode_to_name(key);
      al_draw_text(m_font_medium, color, ((m_screen_width/4)*3)+(m_font_medium_incrementor/2), y, ALLEGRO_ALIGN_RIGHT, text.c_str());
      y += m_font_medium_incrementor*2;
    }
    al_flip_display();
    if (setting_key)
    {
      this->assign_key(player_num, setting_key_selection);
      setting_key = false;
      continue;
    }

    ALLEGRO_EVENT e;
    al_wait_for_event(m_event, &e);
    if (e.type == ALLEGRO_EVENT_KEY_DOWN)
    {
      switch(m_controls->get_control(e.keyboard.keycode))
      {
        case Controls::NONE:
          break;
        case Controls::PLAYER_1_DOWN:
        case Controls::PLAYER_2_DOWN:
        case Controls::PLAYER_3_DOWN:
        case Controls::PLAYER_4_DOWN:
          if (m_move_sound_down) al_play_sample(m_move_sound_down, 2.5*m_sound_effects_level*0.1, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
          selection = (selection + 1) % 7;
          break;
        case Controls::PLAYER_1_UP:
        case Controls::PLAYER_2_UP:
        case Controls::PLAYER_3_UP:
        case Controls::PLAYER_4_UP:
          if (m_move_sound_up) al_play_sample(m_move_sound_up, 2.5*m_sound_effects_level*0.1, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
          selection -= 1;
          if (selection < 0) selection = 6;
          break;
        case Controls::PLAYER_1_CONFIRM:
        case Controls::PLAYER_2_CONFIRM:
        case Controls::PLAYER_3_CONFIRM:
        case Controls::PLAYER_4_CONFIRM:
          if (selection == 6)
          {
            if (m_move_sound_down) al_play_sample(m_move_sound_down, 2.5*m_sound_effects_level*0.1, 0.0, 0.7, ALLEGRO_PLAYMODE_ONCE, NULL);
            return;
          }
          else
          {
            if (m_move_sound_up) al_play_sample(m_move_sound_down, 2.5*m_sound_effects_level*0.1, 0.0, 1.5, ALLEGRO_PLAYMODE_ONCE, NULL);
            setting_key = true;
            setting_key_selection = selection;
          }
          break;
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

void Menu::show_credits()
{
  al_clear_to_color(al_color_name("black"));
  al_draw_text(m_font_large, al_color_name("lawngreen"), m_screen_width/2, (m_screen_height/8)*5, ALLEGRO_ALIGN_CENTER, "Created by Coolcord");
  al_draw_text(m_font_large, al_color_name("blue"), m_screen_width/2, (m_screen_height/4)*3, ALLEGRO_ALIGN_CENTER, "Music by Daft Punk");
  this->draw_title_logo();
  al_flip_display();

  while (true)
  {
    ALLEGRO_EVENT e;
    al_wait_for_event(m_event, &e);
    if (e.type == ALLEGRO_EVENT_KEY_DOWN)
    {
      switch(m_controls->get_control(e.keyboard.keycode))
      {
        case Controls::NONE:
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

void Menu::assign_key(int player_num, int selection)
{
  al_flush_event_queue(m_event);
  bool assigned = false;
  for (int i = 0; i < 5; i++)
  {
    if (assigned)
      break;
    ALLEGRO_EVENT e;
    al_wait_for_event_timed(m_event, &e, 1);
    int key = 0;
    if (e.type == ALLEGRO_EVENT_KEY_DOWN)
    {
      key = e.keyboard.keycode;
    }
    else
      continue; //invalid event

    //Assign the new key
    switch (player_num)
    {
      case 1:
        switch (selection)
        {
          case 0: //Up
            m_controls->set_control(key, Controls::PLAYER_1_UP);
            break;
          case 1: //Down
            m_controls->set_control(key, Controls::PLAYER_1_DOWN);
            break;
          case 2: //Left
            m_controls->set_control(key, Controls::PLAYER_1_LEFT);
            break;
          case 3: //Right
            m_controls->set_control(key, Controls::PLAYER_1_RIGHT);
            break;
          case 4: //Confirm
            m_controls->set_control(key, Controls::PLAYER_1_CONFIRM);
            break;
          case 5: //Cancel
            m_controls->set_control(key, Controls::PLAYER_1_CANCEL);
            break;
          default:
            assert(false);
        }
        break;
      case 2:
        switch (selection)
        {
          case 0: //Up
            m_controls->set_control(key, Controls::PLAYER_2_UP);
            break;
          case 1: //Down
            m_controls->set_control(key, Controls::PLAYER_2_DOWN);
            break;
          case 2: //Left
            m_controls->set_control(key, Controls::PLAYER_2_LEFT);
            break;
          case 3: //Right
            m_controls->set_control(key, Controls::PLAYER_2_RIGHT);
            break;
          case 4: //Confirm
            m_controls->set_control(key, Controls::PLAYER_2_CONFIRM);
            break;
          case 5: //Cancel
            m_controls->set_control(key, Controls::PLAYER_2_CANCEL);
            break;
          default:
            assert(false);
        }
        break;
      case 3:
        switch (selection)
        {
          case 0: //Up
            m_controls->set_control(key, Controls::PLAYER_3_UP);
            break;
          case 1: //Down
            m_controls->set_control(key, Controls::PLAYER_3_DOWN);
            break;
          case 2: //Left
            m_controls->set_control(key, Controls::PLAYER_3_LEFT);
            break;
          case 3: //Right
            m_controls->set_control(key, Controls::PLAYER_3_RIGHT);
            break;
          case 4: //Confirm
            m_controls->set_control(key, Controls::PLAYER_3_CONFIRM);
            break;
          case 5: //Cancel
            m_controls->set_control(key, Controls::PLAYER_3_CANCEL);
            break;
          default:
            assert(false);
        }
        break;
      case 4:
        switch (selection)
        {
          case 0: //Up
            m_controls->set_control(key, Controls::PLAYER_4_UP);
            break;
          case 1: //Down
            m_controls->set_control(key, Controls::PLAYER_4_DOWN);
            break;
          case 2: //Left
            m_controls->set_control(key, Controls::PLAYER_4_LEFT);
            break;
          case 3: //Right
            m_controls->set_control(key, Controls::PLAYER_4_RIGHT);
            break;
          case 4: //Confirm
            m_controls->set_control(key, Controls::PLAYER_4_CONFIRM);
            break;
          case 5: //Cancel
            m_controls->set_control(key, Controls::PLAYER_4_CANCEL);
            break;
          default:
            assert(false);
        }
        break;
      default:
        assert(false);
    }
    assigned = true;
  }
  al_flush_event_queue(m_event);
}

void Menu::draw_title_logo()
{
  //Title logo's size is m_title_snake_width * 24
  for (std::vector<Rectangle*>::iterator iter = m_title->begin(); iter != m_title->end(); ++iter)
  {
    if (*iter) (*iter)->draw();
  }
}

void Menu::create_t(float x, float y)
{
  ALLEGRO_COLOR color = al_color_name("lawngreen");
  float vertical_x = 0.0;
  //Start at the top
  for (int i = 0; i < 5; i++)
  {
    m_title->push_back(new Rectangle(x+(i*m_title_snake_width), y, m_title_snake_width, m_title_snake_width, false, color, m_thickness));
    if (i == 2) vertical_x = x+(i*m_title_snake_width);
  }
  //Finish with a vertical line
  for (int i = 1; i < 5; i++)
  {
    m_title->push_back(new Rectangle(vertical_x, y+(i*m_title_snake_width), m_title_snake_width, m_title_snake_width, false, color, m_thickness));
  }
}

void Menu::create_r(float x, float y)
{
  ALLEGRO_COLOR color = al_color_name("blue");
  //Start with a vertical line
  for (int i = 0; i < 5; i++)
  {
    m_title->push_back(new Rectangle(x, y+(i*m_title_snake_width), m_title_snake_width, m_title_snake_width, false, color, m_thickness));
  }
  //Draw the horizontal lines
  for (int i = 1; i < 3; i++)
  {
    m_title->push_back(new Rectangle(x+(i*m_title_snake_width), y, m_title_snake_width, m_title_snake_width, false, color, m_thickness));
    if (i != 3) m_title->push_back(new Rectangle(x+(i*m_title_snake_width), y+(2*m_title_snake_width), m_title_snake_width, m_title_snake_width, false, color, m_thickness));
  }
  m_title->push_back(new Rectangle(x+(3*m_title_snake_width), y+(1*m_title_snake_width), m_title_snake_width, m_title_snake_width, false, color, m_thickness));
  m_title->push_back(new Rectangle(x+(3*m_title_snake_width), y+(3*m_title_snake_width), m_title_snake_width, m_title_snake_width, false, color, m_thickness));
  m_title->push_back(new Rectangle(x+(3*m_title_snake_width), y+(4*m_title_snake_width), m_title_snake_width, m_title_snake_width, false, color, m_thickness));
}

void Menu::create_a(float x, float y)
{
  ALLEGRO_COLOR color = al_color_name("white");
  //Draw Vertical Lines
  for (int i = 1; i < 5; i++)
  {
    m_title->push_back(new Rectangle(x, y+(i*m_title_snake_width), m_title_snake_width, m_title_snake_width, false, color, m_thickness));
    m_title->push_back(new Rectangle(x+(3*m_title_snake_width), y+(i*m_title_snake_width), m_title_snake_width, m_title_snake_width, false, color, m_thickness));
  }
  //Draw Horizontal Lines
  for (int i = 1; i < 3; i++)
  {
    m_title->push_back(new Rectangle(x+(i*m_title_snake_width), y, m_title_snake_width, m_title_snake_width, false, color, m_thickness));
  m_title->push_back(new Rectangle(x+(i*m_title_snake_width), y+(2*m_title_snake_width), m_title_snake_width, m_title_snake_width, false, color, m_thickness));
  }
}

void Menu::create_k(float x, float y)
{
  ALLEGRO_COLOR color = al_color_name("red");
  //Draw the left side
  for (int i = 0; i < 5; i++)
  {
    m_title->push_back(new Rectangle(x, y+(i*m_title_snake_width), m_title_snake_width, m_title_snake_width, false, color, m_thickness));
  }
  //Draw the rest of the letter
  m_title->push_back(new Rectangle(x+(1*m_title_snake_width), y+(2*m_title_snake_width), m_title_snake_width, m_title_snake_width, false, color, m_thickness));
  m_title->push_back(new Rectangle(x+(2*m_title_snake_width), y+(1*m_title_snake_width), m_title_snake_width, m_title_snake_width, false, color, m_thickness));
  m_title->push_back(new Rectangle(x+(2*m_title_snake_width), y+(3*m_title_snake_width), m_title_snake_width, m_title_snake_width, false, color, m_thickness));
  m_title->push_back(new Rectangle(x+(3*m_title_snake_width), y, m_title_snake_width, m_title_snake_width, false, color, m_thickness));
  m_title->push_back(new Rectangle(x+(3*m_title_snake_width), y+(4*m_title_snake_width), m_title_snake_width, m_title_snake_width, false, color, m_thickness));

}

void Menu::create_e(float x, float y)
{
  ALLEGRO_COLOR color = al_color_name("yellow");
  //Draw the left side
  for (int i = 0; i < 5; i++)
  {
    m_title->push_back(new Rectangle(x, y+(i*m_title_snake_width), m_title_snake_width, m_title_snake_width, false, color, m_thickness));
  }
  //Draw the horizontal lines
  for (int i = 1; i < 3; i++)
  {
    m_title->push_back(new Rectangle(x+(i*m_title_snake_width), y, m_title_snake_width, m_title_snake_width, false, color, m_thickness));
    m_title->push_back(new Rectangle(x+(i*m_title_snake_width), y+(2*m_title_snake_width), m_title_snake_width, m_title_snake_width, false, color, m_thickness));
    m_title->push_back(new Rectangle(x+(i*m_title_snake_width), y+(4*m_title_snake_width), m_title_snake_width, m_title_snake_width, false, color, m_thickness));
  }
}

void Menu::create_title(float x, float y)
{
  this->create_t(x, y);
  this->create_r(x+(6*m_title_snake_width), y);
  this->create_a(x+(11*m_title_snake_width), y);
  this->create_k(x+(16*m_title_snake_width), y);
  this->create_e(x+(21*m_title_snake_width), y);
}

void *Music_Thread::Music_Fade_Thread(ALLEGRO_THREAD *thread, void *arg)
{
  Music *music = (Music*)arg;
  music->fade_to_stop();
  return NULL;
}

Music *Music_Thread::Music_Load_Thread(ALLEGRO_THREAD *thread, void *arg)
{
  float *music_level = (float*)arg;
  return (new Music(*music_level));
}

