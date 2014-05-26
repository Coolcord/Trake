#include "snake.h"
#include "snake_piece.h"
#include "pellet.h"
#include "rectangle.h"
#include <allegro5/allegro_color.h>
#include <assert.h>
#include <iterator>
#include <string>
#include "scoreboard.h"
#include "collision_table.h"

Snake::Snake(int player_num, float x, float y, float volume, Input::Direction direction, int size, ALLEGRO_COLOR color, float width, float max_x, float max_y, bool wrap, Collision_Table *collision_table, bool tron)
{
  assert(max_x > 0);
  assert(max_y > 0);
  assert(size >= 3);
  assert(collision_table);
  m_player_num = player_num;
  m_width = width;
  if (tron)
    m_size = 3;
  else
    m_size = size;
  m_color = color;
  m_pieces = new std::vector<Snake_Piece*>();
  m_grow = 0;
  m_max_x = max_x;
  m_max_y = max_y;
  m_volume = volume * 0.1;
  m_wrap = wrap;
  m_scoreboard = NULL; //set it to NULL for now
  m_collision_table = collision_table;
  m_dead = false;
  m_changing_direction = false;
  m_next_direction = Input::NONE;
  m_tron = tron;
  int random = (rand() % 3) + 1;
  m_dead_sound = al_load_sample(("./sounds/squish" + std::to_string(random) + ".wav").c_str());
  m_eat_sound_speed = 0.7 + ((float)player_num * 0.2);

  //Head
  m_pieces->push_back(new Snake_Piece(direction, Snake_Piece::HEAD, new Rectangle(x, y, m_width, m_width, true, m_color)));
  m_collision_table->insert(x, y, this);
  for (int i = 1; i < m_size; i++)
  { //Body
    Rectangle *rectangle = this->create_rectangle(direction, x, y, i);
    Snake_Piece *snake_piece = NULL;
    if (i == m_size - 1)
    { //Tail
      snake_piece = new Snake_Piece(direction, Snake_Piece::TAIL, rectangle);
    }
    else
    { //Body
      snake_piece = new Snake_Piece(direction, Snake_Piece::BODY, rectangle);
    }
    assert(snake_piece);
    m_pieces->push_back(snake_piece);
  }
  this->draw();
}

Snake::~Snake()
{
  for (std::vector<Snake_Piece*>::iterator iter = m_pieces->begin(); iter != m_pieces->end(); ++iter)
  {
    delete (*iter);
  }
  m_pieces->clear();
  al_destroy_sample(m_dead_sound);
}

void Snake::move()
{
  if (m_dead)
    return;
  if (m_changing_direction)
    m_changing_direction = false;

  assert(m_pieces);
  assert(m_pieces->size() >= 3);
  assert(m_pieces->front());
  assert(m_pieces->front()->get_type() == Snake_Piece::HEAD);
  
  float prev_x = m_pieces->front()->get_x();
  float prev_y = m_pieces->front()->get_y();
  Input::Direction prev_direction = m_pieces->front()->get_direction();
  //Move the head piece
  switch(m_pieces->front()->get_direction())
  {
    case Input::LEFT:
      if (m_wrap && prev_x <= 0)
        m_pieces->front()->set_x(m_max_x);
      else
        m_pieces->front()->set_x(m_pieces->front()->get_x() - m_width);
      break;
    case Input::RIGHT:
      if (m_wrap && prev_x >= m_max_x)
        m_pieces->front()->set_x(0);
      else
        m_pieces->front()->set_x(m_pieces->front()->get_x() + m_width);
      break;
    case Input::DOWN:
      if (m_wrap && prev_y >= m_max_y)
        m_pieces->front()->set_y(0);
      else
        m_pieces->front()->set_y(m_pieces->front()->get_y() + m_width);
      break;
    case Input::UP:
      if (m_wrap && prev_y <= 0)
        m_pieces->front()->set_y(m_max_y);
      else
        m_pieces->front()->set_y(m_pieces->front()->get_y() - m_width);
      break;
    default:
      assert(false);
  }
  float front_x = m_pieces->front()->get_x();
  float front_y = m_pieces->front()->get_y();
  std::string collision_key = std::to_string(front_x) + std::to_string(front_y);
  Collision_Table::Type collision = m_collision_table->check_collision(collision_key);
  if (collision == Collision_Table::SNAKE)
  {
    m_dead = true;
    assert(m_scoreboard);
    m_scoreboard->draw();
    float speed = static_cast <float> (rand() / static_cast <float> (RAND_MAX/0.8)) + 0.6;
    if (m_dead_sound) al_play_sample(m_dead_sound, 2.5*m_volume, 0.0, speed, ALLEGRO_PLAYMODE_ONCE, NULL);
    m_pieces->front()->set_x(prev_x);
    m_pieces->front()->set_y(prev_y);
    this->draw();
    return;
  }
  else if (collision == Collision_Table::PELLET)
  {
    m_collision_table->get_pellet(collision_key)->eat(this);
  }
  m_pieces->front()->draw();
  m_collision_table->insert(collision_key, this);
  //Move the body
  for (std::vector<Snake_Piece*>::iterator iter = std::next(m_pieces->begin()); iter != m_pieces->end(); ++iter)
  {
    assert(*iter);
    float tmp_x = (*iter)->get_x();
    float tmp_y = (*iter)->get_y();
    Input::Direction tmp_direction = (*iter)->get_direction();
    (*iter)->set_x(prev_x);
    (*iter)->set_y(prev_y);
    (*iter)->set_direction(prev_direction);
    if ((*iter) != m_pieces->back())
    {
      prev_x = tmp_x;
      prev_y = tmp_y;
      prev_direction = tmp_direction;
    }
    else //tail
    {
      if (m_tron || m_grow > 0)
      {
        if (m_grow > 0)
        {
          --m_grow;
        }
        (*iter)->set_type(Snake_Piece::BODY);
        Rectangle *rectangle = this->create_rectangle(tmp_direction, prev_x, prev_y, m_size-1);
        Snake_Piece *snake_piece = new Snake_Piece(tmp_direction, Snake_Piece::TAIL, rectangle);
        assert(snake_piece);
        ++m_size;
        m_pieces->push_back(snake_piece);
        if (!m_tron)
        {
          Collision_Table::Type type = m_collision_table->check_collision(tmp_x, tmp_y);
          if (type == Collision_Table::SNAKE 
            && m_collision_table->get_snake(tmp_x, tmp_y) == this
            && tmp_x <= m_max_x && tmp_y <= m_max_y)
          {
            al_draw_filled_rectangle(tmp_x, tmp_y, tmp_x + m_width, tmp_y + m_width, al_color_name("black")); //remove the square
            m_collision_table->remove(tmp_x, tmp_y); //remove from the collision table
          }
        }
        break;
      }
      else
      {
        Collision_Table::Type type = m_collision_table->check_collision(tmp_x, tmp_y);
        if (type == Collision_Table::SNAKE
          && m_collision_table->get_snake(tmp_x, tmp_y) == this
          && tmp_x <= m_max_x && tmp_y <= m_max_y)
        {
          al_draw_filled_rectangle(tmp_x, tmp_y, tmp_x + m_width, tmp_y + m_width, al_color_name("black")); //remove the square
          m_collision_table->remove(tmp_x, tmp_y); //remove from the collision table
        }
        break;
      }
    }
  }
  if (m_next_direction != Input::NONE)
  {
    if (!m_changing_direction)
    {
      Input::Direction tmp_direction = m_next_direction;
      m_next_direction = Input::NONE;
      change_direction(tmp_direction);
    }
  }
}

