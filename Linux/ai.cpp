#include "ai.h"
#include "snake.h"
#include "pellet.h"
#include "collision_table.h"
#include <assert.h>
#include <string>
#include <unordered_map>
#include <iterator>
#include <algorithm>

int SNAKE_TURN_CHANCE = 25;
int TRON_TURN_CHANCE = 40;
int MAX_LOOK_AHEAD = 100;
int MAX_OFFENSIVE_MOVES = 5;
int MAX_NOTICE_PELLET = 0;
int CHECK_FAIL = 0;

//EASY
const int EASY_MAX_LOOK_AHEAD = 10;
const int EASY_MAX_OFFENSIVE_MOVES = 10;
const int EASY_MAX_NOTICE_PELLET = 30;
const int EASY_CHECK_FAIL = 20;

//MEDIUM
const int MEDIUM_MAX_LOOK_AHEAD = 50;
const int MEDIUM_MAX_OFFENSIVE_MOVES = 5;
const int MEDIUM_MAX_NOTICE_PELLET = 15;
const int MEDIUM_CHECK_FAIL_SNAKE = 250;
const int MEDIUM_CHECK_FAIL_TRON = 500;

//HARD
const int HARD_MAX_LOOK_AHEAD = 100;
const int HARD_MAX_OFFENSIVE_MOVES = 3;
const int HARD_MAX_NOTICE_PELLET = 0;


AI::AI(Snake *snakes[], int player_num, Pellet *pellet, Collision_Table *collision_table, int difficulty, bool tron)
{
  for (int i = 0; i < 4; i++)
  {
    m_snakes[i] = snakes[i];
  }
  assert(pellet);
  m_pellet = pellet;
  m_collision_table = collision_table;
  m_player_num = player_num;
  if (difficulty == 0)
  { //EASY
    MAX_LOOK_AHEAD = EASY_MAX_LOOK_AHEAD;
    MAX_OFFENSIVE_MOVES = EASY_MAX_OFFENSIVE_MOVES;
    MAX_NOTICE_PELLET = EASY_MAX_NOTICE_PELLET;
    CHECK_FAIL = EASY_CHECK_FAIL;
  }
  else if (difficulty == 1)
  { //MEDIUM
    MAX_LOOK_AHEAD = MEDIUM_MAX_LOOK_AHEAD;
    MAX_OFFENSIVE_MOVES = MEDIUM_MAX_OFFENSIVE_MOVES;
    MAX_NOTICE_PELLET = MEDIUM_MAX_NOTICE_PELLET;
    if (tron)
      CHECK_FAIL = MEDIUM_CHECK_FAIL_TRON;
    else
      CHECK_FAIL = MEDIUM_CHECK_FAIL_SNAKE;
  }
  else
  { //HARD
    MAX_LOOK_AHEAD = HARD_MAX_LOOK_AHEAD;
    MAX_OFFENSIVE_MOVES = HARD_MAX_OFFENSIVE_MOVES;
    MAX_NOTICE_PELLET = HARD_MAX_NOTICE_PELLET;
  }
  m_ai_difficulty = difficulty;
  m_notice_pellet = MAX_NOTICE_PELLET;
  m_tron = tron;
  assert(snakes[m_player_num]);
  m_max_x = snakes[m_player_num]->get_max_x();
  m_max_y = snakes[m_player_num]->get_max_y();
  m_width = snakes[m_player_num]->get_width();
  if (m_tron)
    m_offensive_count = rand() % MAX_OFFENSIVE_MOVES + (MAX_OFFENSIVE_MOVES/4) + 1;
  else
    m_offensive_count = 0;
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
  if (try_move_to_pellet())
    return;
  if (!m_tron)
  {
    if (rand() % SNAKE_TURN_CHANCE == 0)
    {
      if (ok_to_turn())
      {
        go_safest_turn_direction();
        return;
      }
    }
  }
  else if (m_offensive_count > 0)
  {
    offensive();
    return;
  }
  defensive();
}

bool AI::try_move_to_pellet()
{
  if (!m_pellet->exists())
  {
    m_notice_pellet = MAX_NOTICE_PELLET; 
    return false;
  }
  else if (m_notice_pellet > 0)
  {
    m_notice_pellet--;
    return false;
  }
  return move_to_pellet();
}

