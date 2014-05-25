#include "scoreboard.h"
#include "snake.h"
#include "rectangle.h"
#include <allegro5/allegro_color.h>
#include <assert.h>

Scoreboard::Scoreboard(float screen_width, float screen_height, float max_x, float max_y, int num_snakes, Snake *snakes[])
{
  m_screen_width = screen_width;
  m_screen_height = screen_height;
  m_max_x = max_x;
  m_max_y = max_y;
  m_num_snakes = num_snakes;
  for (int i = 0; i < 4; i++)
  {
    m_snakes[i] = snakes[i];
    m_backgrounds[i] = NULL;
  }
  m_font = al_load_font("./fonts/Sabo-Regular.ttf", 16, 0);
  m_width = m_screen_width;
  m_height = m_screen_height - m_max_y;
  float x = 0;
  float y = m_screen_height - m_height;
  for (int i = 0; i < m_num_snakes; i++)
  {
    ALLEGRO_COLOR color;
    switch (i)
    {
      case 0:
        color = al_color_name("lawngreen");
        break;
      case 1:
        color = al_color_name("blue");
        break;
      case 2:
        color = al_color_name("yellow");
        break;
      case 3:
        color = al_color_name("red");
        break;
      default:
        assert(false);
    }
    m_backgrounds[i] = new Rectangle(x, y, m_width/m_num_snakes, m_height, true, color);
    x += m_screen_width / m_num_snakes;
  }
}

Scoreboard::~Scoreboard()
{
  for (int i = 0; i < m_num_snakes; i++)
  {
    delete m_backgrounds[i];
  }
  al_destroy_font(m_font);
}

void Scoreboard::draw()
{
  for (int i = 0; i < m_num_snakes; i++)
  {
    m_backgrounds[i]->draw();
  }
}

