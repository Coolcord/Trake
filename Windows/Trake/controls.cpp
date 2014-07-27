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

int Controls::get_key(int control)
{
  for (std::unordered_map<int, int>::iterator iter = m_controls->begin(); iter != m_controls->end(); ++iter)
  {
    if ((*iter).second == control)
      return (*iter).first;
  }
  return -1; //nothing found
}

void Controls::set_control(int key, int control)
{
  if (this->get_control(key) != Controls::NONE)
    m_controls->erase(key);
  int prev_key = this->get_key(control);
  if (prev_key != -1)
    m_controls->erase(prev_key);
  m_controls->insert(std::pair<int, int>(key, control));
}

std::unordered_map<int, int> *Controls::get_map()
{
  return m_controls;
}

