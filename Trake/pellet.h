#ifndef PELLET_H
#define PELLET_H

class Rectangle;
class Collision_Table;

class Pellet
{
  public:
    Pellet();
    ~Pellet();
  private:
    Rectangle *m_rectangle;
    Collision_Table *m_collision_table;
};

#endif

