#include "game.h"

Game::Game(float screen_width, float screen_height, float snake_width)
{
  m_screen_width = screen_width;
  m_screen_height = screen_height;
  m_snake_width = screen_width;
  m_font = al_load_font("./fonts/Sabo-Regular.ttf", 72, 0);
}

Game::~Game()
{
  al_destroy_font(m_font);
}

void Game::run()
{
  while (!quit)
  {
    if (paused)
    {
      al_rest(0.1);
      continue;
    }

    pellet->handle_state();
    for (int i = 0; i < 4; i++)
    {
      if (ai[i]) ai[i]->read_input();
      if (snakes[i]) snakes[i]->move();
    }
    if (wait_time > 0)
    {
      al_rest(wait_time);
      wait_time -= 0.00001; //slowly get faster
    }
    if (!is_anyone_alive(snakes))
      quit = true;
    al_flip_display();
  }
  music->slow_to_stop();

  al_clear_to_color(al_color_name("black"));
  al_draw_text(font, al_color_name("lawngreen"), screen_width/2, screen_height/2, ALLEGRO_ALIGN_CENTER, "Game Over");
  al_flip_display();
  al_rest(3);
}

