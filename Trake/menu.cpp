#include "menu.h"
#include "rectangle.h"
#include "game.h"
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
  m_menu_screen = Menu::NONE;
  m_event = event;
  m_selection = 0;
  m_screen_width = screen_width;
  m_screen_height = screen_height;
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
  m_font = al_load_font("./fonts/Sabo-Regular.ttf", 72, 0);
  m_move_sound_up = al_load_sample("./sounds/pop2.wav");
  m_move_sound_down = al_load_sample("./sounds/pop1.wav");
  m_game = NULL;
  m_title = new std::vector<Rectangle*>();
  x = (m_screen_width - x)/2;
  this->create_title(x, m_title_snake_width);
}

Menu::~Menu()
{
  for (std::vector<Rectangle*>::iterator iter = m_title->begin(); iter != m_title->end(); ++iter)
  {
    delete (*iter);
  }
  m_title->clear();
  al_destroy_sample(m_move_sound_down);
  al_destroy_sample(m_move_sound_up);
  al_destroy_font(m_font);
}

void Menu::show()
{
  this->show_title();
}

void Menu::show_title()
{
  int selection = 0;
  std::string items[3] = { "Start", "Options", "Exit" };
  while (true)
  {
    m_menu_screen = Menu::TITLE;
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
      al_draw_text(m_font, color, m_screen_width/2, ((m_screen_height/8)*5)+(i*80), ALLEGRO_ALIGN_CENTER, items[i].c_str());
    }
    al_flip_display();
    
    ALLEGRO_EVENT e;
    assert(m_event);
    al_wait_for_event(m_event, &e);
    if (e.type == ALLEGRO_EVENT_KEY_DOWN)
    {
      switch(e.keyboard.keycode)
      {
        case ALLEGRO_KEY_DOWN:
          if (m_move_sound_down) al_play_sample(m_move_sound_down, 2.5, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
          selection = (selection + 1) % 3;
          break;
        case ALLEGRO_KEY_UP:
          if (m_move_sound_up) al_play_sample(m_move_sound_up, 2.5, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
          selection -= 1;
          if (selection < 0) selection = 2;
          break;
        case ALLEGRO_KEY_ENTER:
          switch (selection)
          {
            case 0: //Start
              if (m_move_sound_up) al_play_sample(m_move_sound_down, 2.5, 0.0, 1.5, ALLEGRO_PLAYMODE_ONCE, NULL);
              this->show_game_setup();
              break;
            case 1: //Options
              if (m_move_sound_up) al_play_sample(m_move_sound_down, 2.5, 0.0, 1.5, ALLEGRO_PLAYMODE_ONCE, NULL);
              this->show_options();
              break;
            case 2: //Exit
              if (m_move_sound_down) al_play_sample(m_move_sound_down, 2.5, 0.0, 0.7, ALLEGRO_PLAYMODE_ONCE, NULL);
              al_rest(2);
              return;
            default:
              assert(false);
          }
          break;
      }
    }
  }
}

void Menu::show_game_setup()
{
  int selection = 0;
  int gametype_selection = 0;
  int win_selection = 0;
  int human_players = 1;
  int ai_players = 3;
  std::string items[6] = { "Play!", "Game Type:", "Win Condition:", "Human Players:", "AI Players:", "Back" };
    std::string gametype_items[2] = { "<   Snake   >", "<   Tron   >" };
    std::string win_condition_items[2] = { "<   Score   >", "<   Survival   >" };
  while (true)
  {
    m_menu_screen = Menu::GAME_SETUP;
    al_clear_to_color(al_color_name("black"));

    //Draw Selections
    int y = m_screen_height/50;
    for (int i = 0; i < 6; i++)
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
      al_draw_text(m_font, color, m_screen_width/2, y, ALLEGRO_ALIGN_CENTER, items[i].c_str());
      y += 80;
      switch (i)
      {
        case 1: //GameType
          al_draw_text(m_font, color, m_screen_width/2, y, ALLEGRO_ALIGN_CENTER, gametype_items[gametype_selection].c_str());
          y += 80;
          break;
        case 2: //Win Condition
          al_draw_text(m_font, color, m_screen_width/2, y, ALLEGRO_ALIGN_CENTER, win_condition_items[win_selection].c_str());
          y += 80;
          break;
        case 3: //Human Players
          al_draw_text(m_font, color, m_screen_width/2, y, ALLEGRO_ALIGN_CENTER, ("<   " + std::to_string(human_players) + "   >").c_str());
          y += 80;
          break;
        case 4: //AI Players
          al_draw_text(m_font, color, m_screen_width/2, y, ALLEGRO_ALIGN_CENTER, ("<   " + std::to_string(ai_players) + "   >").c_str());
          y += 80;
          break;
      }
    }
    al_flip_display();
    
    ALLEGRO_EVENT e;
    al_wait_for_event(m_event, &e);
    if (e.type == ALLEGRO_EVENT_KEY_DOWN)
    {
      switch(e.keyboard.keycode)
      {
        case ALLEGRO_KEY_LEFT:
          switch (selection)
          {
            case 1: //GameType
              if (m_move_sound_down) al_play_sample(m_move_sound_down, 2.5, 0.0, 1.2, ALLEGRO_PLAYMODE_ONCE, NULL);
              --gametype_selection;
              if (gametype_selection < 0)
              gametype_selection = 1;
              break;
            case 2: //Win Condition
              if (m_move_sound_down) al_play_sample(m_move_sound_down, 2.5, 0.0, 1.2, ALLEGRO_PLAYMODE_ONCE, NULL);
              --win_selection;
              if (win_selection < 0)
                win_selection = 1;
              break;
            case 3: //Human Players
              if (m_move_sound_down) al_play_sample(m_move_sound_down, 2.5, 0.0, 1.2, ALLEGRO_PLAYMODE_ONCE, NULL);
              --human_players;
              if (human_players < 0)
                human_players = 0;
              if (ai_players == 0 && human_players == 0)
                human_players = 1;
              while (ai_players + human_players > 4)
                --ai_players;
              break;
            case 4: //AI Players
              if (m_move_sound_down) al_play_sample(m_move_sound_down, 2.5, 0.0, 1.2, ALLEGRO_PLAYMODE_ONCE, NULL);
              --ai_players;
              if (ai_players < 0)
                ai_players = 0;
              if (ai_players == 0 && human_players == 0)
                human_players = 1;
              while (ai_players + human_players > 4)
                --human_players;
              break;
          }
          break;
        case ALLEGRO_KEY_RIGHT:
          switch (selection)
          {
            case 1: //GameType
              if (m_move_sound_up) al_play_sample(m_move_sound_down, 2.5, 0.0, 1.2, ALLEGRO_PLAYMODE_ONCE, NULL);
              gametype_selection = (gametype_selection + 1) % 2;
              break;
            case 2: //Win Condition
              if (m_move_sound_up) al_play_sample(m_move_sound_down, 2.5, 0.0, 1.2, ALLEGRO_PLAYMODE_ONCE, NULL);
              win_selection = (win_selection + 1) % 2;
              break;
            case 3: //Human Players
              if (m_move_sound_up) al_play_sample(m_move_sound_down, 2.5, 0.0, 1.2, ALLEGRO_PLAYMODE_ONCE, NULL);
              ++human_players;
              if (human_players > 4)
                human_players = 4;
              if (ai_players == 0 && human_players == 0)
                human_players = 1;
              while (ai_players + human_players > 4)
                --ai_players;
              break;
            case 4: //AI Players
              if (m_move_sound_up) al_play_sample(m_move_sound_down, 2.5, 0.0, 1.2, ALLEGRO_PLAYMODE_ONCE, NULL);
              ++ai_players;
              if (ai_players > 4)
                ai_players = 4;
              if (ai_players == 0 && human_players == 0)
                human_players = 1;
              while (ai_players + human_players > 4)
                --human_players;
              break;
          }
          break;
        case ALLEGRO_KEY_DOWN:
          if (m_move_sound_down) al_play_sample(m_move_sound_down, 2.5, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
          selection = (selection + 1) % 6;
          break;
        case ALLEGRO_KEY_UP:
          if (m_move_sound_up) al_play_sample(m_move_sound_up, 2.5, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
          selection -= 1;
          if (selection < 0) selection = 5;
          break;
        case ALLEGRO_KEY_ENTER:
          switch (selection)
          {
            case 0: //Play!
              if (m_move_sound_up) al_play_sample(m_move_sound_down, 2.5, 0.0, 1.5, ALLEGRO_PLAYMODE_ONCE, NULL);
              this->draw_loading();
              al_flip_display();
              m_game = new Game(m_event, m_screen_width, m_screen_height, m_snake_width, human_players, ai_players, gametype_selection, win_selection);
              m_game->run();
              delete m_game;
              m_game = NULL;
              return;
            case 5: //Back
              if (m_move_sound_down) al_play_sample(m_move_sound_down, 2.5, 0.0, 0.7, ALLEGRO_PLAYMODE_ONCE, NULL);
              return;
          }
          break;
      }
    }
  }
}

void Menu::show_options()
{

}

void Menu::show_control_setup()
{

}

void Menu::show_credits()
{

}

void Menu::draw_loading()
{
  al_clear_to_color(al_color_name("black"));
  al_draw_text(m_font, al_color_name("yellow"), m_screen_width/2, m_screen_height/2, ALLEGRO_ALIGN_CENTER, "Loading...");
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

