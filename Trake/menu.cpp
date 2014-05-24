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
  m_snake_width = snake_width * m_thickness;
  m_font = al_load_font("./fonts/Sabo-Regular.ttf", 72, 0);
  m_move_sound_up = al_load_sample("./sounds/pop1.wav");
  m_move_sound_down = al_load_sample("./sounds/pop2.wav");
  m_game = new Game();
  m_title = new std::vector<Rectangle*>();
  x = (m_screen_width - x)/2;
  this->create_title(x, m_snake_width);
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
  delete m_game;
}

void Menu::show()
{
  this->show_title();
}

void Menu::handle_input()
{
  switch (m_menu_screen)
  {
    case Menu::NONE:
      break;
    case Menu::TITLE:
      break;
    case Menu::PLAYERS:
      break;
    case Menu::GAMETYPE:
      break;
    case Menu::OPTIONS:
      break;
    default:
      assert(false);
  }
}

void Menu::show_title()
{
  int selection = 0;
  while (true)
  {
    m_menu_screen = Menu::TITLE;
    al_clear_to_color(al_color_name("black"));
    this->draw_title_logo();
    std::string items[3] = { "Start", "Options", "Exit" };

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
    al_wait_for_event(m_event, &e);
    if (e.type == ALLEGRO_EVENT_KEY_DOWN)
    {
      switch(e.keyboard.keycode)
      {
        case ALLEGRO_KEY_DOWN:
          selection = (selection + 1) % 3;
          if (m_move_sound_down) al_play_sample(m_move_sound_down, 2.5, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
          break;
        case ALLEGRO_KEY_UP:
          selection -= 1;
          if (selection < 0) selection = 2;
          if (m_move_sound_up) al_play_sample(m_move_sound_up, 2.5, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
          break;
        case ALLEGRO_KEY_ENTER:
          if (m_move_sound_down) al_play_sample(m_move_sound_down, 2.5, 0.0, 1.5, ALLEGRO_PLAYMODE_ONCE, NULL);
          this->draw_loading();
          al_flip_display();
          return;
      }
    }
  }
}

void Menu::draw_loading()
{
  al_clear_to_color(al_color_name("black"));
  al_draw_text(m_font, al_color_name("yellow"), m_screen_width/2, m_screen_height/2, ALLEGRO_ALIGN_CENTER, "Loading...");
}

void Menu::draw_title_logo()
{
  //Title logo's size is m_snake_width * 24
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
    m_title->push_back(new Rectangle(x+(i*m_snake_width), y, m_snake_width, m_snake_width, false, color, m_thickness));
    if (i == 2) vertical_x = x+(i*m_snake_width);
  }
  //Finish with a vertical line
  for (int i = 1; i < 5; i++)
  {
    m_title->push_back(new Rectangle(vertical_x, y+(i*m_snake_width), m_snake_width, m_snake_width, false, color, m_thickness));
  }
}

void Menu::create_r(float x, float y)
{
  ALLEGRO_COLOR color = al_color_name("blue");
  //Start with a vertical line
  for (int i = 0; i < 5; i++)
  {
    m_title->push_back(new Rectangle(x, y+(i*m_snake_width), m_snake_width, m_snake_width, false, color, m_thickness));
  }
  //Draw the horizontal lines
  for (int i = 1; i < 3; i++)
  {
    m_title->push_back(new Rectangle(x+(i*m_snake_width), y, m_snake_width, m_snake_width, false, color, m_thickness));
    if (i != 3) m_title->push_back(new Rectangle(x+(i*m_snake_width), y+(2*m_snake_width), m_snake_width, m_snake_width, false, color, m_thickness));
  }
  m_title->push_back(new Rectangle(x+(3*m_snake_width), y+(1*m_snake_width), m_snake_width, m_snake_width, false, color, m_thickness));
  m_title->push_back(new Rectangle(x+(3*m_snake_width), y+(3*m_snake_width), m_snake_width, m_snake_width, false, color, m_thickness));
  m_title->push_back(new Rectangle(x+(3*m_snake_width), y+(4*m_snake_width), m_snake_width, m_snake_width, false, color, m_thickness));
}

void Menu::create_a(float x, float y)
{
  ALLEGRO_COLOR color = al_color_name("white");
  //Draw Vertical Lines
  for (int i = 1; i < 5; i++)
  {
    m_title->push_back(new Rectangle(x, y+(i*m_snake_width), m_snake_width, m_snake_width, false, color, m_thickness));
    m_title->push_back(new Rectangle(x+(3*m_snake_width), y+(i*m_snake_width), m_snake_width, m_snake_width, false, color, m_thickness));
  }
  //Draw Horizontal Lines
  for (int i = 1; i < 3; i++)
  {
    m_title->push_back(new Rectangle(x+(i*m_snake_width), y, m_snake_width, m_snake_width, false, color, m_thickness));
  m_title->push_back(new Rectangle(x+(i*m_snake_width), y+(2*m_snake_width), m_snake_width, m_snake_width, false, color, m_thickness));
  }
}

void Menu::create_k(float x, float y)
{
  ALLEGRO_COLOR color = al_color_name("red");
  //Draw the left side
  for (int i = 0; i < 5; i++)
  {
    m_title->push_back(new Rectangle(x, y+(i*m_snake_width), m_snake_width, m_snake_width, false, color, m_thickness));
  }
  //Draw the rest of the letter
  m_title->push_back(new Rectangle(x+(1*m_snake_width), y+(2*m_snake_width), m_snake_width, m_snake_width, false, color, m_thickness));
  m_title->push_back(new Rectangle(x+(2*m_snake_width), y+(1*m_snake_width), m_snake_width, m_snake_width, false, color, m_thickness));
  m_title->push_back(new Rectangle(x+(2*m_snake_width), y+(3*m_snake_width), m_snake_width, m_snake_width, false, color, m_thickness));
  m_title->push_back(new Rectangle(x+(3*m_snake_width), y, m_snake_width, m_snake_width, false, color, m_thickness));
  m_title->push_back(new Rectangle(x+(3*m_snake_width), y+(4*m_snake_width), m_snake_width, m_snake_width, false, color, m_thickness));

}

void Menu::create_e(float x, float y)
{
  ALLEGRO_COLOR color = al_color_name("yellow");
  //Draw the left side
  for (int i = 0; i < 5; i++)
  {
    m_title->push_back(new Rectangle(x, y+(i*m_snake_width), m_snake_width, m_snake_width, false, color, m_thickness));
  }
  //Draw the horizontal lines
  for (int i = 1; i < 3; i++)
  {
    m_title->push_back(new Rectangle(x+(i*m_snake_width), y, m_snake_width, m_snake_width, false, color, m_thickness));
    m_title->push_back(new Rectangle(x+(i*m_snake_width), y+(2*m_snake_width), m_snake_width, m_snake_width, false, color, m_thickness));
    m_title->push_back(new Rectangle(x+(i*m_snake_width), y+(4*m_snake_width), m_snake_width, m_snake_width, false, color, m_thickness));
  }
}

void Menu::create_title(float x, float y)
{
  this->create_t(x, y);
  this->create_r(x+(6*m_snake_width), y);
  this->create_a(x+(11*m_snake_width), y);
  this->create_k(x+(16*m_snake_width), y);
  this->create_e(x+(21*m_snake_width), y);
}

