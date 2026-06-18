#ifndef COLLISION_TABLE_H
#define COLLISION_TABLE_H

#include <string>
#include <unordered_map>
#include <iterator>

class Snake;
class Pellet;

class Collision_Table
{
  public:
    enum Type
    {
      NONE = 0,
      SNAKE = 1,
      PELLET = 2
    };
    union Collision_Object
    {
      Snake *union_snake;
      Pellet *union_pellet;
    };
    class Node
    {
      public:
        Node(Snake *snake);
        Node(Pellet *pellet);
        ~Node() {}
        Type m_type;
        Collision_Object m_collision_object;
    };
    Collision_Table();
    ~Collision_Table();
    std::unordered_map<std::string, Node*>::iterator begin();
    std::unordered_map<std::string, Node*>::iterator end();
    void insert(int x, int y, Snake *snake);
    void insert(int x, int y, Pellet *pellet);
    void insert(std::string key, Node *node);
    void remove(int x, int y);
    Type check_collision(int x, int y);
    Snake *get_snake(int x, int y);
    Pellet *get_pellet(int x, int y);
  private:
    std::unordered_map<std::string, Node*> *m_table;
    void insert(std::string key, Snake *snake);
    void insert(std::string key, Pellet *pellet);
    void remove(std::string key);
    Type check_collision(std::string key);
    Snake *get_snake(std::string key);
    Pellet *get_pellet(std::string key);
};

#endif

