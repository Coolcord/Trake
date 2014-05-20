#ifndef PELLET_H
#define PELLET_H

class Rectangle;
class Collision_Table;

class Pellet
{
  public:
    Pellet(float width, float max_x, float max_y, Collision_Table *collision_table, bool tron);
    ~Pellet();
    void handle_state();
    void eat();
  private:
    float m_width;
    float m_max_x;
    float m_max_y;
    bool m_exists;
    bool m_tron;
    int m_spawn_countdown;
    void spawn();
    Rectangle *m_rectangle;
    Collision_Table *m_collision_table;
};

#endif

