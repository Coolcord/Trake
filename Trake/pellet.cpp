#include <allegro5/allegro.h>
#include <allegro5/allegro_color.h>
#include <cstdlib>
#include "pellet.h"
#include "snake.h"
#include "rectangle.h"
#include "collision_table.h"

Pellet::Pellet(float width, float max_x, float max_y, int spawn_countdown_max, Collision_Table *collision_table, bool tron)
{
  assert(collision_table);
  m_width = width;
  m_max_x = max_x;
  m_max_y = max_y;
  m_tron = tron;
  m_value = 0;
  m_spawn_countdown_max = spawn_countdown_max + 1;
  m_spawn_countdown = rand() % (m_spawn_countdown_max / 2);
  m_collision_table = collision_table;
  m_exists = false;
  m_rectangle = new Rectangle(0, 0, m_width, m_width, true, al_color_name("white"));
}

Pellet::~Pellet()
{
  delete m_rectangle;
}

void Pellet::handle_state()
{
  if (m_tron)
    return;
  if (m_exists)
    return;
  if (m_spawn_countdown > 0)
  {
    --m_spawn_countdown;
    return;
  }
  spawn();
}

void Pellet::spawn()
{
  assert(!m_exists);
  m_value = 3;
  float test_x = rand() % ((int)m_max_x + 1);
  float x = 0;
  while (x < test_x)
  {
    x += m_width;
  }
  float test_y = rand() % ((int)m_max_y + 1);
  float y = 0;
  while (y < test_y)
  {
    y += m_width;
  }
  Collision_Table::Type collision = m_collision_table->check_collision(x, y);
  if (collision == Collision_Table::NONE)
  {
    m_exists = true;
    m_rectangle->set_x(x);
    m_rectangle->set_y(y);
    m_rectangle->draw();
    m_collision_table->insert(x, y, this);
    m_rectangle->draw();
  }
}

void Pellet::eat(int &grow)
{
  assert(m_exists);
  grow += m_value;
  m_exists = false;
  m_collision_table->remove(m_rectangle->get_x(), m_rectangle->get_y());
  m_spawn_countdown = rand() % m_spawn_countdown_max;
}

bool Pellet::exists()
{
  return m_exists;
}

float Pellet::get_x()
{
  return m_rectangle->get_x();
}

float Pellet::get_y()
{
  return m_rectangle->get_y();
}

