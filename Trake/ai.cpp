#include "ai.h"
#include "snake.h"
#include "pellet.h"
#include <assert.h>

AI::AI(Snake *snakes[], int player_num, Pellet *pellet, Collision_Table *collision_table, bool tron)
{
  for (int i = 0; i < 4; i++)
  {
    m_snakes[i] = snakes[i];
  }
  m_pellet = pellet;
  m_collision_table = collision_table;
  m_player_num = player_num;
  m_tron = tron;
  assert(snakes[m_player_num]);
  m_max_x = snakes[m_player_num]->get_max_x();
  m_max_y = snakes[m_player_num]->get_max_y();
  m_width = snakes[m_player_num]->get_width();
  update_coordinates();
  update_direction();
}

AI::~AI()
{

}

void AI::read_input()
{
  update_coordinates();
  update_direction();
  if (!is_direction_safe(m_direction))
  {
    if (is_left_safe())
      this->change_direction(Input::LEFT);
    else if (is_right_safe())
      this->change_direction(Input::RIGHT);
    else if (is_down_safe())
      this->change_direction(Input::DOWN);
    else if (is_up_safe())
      this->change_direction(Input::UP);
  }
}

void AI::update_coordinates()
{
  m_x = m_snakes[m_player_num]->get_x();
  m_y = m_snakes[m_player_num]->get_y();
}

void AI::update_direction()
{
  m_direction = m_snakes[m_player_num]->get_direction();
}

void AI::change_direction(Input::Direction direction)
{
  m_snakes[m_player_num]->change_direction(direction);
}

bool AI::is_direction_safe(Input::Direction direction)
{
  float x = m_x;
  float y = m_y;
  switch (direction)
  {
    case Input::LEFT:
      x -= m_width;
      if (x < 0) x = m_max_x;
      break;
    case Input::RIGHT:
      x += m_width;
      if (x > m_max_x) x = 0;
      break;
    case Input::DOWN:
      y += m_width;
      if (y > m_max_y) y = 0;
      break;
    case Input::UP:
      y -= m_width;
      if (y < 0) y = m_max_y;
      break;
    default:
      assert(false);
  }
  Collision_Table::Type collision = m_collision_table->check_collision(x, y);
  if (collision == Collision_Table::NONE
   || collision == Collision_Table::PELLET)
  {
    return true;
  }
  return false;
}

bool AI::is_left_safe()
{
  return is_direction_safe(Input::LEFT);
}

bool AI::is_right_safe()
{
  return is_direction_safe(Input::RIGHT);
}

bool AI::is_down_safe()
{
  return is_direction_safe(Input::DOWN);
}

bool AI::is_up_safe()
{
  return is_direction_safe(Input::UP);
}