bool AI::move_to_pellet()
{
  int random = rand() % 2;
  bool need_x = false;
  bool need_y = false;
  if (m_x != m_pellet->get_x()) need_x = true;
  if (m_y != m_pellet->get_y()) need_y = true;
  if (random == 0)
  { //Try x first
    if (need_x && move_to_pellet_x(true))
      return true;
    else if (need_y && move_to_pellet_y(true))
      return true;
    else
      return false;
  }
  else
  { //Try y first
    if (need_y && move_to_pellet_y(true))
      return true;
    else if (need_x && move_to_pellet_x(true))
      return true;
    else
      return false;
  }
}

bool AI::move_to_pellet_x(bool try_again)
{
  if (m_x < m_pellet->get_x()) 
  {
    if (try_go_right())
      return true;
    else if (!try_again)
      return move_to_pellet_y(false);
    else
      return false;
  }
  else 
  {
    if (try_go_left())
      return true;
    else if (!try_again)
      return move_to_pellet_y(false);
    else
      return false;
  }
}

bool AI::move_to_pellet_y(bool try_again)
{
  if (m_y < m_pellet->get_y())
  { 
    if (try_go_down())
      return true;
    else if (!try_again)
      return move_to_pellet_y(false);
    else
      return false;
  } 
  else 
  { 
    if (try_go_up())
      return true;
    else if (!try_again)
      return move_to_pellet_y(false);
    else
      return false;
  }
}

void AI::defensive()
{
  if (!is_direction_safe_later(m_direction))
  {
    go_safest_direction();
  }
}

