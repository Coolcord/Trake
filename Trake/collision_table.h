#ifndef COLLISION_TABLE_H
#define COLLISION_TABLE_H

#include <string>
#include <unordered_map>

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
    Collision_Table();
    ~Collision_Table();
    void insert(std::string key, Snake *snake);
    void insert(float x, float y, Snake *snake);
    void insert(std::string key, Pellet *pellet);
    void insert(float x, float y, Pellet *pellet);
    void remove(std::string key);
    void remove(float x, float y);
    Type check_collision(std::string key);
    Type check_collision(float x, float y);
    Snake *get_snake(std::string key);
    Pellet *get_pellet(std::string key);
  private:
    class Node
    {
      public:
        Node(Snake *snake);
        Node(Pellet *pellet);
        ~Node() {}
        Type m_type;
        Collision_Object m_collision_object;
    };
    std::unordered_map<std::string, Node*> *m_table;
};

#endif

