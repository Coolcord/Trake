#include "collision_table.h"
#include "snake.h"
#include "pellet.h"

Collision_Table::Collision_Table()
{
  m_table = new std::unordered_map<std::string, Collision_Table::Node*>();
}

Collision_Table::~Collision_Table()
{
  for (std::unordered_map<std::string, Collision_Table::Node*>::iterator iter = m_table->begin(); iter != m_table->end(); ++iter)
  {
    delete (*iter).second;
  }
  m_table->clear();
}

Collision_Table::Node::Node(Snake *snake)
{
  m_type = Collision_Table::SNAKE;
  m_collision_object.union_snake = snake;
}

Collision_Table::Node::Node(Pellet *pellet)
{
  m_type = Collision_Table::PELLET;
  m_collision_object.union_pellet = pellet;
}

std::unordered_map<std::string, Collision_Table::Node*>::iterator Collision_Table::begin()
{
  return m_table->begin();
}

std::unordered_map<std::string, Collision_Table::Node*>::iterator Collision_Table::end()
{
  return m_table->end();
}

void Collision_Table::insert(std::string key, Snake *snake)
{
  std::unordered_map<std::string, Collision_Table::Node*>::iterator iter = m_table->find(key);
  if (iter != m_table->end())
  {
    delete iter->second;
    m_table->erase(iter);
  }
  m_table->insert(std::pair<std::string, Collision_Table::Node*>(key, new Node(snake)));
}

void Collision_Table::insert(int x, int y, Snake *snake)
{
  return this->insert(std::to_string(x) + "x" + std::to_string(y), snake);
}

void Collision_Table::insert(std::string key, Pellet *pellet)
{
  std::unordered_map<std::string, Collision_Table::Node*>::iterator iter = m_table->find(key);
  if (iter != m_table->end())
  {
    delete iter->second;
    m_table->erase(iter);
  }
  m_table->insert(std::pair<std::string, Collision_Table::Node*>(key, new Node(pellet)));
}

void Collision_Table::insert(int x, int y, Pellet *pellet)
{
  return this->insert(std::to_string(x) + "x" + std::to_string(y), pellet);
}

void Collision_Table::insert(std::string key, Collision_Table::Node *node)
{
  std::unordered_map<std::string, Collision_Table::Node*>::iterator iter = m_table->find(key);
  if (iter != m_table->end())
  {
    delete iter->second;
    m_table->erase(iter);
  }
  m_table->insert(std::pair<std::string, Collision_Table::Node*>(key, node));
}

void Collision_Table::remove(std::string key)
{
  std::unordered_map<std::string, Collision_Table::Node*>::iterator iter = m_table->find(key);
  if (iter != m_table->end())
  {
    delete iter->second;
    m_table->erase(iter);
  }
}

void Collision_Table::remove(int x, int y)
{
  return this->remove(std::to_string(x) + "x" + std::to_string(y));
}

Collision_Table::Type Collision_Table::check_collision(std::string key)
{
  std::unordered_map<std::string, Collision_Table::Node*>::iterator iter = m_table->find(key);
  if (iter == m_table->end())
    return Collision_Table::NONE;
  else
    return (*iter).second->m_type;
}

Collision_Table::Type Collision_Table::check_collision(int x, int y)
{
  return this->check_collision(std::to_string(x) + "x" + std::to_string(y));
}

Snake *Collision_Table::get_snake(std::string key)
{
  std::unordered_map<std::string, Collision_Table::Node*>::iterator iter = m_table->find(key);
  if (iter == m_table->end())
    return NULL;
  else
    return (*iter).second->m_collision_object.union_snake;
}

Snake *Collision_Table::get_snake(int x, int y)
{
  return this->get_snake(std::to_string(x) + "x" + std::to_string(y));
}

Pellet *Collision_Table::get_pellet(std::string key)
{
  std::unordered_map<std::string, Collision_Table::Node*>::iterator iter = m_table->find(key);
  if (iter == m_table->end())
    return NULL;
  else
    return (*iter).second->m_collision_object.union_pellet;
}

Pellet *Collision_Table::get_pellet(int x, int y)
{
  return this->get_pellet(std::to_string(x) + "x" + std::to_string(y));
}

