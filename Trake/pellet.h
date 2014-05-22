#ifndef PELLET_H
#define PELLET_H

class Snake;
class Rectangle;
class Collision_Table;

class Pellet
{
  public:
    Pellet(float width, float max_x, float max_y, int spawn_countdown_max, Collision_Table *collision_table, bool tron);
    ~Pellet();
    void handle_state();
    void eat(int &grow);
    bool exists();
    float get_x();
    float get_y();
  private:
    float m_width;
    float m_max_x;
    float m_max_y;
    int m_value;
    bool m_exists;
    bool m_tron;
    int m_spawn_countdown_max;
    int m_spawn_countdown;
    int m_respawn_time_max;
    int m_respawn_time;
    void spawn();
    void remove();
    Rectangle *m_rectangle;
    Collision_Table *m_collision_table;
};

#endif

