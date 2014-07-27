#ifndef AI_H
#define AI_H

#include "input.h"
class Collision_Table;
class Snake;
class Pellet;

class AI
{
  public:
    AI(Snake *snakes[], int player_num, Pellet *pellet, Collision_Table *collision_table, bool tron);
    ~AI();
    void read_input();
  private:
    bool try_move_to_pellet();
    bool move_to_pellet();
    bool move_to_pellet_x(bool try_again);
    bool move_to_pellet_y(bool try_again);
    void defensive();
    void offensive();
    void update_coordinates();
    void update_direction();
    void go_safest_direction();
    void go_safest_turn_direction();
    bool ok_to_turn();
    void turn_random_direction();
    void change_direction(Input::Direction direction);
    int how_long_is_direction_safe(Input::Direction direction);
    bool is_direction_safe_later(Input::Direction direction);
    bool is_left_safe_later();
    bool is_right_safe_later();
    bool is_down_safe_later();
    bool is_up_safe_later();
    bool is_direction_safe(Input::Direction direction);
    bool is_left_safe();
    bool is_right_safe();
    bool is_down_safe();
    bool is_up_safe();
    bool try_go_left();
    bool try_go_right();
    bool try_go_down();
    bool try_go_up();
    bool go_left();
    bool go_right();
    bool go_down();
    bool go_up();
    bool simulate_left(Collision_Table *simulated_table);
    bool simulate_right(Collision_Table *simulated_table);
    bool simulate_down(Collision_Table *simulated_table);
    bool simulate_up(Collision_Table *simulated_table);
    bool simulate_direction(Input::Direction direction, Collision_Table *simulated_table);
    Input::Direction find_safe_direction();
    Input::Direction find_safest_direction();
    Input::Direction find_safest_turn_direction();
    Input::Direction get_random_turn_direction();
	int get_max(int a, int b);
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
    int m_offensive_count;
};

#endif

