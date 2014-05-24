#include "menu.h"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_color.h>
#include <iterator>


Menu::Menu(float screen_width, float screen_height, float snake_width)
{
  m_screen_width = screen_width;
  m_screen_height = screen_height;
  m_thickness = 4;
  m_snake_width = snake_width * m_thickness;
  m_font = al_load_font("./fonts/Sabo-Regular.ttf", 72, 0);
  m_title = new std::vector<Rectangle*>();
  this->create_title(m_snake_width/2, m_snake_width);
}

Menu::~Menu()
{
  for (std::vector<Rectangle*>::iterator iter = m_title->begin(); iter != m_title->end(); ++iter)
  {
    delete (*iter);
  }
  m_title->clear();
  al_destroy_font(m_font);
}

void Menu::show()
{
  this->show_main();
}

void Menu::show_main()
{
  this->draw();
  al_draw_text(m_font, al_color_name("white"), m_screen_width / 2, (m_screen_height/8)*5, ALLEGRO_ALIGN_CENTER, "Start");
  al_draw_text(m_font, al_color_name("white"), m_screen_width / 2, ((m_screen_height/8)*5) + 80, ALLEGRO_ALIGN_CENTER, "Options");
  al_draw_text(m_font, al_color_name("white"), m_screen_width / 2, ((m_screen_height/8)*5) + 160, ALLEGRO_ALIGN_CENTER, "Exit");
  al_flip_display();
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
  for (int i = 1; i < 4; i++)
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
  for (int i = 1; i < 4; i++)
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

