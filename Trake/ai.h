#ifndef AI_H
#define AI_H

#include "input.h"
#include "collision_table.h"
class Snake;
class Pellet;

class AI
{
  public:
    AI(Snake *snakes[], int player_num, Pellet *pellet, Collision_Table *collision_table, bool tron);
    ~AI();
    void read_input();
    void defensive();
  private:
    void update_coordinates();
    void update_direction();
    void turn_random_direction();
    void change_direction(Input::Direction direction);
    bool is_direction_safe(Input::Direction direction);
    bool is_left_safe();
    bool is_right_safe();
    bool is_down_safe();
    bool is_up_safe();
    Snake *m_snakes[4];
    int m_player_num;
    Pellet *m_pellet;
    Collision_Table *m_collision_table;
    float m_x;
    float m_y;
    float m_width;
    Input::Direction m_direction;
    float m_max_x;
    float m_max_y;
    bool m_tron;
};

#endif