void AI::offensive()
{
  assert(m_offensive_count > 0);
  int random = rand() % TRON_TURN_CHANCE;
  if (random == 0)
  {
    if (ok_to_turn())
    {
      --m_offensive_count;
      go_safest_turn_direction();
      return;
    }
  }
  defensive();
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

void AI::go_safest_direction()
{
  change_direction(find_safest_direction());
}

void AI::go_safest_turn_direction()
{
  change_direction(find_safest_turn_direction());
}

bool AI::ok_to_turn()
{
  switch (m_direction)
  {
    case Input::LEFT:
    case Input::RIGHT:
      if (is_up_safe_later() || is_down_safe_later())
        return true;
      else
        return false;
    case Input::DOWN:
    case Input::UP:
      if (is_left_safe_later() || is_right_safe_later())
        return true;
      else
        return false;
    default:
        assert(false);
  }
}

Input::Direction AI::get_random_turn_direction()
{
  int random = rand() % 2;
  Input::Direction direction = m_direction;
  switch (m_direction)
  {
    case Input::LEFT: //Turn up or down
    case Input::RIGHT:
      if (random == 0) direction = Input::DOWN;
      else direction = Input::UP;
      break;
    case Input::DOWN: //Turn left or right
    case Input::UP:
      if (random == 0) direction = Input::LEFT;
      else direction = Input::RIGHT;
      break;
    default:
      assert(false);
  }
  if (!is_direction_safe(direction))
  {
    switch (direction)
    {
      case Input::LEFT:
        direction = Input::RIGHT;
        break;
      case Input::RIGHT:
        direction = Input::LEFT;
        break;
      case Input::DOWN:
        direction = Input::UP;
        break;
      case Input::UP:
        direction = Input::DOWN;
        break;
      default:
        assert(false);
    }
  }
  return direction;
}

void AI::turn_random_direction()
{
  this->change_direction(get_random_turn_direction());
}

void AI::change_direction(Input::Direction direction)
{
  m_snakes[m_player_num]->change_direction(direction);
}

int AI::how_long_is_direction_safe(Input::Direction direction)
{
  if (!is_direction_safe(direction))
    return 0; //don't bother running the simulation

  //Save "reality"
  float tmp_x = m_x;
  float tmp_y = m_y;
  Input::Direction tmp_direction = m_direction;
  
  //Prepare to run simulation
  int safe_moves = 0;
  bool alive = true;
  m_direction = direction;
  Collision_Table *simulated_table = new Collision_Table();
  
  for (std::unordered_map<std::string, Collision_Table::Node*>::iterator iter = m_collision_table->begin(); iter != m_collision_table->end(); ++iter)
  {
    Collision_Table::Node *node = NULL;
    switch ((*iter).second->m_type)
    {
      case Collision_Table::SNAKE:
        node = new Collision_Table::Node((*iter).second->m_collision_object.union_snake);
        simulated_table->insert((*iter).first, node);
        break;
      case Collision_Table::PELLET:
        node = new Collision_Table::Node((*iter).second->m_collision_object.union_pellet);
        simulated_table->insert((*iter).first, node);
        break;
      case Collision_Table::NONE:
        break;
      default:
        assert(false);
    }
  }

  //Run simulation
  alive = simulate_direction(m_direction, simulated_table);
  ++safe_moves;
  while (safe_moves < MAX_LOOK_AHEAD)
  {
    if (!is_direction_safe(m_direction))
    {
      m_direction = find_safe_direction();
    }
    alive = simulate_direction(m_direction, simulated_table);
    if (!alive) break;
    else ++safe_moves;
  }
  
  //Restore "reality"
  m_x = tmp_x;
  m_y = tmp_y;
  m_direction = tmp_direction;
  delete simulated_table;
  
  return safe_moves;
}

bool AI::is_direction_safe_later(Input::Direction direction)
{
  int safe_moves = how_long_is_direction_safe(direction);
  if (safe_moves == MAX_LOOK_AHEAD)
    return true;
  else
    return false;
}

bool AI::is_left_safe_later()
{
  return is_direction_safe_later(Input::LEFT);
}

bool AI::is_right_safe_later()
{
  return is_direction_safe_later(Input::RIGHT);
}

bool AI::is_down_safe_later()
{
  return is_direction_safe_later(Input::DOWN);
}

bool AI::is_up_safe_later()
{
  return is_direction_safe_later(Input::UP);
}

bool AI::is_direction_safe(Input::Direction direction)
{
  if (m_ai_difficulty < 2 && (rand() % CHECK_FAIL) == 0)
  {
    return true;
  }
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

bool AI::try_go_left()
{
  if (is_left_safe_later())
    return go_left();
  else
    return false;
}

bool AI::try_go_right()
{
  if (is_right_safe_later())
    return go_right();
  else
    return false;
}

bool AI::try_go_down()
{
  if (is_down_safe_later())
    return go_down();
  else
    return false;
}

bool AI::try_go_up()
{
  if (is_up_safe_later())
    return go_up();
  else
    return false;
}

bool AI::go_left()
{
  switch (m_direction)
  {
    case Input::LEFT:
      break; //nothing to change
    case Input::RIGHT: //facing the wrong way!
      return false;
    case Input::DOWN:
    case Input::UP:
      change_direction(Input::LEFT);
      break;
    default:
      assert(false);
  }
  return true;
}

bool AI::go_right()
{
  switch (m_direction)
  {
    case Input::RIGHT:
      break; //nothing to change
    case Input::LEFT: //facing the wrong way!
      return false;
    case Input::DOWN:
    case Input::UP:
      change_direction(Input::RIGHT);
      break;
    default:
      assert(false);
  }
  return true;
}

bool AI::go_down()
{
  switch (m_direction)
  {
    case Input::DOWN:
      break; //nothing to change
    case Input::UP: //facing the wrong way!
      return false;
    case Input::LEFT:
    case Input::RIGHT:
      change_direction(Input::DOWN);
      break;
    default:
      assert(false);
  }
  return true;
}

bool AI::go_up()
{
  switch (m_direction)
  {
    case Input::UP:
      break; //nothing to change
    case Input::DOWN: //facing the wrong way!
      return false;
    case Input::LEFT:
    case Input::RIGHT:
      change_direction(Input::UP);
      break;
    default:
      assert(false);
  }
  return true;
}

bool AI::simulate_left(Collision_Table *simulated_table)
{
  if (!is_left_safe())
  {
    return false;
  }
  if (m_x <= 0)
  {
    m_x = m_max_x;
  }
  else
  {
    m_x -= m_width;
  }
  simulated_table->insert(m_x, m_y, m_snakes[m_player_num]);
  return true;
}

bool AI::simulate_right(Collision_Table *simulated_table)
{
  if (!is_right_safe())
  {
    return false;
  }
  if (m_x >= m_max_x)
  {
    m_x = 0;
  }
  else
  {
    m_x += m_width;
  }
  simulated_table->insert(m_x, m_y, m_snakes[m_player_num]);
  return true;
}

bool AI::simulate_down(Collision_Table *simulated_table)
{
  if (!is_down_safe())
  {
    return false;
  }
  if (m_y >= m_max_y)
  {
    m_y = 0;
  }
  else
  {
    m_y += m_width;
  }
  simulated_table->insert(m_x, m_y, m_snakes[m_player_num]);
  return true;
}

bool AI::simulate_up(Collision_Table *simulated_table)
{
  if (!is_up_safe())
  {
    return false;
  }
  if (m_y <= 0)
  {
    m_y = m_max_y;
  }
  else
  {
    m_y -= m_width;
  }
  simulated_table->insert(m_x, m_y, m_snakes[m_player_num]);
  return true;
}

bool AI::simulate_direction(Input::Direction direction, Collision_Table *simulated_table)
{
  switch (direction)
  {
    case Input::LEFT:
      return simulate_left(simulated_table);
      break;
    case Input::RIGHT:
      return simulate_right(simulated_table);
      break;
    case Input::DOWN:
      return simulate_down(simulated_table);
      break;
    case Input::UP:
      return simulate_up(simulated_table);
      break;
    default:
      assert(false);
  }
  return false;
}

Input::Direction AI::find_safe_direction()
{
  switch (m_direction)
  {
    case Input::LEFT:
      if (is_down_safe()) return Input::DOWN;
      else if (is_up_safe()) return Input::UP;
      else return Input::LEFT;
    case Input::RIGHT:
      if (is_down_safe()) return Input::DOWN;
      else if (is_up_safe()) return Input::UP;
      else return Input::RIGHT;
    case Input::DOWN:
      if (is_left_safe()) return Input::LEFT;
      else if (is_right_safe()) return Input::RIGHT;
      else return Input::DOWN;
    case Input::UP:
      if (is_left_safe()) return Input::LEFT;
      else if (is_right_safe()) return Input::RIGHT;
      else return Input::UP;
    default:
      assert(false);
  }
}

Input::Direction AI::find_safest_direction()
{
  int left_moves = how_long_is_direction_safe(Input::LEFT);
  int right_moves = how_long_is_direction_safe(Input::RIGHT);
  int down_moves = how_long_is_direction_safe(Input::DOWN);
  int up_moves = how_long_is_direction_safe(Input::UP);
  int max = 0;
  switch (m_direction)
  {
    case Input::LEFT:
      max = std::max(left_moves, std::max(down_moves, up_moves));
      if (max == left_moves)
        return Input::LEFT;
      else if (down_moves == up_moves)
        return get_random_turn_direction();
      else if (max == down_moves)
        return Input::DOWN;
      else if (max == up_moves)
        return Input::UP;
      else
        assert(false);
      break;
    case Input::RIGHT:
      max = std::max(right_moves, std::max(down_moves, up_moves));
      if (max == right_moves)
        return Input::RIGHT;
      else if (down_moves == up_moves)
        return get_random_turn_direction();
      else if (max == down_moves)
        return Input::DOWN;
      else if (max == up_moves)
        return Input::UP;
      else
        assert(false);
      break;
    case Input::DOWN:
      max = std::max(down_moves, std::max(left_moves, right_moves));
      if (max == down_moves)
        return Input::DOWN;
      else if (left_moves == right_moves)
        return get_random_turn_direction();
      else if (max == left_moves)
        return Input::LEFT;
      else if (max == right_moves)
        return Input::RIGHT;
      else
        assert(false);
      break;
    case Input::UP:
      max = std::max(up_moves, std::max(left_moves, right_moves));
      if (max == up_moves)
        return Input::UP;
      else if (left_moves == right_moves)
        return get_random_turn_direction();
      else if (max == left_moves)
        return Input::LEFT;
      else if (max == right_moves)
        return Input::RIGHT;
      else
        assert(false);
      break;
    default:
      assert(false);
  }
}

Input::Direction AI::find_safest_turn_direction()
{
  int left_moves = 0;
  int right_moves = 0;
  int down_moves = 0;
  int up_moves = 0;
  switch (m_direction)
  {
    case Input::LEFT:
    case Input::RIGHT:
      down_moves = how_long_is_direction_safe(Input::DOWN);
      up_moves = how_long_is_direction_safe(Input::UP);
      if (down_moves == up_moves)
        return get_random_turn_direction();
      else if (down_moves > up_moves)
        return Input::DOWN;
      else if (up_moves > down_moves)
        return Input::UP;
      else
        assert(false);
      break;
    case Input::DOWN:
    case Input::UP:
      left_moves = how_long_is_direction_safe(Input::LEFT);
      right_moves = how_long_is_direction_safe(Input::RIGHT);
      if (left_moves == right_moves)
        return get_random_turn_direction();
      else if (left_moves > right_moves)
        return Input::LEFT;
      else if (right_moves > left_moves)
        return Input::RIGHT;
      else
        assert(false);
      break;
    default:
      assert(false);
  }
}

