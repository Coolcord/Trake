#include "scoreboard.h"
#include "snake.h"
#include "rectangle.h"
#include <allegro5/allegro_color.h>
#include <string>
#include <assert.h>

Scoreboard::Scoreboard(float screen_width, float screen_height, float snake_width, float y, int num_snakes, int player_scores[], Snake *snakes[])
{
  m_screen_width = screen_width;
  m_screen_height = screen_height;
  m_snake_width = snake_width;
  m_y = y;
  m_num_snakes = num_snakes;
  for (int i = 0; i < 4; i++)
  {
    m_player_scores[i] = player_scores[i];
    m_snakes[i] = snakes[i];
    m_backgrounds[i] = NULL;
    m_text_x[i] = 0;
  }
  m_font = al_load_font("./fonts/Sabo-Regular.ttf", 32, 0);
  m_width = m_screen_width;
  m_height = m_screen_height - m_y;
  m_text_y = m_screen_height - (m_height / 2);
  float x = 0;
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
        color = al_color_name("red");
        break;
      case 3:
        color = al_color_name("yellow");
        break;
      default:
        assert(false);
    }
    m_text_x[i] = ((m_width/m_num_snakes) * (i+1)) - ((m_width/m_num_snakes)/2);
    m_backgrounds[i] = new Rectangle(x, y, m_width/m_num_snakes, m_height, true, color, m_snake_width/5);
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
  al_draw_filled_rectangle(0, m_y-m_snake_width, m_screen_width, m_y, al_color_name("purple"));
  for (int i = 0; i < m_num_snakes; i++)
  {
    this->draw_player_score(m_snakes[i]);
  }
}

void Scoreboard::draw_player_score(Snake *snake)
{
  assert(snake);
  int player_num = snake->get_player_num();
  ALLEGRO_COLOR text_color;
  if (snake->is_dead())
  {
    text_color = al_color_name("white");
    ALLEGRO_COLOR tmp_color = m_backgrounds[player_num]->get_color();
    m_backgrounds[player_num]->set_color(al_color_name("black"));
    m_backgrounds[player_num]->set_filled(true);
    m_backgrounds[player_num]->draw();
    m_backgrounds[player_num]->set_color(tmp_color);
    m_backgrounds[player_num]->set_filled(false);
    m_backgrounds[player_num]->draw();
  }
  else
  {
    text_color = al_color_name("black");
    m_backgrounds[player_num]->set_filled(true);
    m_backgrounds[player_num]->draw();
  }
  std::string text_header = "Score: ";
  al_draw_text(m_font, text_color, m_text_x[player_num], m_y, ALLEGRO_ALIGN_CENTER, text_header.c_str());
  al_draw_text(m_font, text_color, m_text_x[player_num], m_y + (m_height / 2), ALLEGRO_ALIGN_CENTER, (std::to_string(m_player_scores[player_num])).c_str());
}

void Scoreboard::increment_score_by_one(Snake *snake)
{
  assert(snake);
  int player_num = snake->get_player_num();
  ++m_player_scores[player_num];
}

int Scoreboard::get_player_score(int player_num)
{
  return m_player_scores[player_num];
}

