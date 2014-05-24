#include "menu.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_color.h>
#include <iterator>


Menu::Menu(float screen_width, float screen_height, float snake_width)
{
  m_screen_width = screen_width;
  m_screen_height = screen_height;
  m_snake_width = snake_width;
  m_title = new std::vector<Rectangle*>();
  this->create_title(0.0, 0.0);
}

Menu::~Menu()
{
  for (std::vector<Rectangle*>::iterator iter = m_title->begin(); iter != m_title->end(); ++iter)
  {
    delete (*iter);
  }
  m_title->clear();
}

void Menu::show()
{
  this->draw();
  al_rest(5);
}

void Menu::draw()
{
  for (std::vector<Rectangle*>::iterator iter = m_title->begin(); iter != m_title->end(); ++iter)
  {
    if (*iter) (*iter)->draw();
  }
  al_flip_display();
}

void Menu::create_t(float x, float y)
{
  ALLEGRO_COLOR color = al_color_name("lawngreen");
  float vertical_x = 0.0;
  //Start at the top
  for (int i = 0; i < 5; i++)
  {
    m_title->push_back(new Rectangle(x+(i*m_snake_width), y, m_snake_width, m_snake_width, false, color));
    if (i == 2) vertical_x = x+(i*m_snake_width);
  }
  //Finish with a vertical line
  for (int i = 1; i < 5; i++)
  {
    m_title->push_back(new Rectangle(vertical_x, y+(i*m_snake_width), m_snake_width, m_snake_width, false, color));
  }
}

void Menu::create_r(float x, float y)
{
  ALLEGRO_COLOR color = al_color_name("blue");
  //Start with a vertical line
  for (int i = 0; i < 5; i++)
  {
    m_title->push_back(new Rectangle(x, y+(i*m_snake_width), m_snake_width, m_snake_width, false, color));
  }
  //Draw the horizontal lines
  for (int i = 1; i < 4; i++)
  {
    m_title->push_back(new Rectangle(x+(i*m_snake_width), y, m_snake_width, m_snake_width, false, color));
    if (i != 3) m_title->push_back(new Rectangle(x+(i*m_snake_width), y+(2*m_snake_width), m_snake_width, m_snake_width, false, color));
  }
  m_title->push_back(new Rectangle(x+(3*m_snake_width), y+(1*m_snake_width), m_snake_width, m_snake_width, false, color));
  m_title->push_back(new Rectangle(x+(3*m_snake_width), y+(3*m_snake_width), m_snake_width, m_snake_width, false, color));
  m_title->push_back(new Rectangle(x+(3*m_snake_width), y+(4*m_snake_width), m_snake_width, m_snake_width, false, color));
}

void Menu::create_title(float x, float y)
{
  this->create_t(x, y);
  this->create_r(x+(6*m_snake_width), y);
}

