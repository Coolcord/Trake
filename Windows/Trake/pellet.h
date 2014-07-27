#ifndef PELLET_H
#define PELLET_H

#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

class Snake;
class Simple_Rectangle;
class Scoreboard;
class Collision_Table;

class Pellet
{
  public:
    Pellet(float width, float max_x, float max_y, float volume, int spawn_countdown_max, Scoreboard *scoreboard, Collision_Table *collision_table, bool tron);
    ~Pellet();
    void handle_state();
    void eat(Snake *snake);
    bool exists();
    float get_x();
    float get_y();
    void draw();
  private:
    float m_width;
    float m_max_x;
    float m_max_y;
    float m_volume;
    int m_value;
    bool m_exists;
    bool m_tron;
    int m_spawn_countdown_max;
    int m_spawn_countdown;
    int m_respawn_time_max;
    int m_respawn_time;
    void spawn();
    void remove();
    Simple_Rectangle *m_rectangle;
    Scoreboard *m_scoreboard;
    Collision_Table *m_collision_table;
    ALLEGRO_SAMPLE *m_eat_sound;
};

#endif

