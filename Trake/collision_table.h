#ifndef COLLISION_TABLE_H
#define COLLISION_TABLE_H

#include <string.h>
#include <unordered_map>

class Snake;

class Collision_Table
{
  public:
    enum Type
    {
      NONE = 0,
      SNAKE = 1
    };
    Collision_Table();
    ~Collision_Table();
    void insert(std::string key, Snake *snake);
    void insert(float x, float y, Snake *snake);
    void remove(std::string key);
    void remove(float x, float y);
    Type check_collision(std::string key);
    Type check_collision(float x, float y);
    Snake *get_snake(std::string key);
  private:
    class Node
    {
      public:
        Node(Snake *snake);
        ~Node() {}
        Type m_type;
        Snake *m_snake;
    };
    std::unordered_map<std::string, Node*> *m_table;
};

#endif

