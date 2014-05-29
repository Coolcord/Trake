#include "game.h"
#include "input.h"
#include "ai.h"
#include "controls.h"
#include "collision_table.h"
#include "pellet.h"
#include "snake.h"
#include "music.h"
#include "scoreboard.h"
#include "pause_menu.h"
#include <allegro5/allegro_color.h>
#include <assert.h>
#include <algorithm>
#include <unordered_map>
#include <iterator>

Game::Game(ALLEGRO_EVENT_QUEUE *event, Controls *controls, ALLEGRO_THREAD *music_fade_thread, float screen_width, float screen_height, float snake_width, float font_small_incrementor, float font_medium_incrementor, float font_large_incrementor, ALLEGRO_FONT *font_small, ALLEGRO_FONT *font_medium, ALLEGRO_FONT *font_large, float music_level, float sound_effects_level, int human_players, int ai_players, int gametype, int win_condition, int rounds, ALLEGRO_SAMPLE *move_sound_down, ALLEGRO_SAMPLE *move_sound_up)
{
  assert(event);
  assert(controls);
  assert(human_players + ai_players <= 4);
  assert(human_players + ai_players > 0);
  assert(screen_width > 0);
  assert(screen_height > 0);
  m_event = event;
  m_music_fade_thread = music_fade_thread;
  m_screen_width = screen_width;
  m_screen_height = screen_height;
  m_snake_width = snake_width;
  m_font_small_incrementor = font_small_incrementor;
  m_font_medium_incrementor = font_medium_incrementor;
  m_font_large_incrementor = font_large_incrementor;
  m_font_small = font_small;
  m_font_medium = font_medium;
  m_font_large = font_large;
  m_music_level = music_level;
  m_win_condition = win_condition;
  m_rounds = rounds;
  m_controls = controls;
  m_move_sound_down = move_sound_down;
  m_move_sound_up = move_sound_up;
  m_sound_effects_level = sound_effects_level;
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
  m_scoreboard_y = m_max_y - (m_snake_width * 4);
  if (m_win_condition == 0)
    m_game_height = m_scoreboard_y - (m_snake_width*2);
  else
    m_game_height = m_max_y;
  m_num_snakes = ai_players + human_players;
  m_num_ai = ai_players;
  if (gametype == 0)
    m_tron = false;
  else
    m_tron = true;
  m_start_snake_length = 5;
  m_max_spawn_time = 100;
  m_player_1_start_x = m_max_x;
  m_player_2_start_x = 0;
  m_player_3_start_x = 0;
  m_player_4_start_x = m_max_x;
  m_player_1_start_y = 0;
  m_player_2_start_y = 0;
  m_player_3_start_y = m_game_height;
  m_player_4_start_y = m_game_height;
  m_scoreboard = NULL;
  m_collision_table = NULL;
  m_pellet = NULL;
  m_music = NULL;
  m_pause_menu = NULL;
  for (int i = 0; i < 4; i++)
  {
    m_player_scores[i] = 0;
    m_ai[i] = NULL;
    m_snakes[i] = NULL;
  }
}

Game::~Game()
{
  
}

