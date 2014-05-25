#include "game.h"
#include "input.h"
#include "ai.h"
#include "collision_table.h"
#include "pellet.h"
#include "snake.h"
#include "music.h"
#include <allegro5/allegro_color.h>
#include <assert.h>

Game::Game(ALLEGRO_EVENT_QUEUE *event, float screen_width, float screen_height, float snake_width, int human_players, int ai_players, int gametype, int win_condition, int rounds)
{
  assert(event);
  assert(human_players + ai_players <= 4);
  assert(human_players + ai_players > 0);
  assert(screen_width > 0);
  assert(screen_height > 0);
  m_event = event;
  m_screen_width = screen_width;
  m_screen_height = screen_height;
  m_snake_width = snake_width;
  m_rounds = rounds;
  m_max_x = 0;
  m_max_y = 0;
  while (m_max_x < m_screen_width-(m_snake_width*2))
  {
    m_max_x += m_snake_width;
  }
  while (m_max_y < m_screen_height-(m_snake_width*2))
  {
    m_max_y += m_snake_width;
  }
  assert(m_max_x > 0);
  assert(m_max_y > 0);
  m_num_snakes = ai_players + human_players;
  m_num_ai = ai_players;
  if (gametype == 0)
    m_tron = false;
  else
    m_tron = true;
  m_win_condition = win_condition;
  m_start_snake_length = 5;
  m_max_spawn_time = 100;
  m_player_1_start_x = m_max_x;
  m_player_2_start_x = 0;
  m_player_3_start_x = 0;
  m_player_4_start_x = m_max_x;
  m_player_1_start_y = 0;
  m_player_2_start_y = 0;
  m_player_3_start_y = m_max_y;
  m_player_4_start_y = m_max_y;
  m_font = al_load_font("./fonts/Sabo-Regular.ttf", 72, 0);
  m_collision_table = NULL;
  m_pellet = NULL;
  m_music = NULL;
  for (int i = 0; i < 4; i++)
  {
    m_ai[i] = NULL;
    m_snakes[i] = NULL;
  }
}

Game::~Game()
{
  al_destroy_font(m_font);
}

void Game::run()
{
  this->draw_loading();
  al_flip_display();
  for (int rounds = m_rounds; rounds > 0; rounds--)
  {
    //Initialize Collision Table
    m_collision_table = new Collision_Table();

    //Initialize Pellet
    m_pellet = new Pellet(m_snake_width, m_max_x, m_max_y, m_max_spawn_time, m_collision_table, m_tron);

    //Initialize Snakes
    m_snakes[0] = new Snake(0, m_player_1_start_x, m_player_1_start_y, Input::LEFT, m_start_snake_length, al_color_name("lawngreen"), m_snake_width, m_max_x, m_max_y, true, m_collision_table, m_tron);
    if (m_num_snakes >= 2) m_snakes[1] = new Snake(1, m_player_2_start_x, m_player_2_start_y, Input::DOWN, m_start_snake_length, al_color_name("blue"), m_snake_width, m_max_x, m_max_y, true, m_collision_table, m_tron);
    if (m_num_snakes >= 3) m_snakes[2] = new Snake(2, m_player_3_start_x, m_player_3_start_y, Input::RIGHT, m_start_snake_length, al_color_name("red"), m_snake_width, m_max_x, m_max_y, true, m_collision_table, m_tron);
    if (m_num_snakes >= 4) m_snakes[3] = new Snake(3, m_player_4_start_x, m_player_4_start_y, Input::UP, m_start_snake_length, al_color_name("yellow"), m_snake_width, m_max_x, m_max_y, true, m_collision_table, m_tron);

    //Initialize AI
    for (int i = 0; i < 4; i++)
    {
      if (m_num_snakes >= i+1 && m_num_snakes - m_num_ai < i+1)
        m_ai[i] = new AI(m_snakes, i, m_pellet, m_collision_table, m_tron);
    }

    //Prepare Input Thread
    m_music = new Music(m_tron);
    bool paused = false;
    bool quit = false;
    float wait_time = 0.05;
    Input::Input_Thread_Data data;
    for (int i = 0; i < 4; i++)
    {
      data.ai[i] = m_ai[i];
      data.snakes[i] = m_snakes[i];
    }
    data.music = m_music;
    data.rounds = &rounds;
    data.event = m_event;
    data.paused = &paused;
    data.quit = &quit;
    ALLEGRO_THREAD *input_thread = al_create_thread(Input::Input_Thread, &data);
    al_start_thread(input_thread);

    //Start Music
    m_music->play();

    //Clear the Screen
    al_clear_to_color(al_color_name("black"));
    al_flip_display();

    while (!quit)
    {
      if (paused)
      {
        al_rest(0.1);
        continue;
      }

      m_pellet->handle_state();
      for (int i = 0; i < 4; i++)
      {
        if (m_ai[i]) m_ai[i]->read_input();
        if (m_snakes[i]) m_snakes[i]->move();
      }
      if (wait_time > 0)
      {
        al_rest(wait_time);
        wait_time -= 0.00001; //slowly get faster
      }
      if (m_win_condition == 0)
      {
        if (!is_anyone_alive())
          quit = true;
      }
      else
      {
        if (how_many_are_alive() <= 1)
          quit = true;
      }
      al_flip_display();
    }
    m_music->slow_to_stop();

    //Show the Game Over Screen
    if (rounds <= 1)
    {
      al_clear_to_color(al_color_name("black"));
      al_draw_text(m_font, al_color_name("lawngreen"), m_screen_width/2, m_screen_height/2, ALLEGRO_ALIGN_CENTER, "Game Over");
      al_flip_display();
      al_rest(3);
    }
    else
    {
      this->draw_loading();
      al_flip_display();
    }

    al_flush_event_queue(m_event);
    al_join_thread(input_thread, NULL);

    //Deallocate Memory
    for (int i = 0; i < 4; i++)
    {
      delete m_snakes[i];
      m_snakes[i] = NULL;
      delete m_ai[i];
      m_ai[i] = NULL;
    }
    delete m_pellet;
    m_pellet = NULL;
    delete m_collision_table;
    m_collision_table = NULL;
    delete m_music;
    m_music = NULL;
  }
}

bool Game::is_anyone_alive()
{
  for (int i = 0; i < 4; i++)
  {
    if (m_snakes[i] && !m_snakes[i]->is_dead())
      return true;
  }
  return false;
}

int Game::how_many_are_alive()
{
  int alive = 0;
  for (int i = 0; i < 4; i++)
  {
    if (m_snakes[i] && !m_snakes[i]->is_dead())
      ++alive;
  }
  return alive;
}

void Game::draw_loading()
{
  al_clear_to_color(al_color_name("black"));
  al_draw_text(m_font, al_color_name("yellow"), m_screen_width/2, m_screen_height/2, ALLEGRO_ALIGN_CENTER, "Loading...");
}