Rectangle *Snake::create_rectangle(Input::Direction direction, float x, float y, int position)
{
  Rectangle *rectangle = NULL;
  switch(direction)
  {
    case Input::LEFT:
      rectangle = new Rectangle(x+(position*m_width), y, m_width, m_width, true, m_color);
      m_collision_table->insert(x+(position*m_width), y, this);
      break;
    case Input::RIGHT:
      rectangle = new Rectangle(x-(position*m_width), y, m_width, m_width, true, m_color);
      m_collision_table->insert(x-(position*m_width), y, this);
      break;
    case Input::DOWN:
      rectangle = new Rectangle(x, y-(position*m_width), m_width, m_width, true, m_color);
      m_collision_table->insert(x, y-(position*m_width), this);
      break;
    case Input::UP:
      rectangle = new Rectangle(x, y+(position*m_width), m_width, m_width, true, m_color);
      m_collision_table->insert(x, y+(position*m_width), this);
      break;
    default:
      assert(false);
  }
  assert(rectangle);
  return rectangle;
}

void Snake::draw()
{
  //Draw the snake on screen
  if (!m_dead)
  {
    for (std::vector<Snake_Piece*>::iterator iter = m_pieces->begin(); iter != m_pieces->end(); ++iter)
    {
      (*iter)->draw();
    }
  }
  else //display differently on death
  {
    for (std::vector<Snake_Piece*>::iterator iter = m_pieces->begin(); iter != m_pieces->end(); ++iter)
    {
      ALLEGRO_COLOR tmp_color = (*iter)->get_color();
      (*iter)->set_color(al_color_name("black"));
      (*iter)->draw();
      (*iter)->set_filled(false);
      (*iter)->set_color(tmp_color);
      (*iter)->draw();
    }
  }
}

void Snake::change_direction(Input::Direction direction)
{
  assert(m_pieces);
  assert(m_pieces->size() >= 3);
  assert(m_pieces->front());
  assert(m_pieces->front()->get_type() == Snake_Piece::HEAD);
  if (m_next_direction != Input::NONE)
    return; //ignore past 2 direction changes
  else if (m_changing_direction)
  {
    m_next_direction = direction;
    return;
  }
  switch (m_pieces->front()->get_direction())
  {
    case Input::LEFT:
      if (direction == Input::LEFT || direction == Input::RIGHT)
        return;
      break;
    case Input::RIGHT:
      if (direction == Input::RIGHT || direction == Input::LEFT)
        return;
      break;
    case Input::DOWN:
      if (direction == Input::DOWN || direction == Input::UP)
        return;
      break;
    case Input::UP:
      if (direction == Input::UP || direction == Input::DOWN)
        return;
      break;
    default:
      assert(false);
  }
  m_changing_direction = true;
  m_pieces->front()->set_direction(direction);
}

float Snake::get_x()
{
  return m_pieces->front()->get_x();
}

float Snake::get_y()
{
  return m_pieces->front()->get_y();
}

float Snake::get_width()
{
  return m_width;
}

Input::Direction Snake::get_direction()
{
  return m_pieces->front()->get_direction();
}

float Snake::get_max_x()
{
  return m_max_x;
}

float Snake::get_max_y()
{
  return m_max_y;
}

void Snake::grow(int value)
{
  m_grow += value;
}

bool Snake::is_dead()
{
  return m_dead;
}

void Snake::kill()
{
  m_dead = true;
  assert(m_scoreboard);
  m_scoreboard->draw();
}

float Snake::get_eat_sound_speed()
{
  return m_eat_sound_speed;
}

int Snake::get_player_num()
{
  return m_player_num;
}

void Snake::set_scoreboard(Scoreboard *scoreboard)
{
  m_scoreboard = scoreboard;
}

