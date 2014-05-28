#include "controls.h"
#include <allegro5/allegro.h>
#include <iterator>

Controls::Controls()
{
  m_controls = new std::unordered_map<int, int>();
}

Controls::~Controls()
{
  m_controls->clear();
  delete m_controls;
}

int Controls::get_control(int key)
{
  std::unordered_map<int, int>::iterator iter = m_controls->find(key);
  if (iter == m_controls->end())
    return Controls::NONE;
  else
    return (*iter).second;
}

void Controls::set_control(int key, int control)
{
  m_controls->insert(std::pair<int, int>(key, control));
}

