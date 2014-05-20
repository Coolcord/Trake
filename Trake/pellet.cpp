#include <allegro5/allegro.h>
#include <allegro5/allegro_color.h>
#include "pellet.h"
#include "rectangle.h"
#include "collision_table.h"

Pellet::Pellet(float width, float max_x, float max_y, Collision_Table *collision_table, bool tron)
{
  assert(collision_table);
  m_width = width;
  m_max_x = max_x;
  m_max_y = max_y;
  m_tron = tron;
  m_spawn_countdown = 10;
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
  float x = m_max_x / 2;
  float y = m_max_y / 2;
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

void Pellet::eat()
{
  assert(m_exists);
  m_exists = false;
  m_spawn_countdown = 10;
}