void Game::run()
{
  this->draw_loading(1);
  al_flip_display();

  for (int rounds = 1; rounds <= m_rounds; rounds++)
  {
    al_clear_to_color(al_color_name("black"));
    //Initialize Collision Table
    m_collision_table = new Collision_Table();

    //Initialize Snakes
    m_snakes[0] = new Snake(0, m_player_1_start_x, m_player_1_start_y, m_sound_effects_level, Input::LEFT, m_start_snake_length, al_color_name("lawngreen"), m_snake_width, m_max_x, m_game_height, true, m_collision_table, m_tron);
    if (m_num_snakes >= 2) m_snakes[1] = new Snake(1, m_player_2_start_x, m_player_2_start_y, m_sound_effects_level, Input::DOWN, m_start_snake_length, al_color_name("blue"), m_snake_width, m_max_x, m_game_height, true, m_collision_table, m_tron);
    if (m_num_snakes >= 3) m_snakes[2] = new Snake(2, m_player_3_start_x, m_player_3_start_y, m_sound_effects_level, Input::RIGHT, m_start_snake_length, al_color_name("red"), m_snake_width, m_max_x, m_game_height, true, m_collision_table, m_tron);
    if (m_num_snakes >= 4) m_snakes[3] = new Snake(3, m_player_4_start_x, m_player_4_start_y, m_sound_effects_level, Input::UP, m_start_snake_length, al_color_name("yellow"), m_snake_width, m_max_x, m_game_height, true, m_collision_table, m_tron);

    //Prepare the Scoreboard
    if (m_win_condition == 0)
    {
      m_scoreboard = new Scoreboard(m_screen_width, m_screen_height, m_snake_width, m_font_small_incrementor, m_font_small, m_scoreboard_y, m_num_snakes, m_player_scores, m_snakes);
    }
    else
    {
      m_scoreboard = NULL;
    }

    //Send the Scoreboard to the snake objects
    for (int i = 0; i < m_num_snakes; i++)
    {
      m_snakes[i]->set_scoreboard(m_scoreboard);
    }
    int num_alive = m_num_snakes;

    //Initialize the Pause Menu
    bool hide_standing = false;
    bool paused = false;
    bool quit = false;
    m_pause_menu = new Pause_Menu(m_event, m_controls, m_screen_width, m_screen_height, m_font_large_incrementor, m_font_large, m_move_sound_down, m_move_sound_up, m_sound_effects_level, &quit, &hide_standing, &rounds, m_rounds);

    //Initialize Pellet
    m_pellet = new Pellet(m_snake_width, m_max_x, m_game_height, m_sound_effects_level, m_max_spawn_time, m_scoreboard, m_collision_table, m_tron);

    //Initialize AI
    for (int i = 0; i < 4; i++)
    {
      if (m_num_snakes >= i+1 && m_num_snakes - m_num_ai < i+1)
        m_ai[i] = new AI(m_snakes, i, m_pellet, m_collision_table, m_tron);
    }

    //Prepare Input Thread
    m_music = new Music(m_music_level, m_tron);
    float wait_time = 0.05;
    Input::Input_Thread_Data data;
    for (int i = 0; i < 4; i++)
    {
      data.ai[i] = m_ai[i];
      data.snakes[i] = m_snakes[i];
    }
    data.controls = m_controls;
    data.event = m_event;
    data.paused = &paused;
    data.quit = &quit;
    ALLEGRO_THREAD *input_thread = al_create_thread(Input::Input_Thread, &data);
    al_start_thread(input_thread);

    //Start Music
    if (m_music_fade_thread)
    { //Stop music from menu if playing
      al_join_thread(m_music_fade_thread, NULL);
      m_music_fade_thread = NULL;
    }
    m_music->play();

    if (m_scoreboard)
      m_scoreboard->draw();
    al_flip_display();

    while (!quit)
    {
      if (paused)
      {
        m_music->pause();
        m_pause_menu->show();
        paused = false;
        m_music->resume();
        //Redraw Everything
        al_clear_to_color(al_color_name("black"));
        for (int i = 0; i < m_num_snakes; i++)
        {
          m_snakes[i]->draw();
        }
        m_pellet->draw();
        if (m_scoreboard)
          m_scoreboard->draw();
        al_flip_display();
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
        int current_living = how_many_are_alive();
        if (current_living <= 1)
          quit = true;
        if (current_living < num_alive)
        {
          //Add 5 points to those still alive
          for (int i = 0; i < m_num_snakes; i++)
          {
            if (m_snakes[i] && !m_snakes[i]->is_dead())
              m_player_scores[i] += 5;
          }
          num_alive = current_living;
        }
      }
      al_flip_display();
    }
    if (m_scoreboard)
    {
      for (int i = 0; i < 4; i++)
      {
        m_player_scores[i] = m_scoreboard->get_player_score(i);
      }
    }
    m_music->slow_to_stop();

    al_flush_event_queue(m_event);
    al_join_thread(input_thread, NULL);
    al_destroy_thread(input_thread);

    //Deallocate Memory
    for (int i = 0; i < 4; i++)
    {
      delete m_snakes[i];
      m_snakes[i] = NULL;
      delete m_ai[i];
      m_ai[i] = NULL;
    }
    delete m_scoreboard;
    m_scoreboard = NULL;
    delete m_pellet;
    m_pellet = NULL;
    delete m_collision_table;
    m_collision_table = NULL;
    delete m_music;
    m_music = NULL;

    //Show the Current Standing of Each Player
    if (rounds == m_rounds)
    {
      this->show_current_standing(hide_standing, true);
    }
    else
    {
      this->show_current_standing(hide_standing, false);
      this->draw_loading(rounds+1);
      al_flip_display();
    }
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

int Game::get_survivor()
{
  for (int i = 0; i < 4; i++)
  {
    if (m_snakes[i] && !m_snakes[i]->is_dead())
      return i;
  }
  return -1; //no survivors
}

ALLEGRO_COLOR Game::get_player_color(int player_num)
{
  ALLEGRO_COLOR color;
  switch (player_num)
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
  return color;
}

std::vector<int> *Game::get_player_rankings()
{
  assert(m_num_snakes > 1);
  std::vector<int> player_scores;
  std::vector<int> *player_rankings = new std::vector<int>();
  for (int i = 0; i < m_num_snakes; i++)
  {
    player_scores.push_back(m_player_scores[i]);
  }
  for (int i = 0; i < m_num_snakes; i++)
  {
    std::vector<int>::iterator iter = max_element(player_scores.begin(), player_scores.end());
    int distance = std::distance(player_scores.begin(), iter);
    assert(distance < 4);
    player_rankings->push_back(distance);
    player_scores.at(distance) = -1;
  }

  return player_rankings;
}

void Game::draw_loading()
{
  this->draw_loading(1);
}

void Game::draw_loading(int round)
{
  al_clear_to_color(al_color_name("black"));
  std::string text = "";
  if (m_rounds > 1)
    text = "Round " + std::to_string(round) + " of " + std::to_string(m_rounds);
  else //only show a loading screen when playing one round
    text = "Loading...";
  al_draw_text(m_font_large, al_color_name("yellow"), m_screen_width/2, m_screen_height/2, ALLEGRO_ALIGN_CENTER, text.c_str());
}

void Game::show_current_standing(bool hide_standing, bool game_over)
{
  al_flush_event_queue(m_event);

  if (!hide_standing && m_num_snakes > 1)
  {
    std::vector<int> *player_rankings = this->get_player_rankings();
    al_clear_to_color(al_color_name("black"));
    std::string text = "";
    ALLEGRO_COLOR color;
    if (!game_over)
    {
      text = "Current Standings";
      color = al_color_name("white");
    }
    else
    {
      text = "Player " + std::to_string(player_rankings->front()+1) + " Wins!";
      color = this->get_player_color(player_rankings->front());
    }
    float y = m_screen_height/50;
    al_draw_text(m_font_large, color, m_screen_width/2, y, ALLEGRO_ALIGN_CENTER, text.c_str());
    float menu_size = m_font_medium_incrementor * m_num_snakes;
    y = (m_screen_height/2) - (menu_size/2);
    
    for (int i = 0; i < m_num_snakes; i++)
    {
      switch (i)
      {
        case 0:
          text = "1st: Player " + std::to_string(player_rankings->at(i)+1) + " with ";
          break;
        case 1:
          text = "2nd: Player " + std::to_string(player_rankings->at(i)+1) + " with ";
          break;
        case 2:
          text = "3rd: Player " + std::to_string(player_rankings->at(i)+1) + " with ";
          break;
        case 3:
          text = "4th: Player " + std::to_string(player_rankings->at(i)+1) + " with ";
          break;
        default:
          assert(false);
      }
      text += std::to_string(m_player_scores[player_rankings->at(i)]);
      if (m_player_scores[player_rankings->at(i)] == 1)
        text += " point";
      else
        text += " points";
      color = this->get_player_color(player_rankings->at(i));
      al_draw_text(m_font_medium, color, m_screen_width/2, y, ALLEGRO_ALIGN_CENTER, text.c_str());
      y += m_font_medium_incrementor;
    }
    al_flip_display();
    delete player_rankings;

    //Wait for one of the users to press a key
    for (int i = 0; i < 10; i++)
    {
      ALLEGRO_EVENT e;
      al_wait_for_event_timed(m_event, &e, 1);
      if (e.type == ALLEGRO_EVENT_KEY_DOWN)
      {
        switch(m_controls->get_control(e.keyboard.keycode))
        {
          case Controls::PLAYER_1_CONFIRM:
          case Controls::PLAYER_2_CONFIRM:
          case Controls::PLAYER_3_CONFIRM:
          case Controls::PLAYER_4_CONFIRM:
          case Controls::PLAYER_1_CANCEL:
          case Controls::PLAYER_2_CANCEL:
          case Controls::PLAYER_3_CANCEL:
          case Controls::PLAYER_4_CANCEL:
            i = 10;
            break;
          default:
            i = 0;
            continue;
        }
      }
    }
  }

  //Show the Game Over Screen
  if (game_over)
  {
    al_clear_to_color(al_color_name("black"));
    al_draw_text(m_font_large, al_color_name("lawngreen"), m_screen_width/2, m_screen_height/2, ALLEGRO_ALIGN_CENTER, "Game Over");
    al_flip_display();
    al_rest(3);
  }
  al_flush_event_queue(m_event);
}

