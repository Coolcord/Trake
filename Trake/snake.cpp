#include "snake.h"
#include "snake_piece.h"
#include "rectangle.h"
#include <allegro5/allegro_color.h>
#include <assert.h>
#include <iterator>
#include "collision_table.h"

Snake::Snake(float x, float y, Input::Direction direction, int size, ALLEGRO_COLOR color, float width, float max_x, float max_y, bool wrap, Collision_Table *collision_table, bool tron)
{
  assert(max_x > 0);
  assert(max_y > 0);
  assert(size >= 3);
  assert(collision_table);
  m_width = width;
  m_size = size;
  m_color = color;
  m_pieces = new std::vector<Snake_Piece*>();
  m_grow = 0;
  m_max_x = max_x;
  m_max_y = max_y;
  m_wrap = wrap;
  m_collision_table = collision_table;
  m_dead = false;
  m_changing_direction = false;
  m_next_direction = Input::NONE;
  m_tron = tron;

  //Head
  m_pieces->push_back(new Snake_Piece(direction, Snake_Piece::HEAD, new Rectangle(x, y, m_width, m_width, true, m_color)));
  m_collision_table->insert(x, y, this);
  for (int i = 1; i < m_size - 1; i++)
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
  if (m_collision_table->check_collision(collision_key) == Collision_Table::SNAKE)
  {
    m_dead = true;
    m_pieces->front()->set_x(prev_x);
    m_pieces->front()->set_y(prev_y);
    this->draw();
    return;
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
    if (std::next(iter) != m_pieces->end())
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
          --m_grow;
        (*iter)->set_type(Snake_Piece::BODY);
        Rectangle *rectangle = this->create_rectangle(tmp_direction, tmp_x, tmp_y, m_size-1);
        Snake_Piece *snake_piece = new Snake_Piece(tmp_direction, Snake_Piece::TAIL, rectangle);
        ++m_size;
        m_pieces->push_back(snake_piece);
        break; //don't continue the for loop
      }
      else
      {
        al_draw_filled_rectangle(tmp_x, tmp_y, tmp_x + m_width, tmp_y + m_width, al_color_name("black")); //remove the square
        m_collision_table->remove(tmp_x, tmp_y); //remove from the collision table
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

void Snake::grow(int size)
{
  m_grow += size;
}

