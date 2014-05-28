#ifndef CONTROLS_H
#define CONTROlS_H

#include <unordered_map>

class Controls
{
  public:
    enum Player_Controls
    {
      NONE,
      PLAYER_1_LEFT,
      PLAYER_1_RIGHT,
      PLAYER_1_DOWN,
      PLAYER_1_UP,
      PLAYER_1_CONFIRM,
      PLAYER_1_CANCEL,
      PLAYER_2_LEFT,
      PLAYER_2_RIGHT,
      PLAYER_2_DOWN,
      PLAYER_2_UP,
      PLAYER_2_CONFIRM,
      PLAYER_2_CANCEL,
      PLAYER_3_LEFT,
      PLAYER_3_RIGHT,
      PLAYER_3_DOWN,
      PLAYER_3_UP,
      PLAYER_3_CONFIRM,
      PLAYER_3_CANCEL,
      PLAYER_4_LEFT,
      PLAYER_4_RIGHT,
      PLAYER_4_DOWN,
      PLAYER_4_UP,
      PLAYER_4_CONFIRM,
      PLAYER_4_CANCEL
    };
    Controls();
    ~Controls();
    int get_control(int key);
    void set_control(int key, int control);
  private:
    std::unordered_map<int, int> *m_controls;
};

#endif

