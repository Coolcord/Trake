#ifndef GAME_H
#define GAME_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>


class AI;
class Collision_Table;
class Pellet;
class Snake;
class Music;
class Scoreboard;

class Game
{
  public:
    Game(ALLEGRO_EVENT_QUEUE *event, float screen_width, float screen_height, float snake_width, float music_level, float sound_effects_level, int human_players, int ai_players, int gametype, int win_condtion, int rounds);
    ~Game();
    void run();
  private:
    bool is_anyone_alive();
    int how_many_are_alive();
    void draw_loading(int round);
    float m_screen_width;
    float m_screen_height;
    float m_snake_width;
    float m_max_x;
    float m_max_y;
    float m_game_height;
    float m_scoreboard_y;
    float m_music_level;
    float m_sound_effects_level;
    int m_num_snakes;
    int m_num_ai;
    int m_rounds;
    bool m_tron;
    int m_win_condition;
    int m_start_snake_length;
    int m_max_spawn_time;
    float m_player_1_start_x;
    float m_player_1_start_y;
    float m_player_2_start_x;
    float m_player_2_start_y;
    float m_player_3_start_x;
    float m_player_3_start_y;
    float m_player_4_start_x;
    float m_player_4_start_y;
    ALLEGRO_FONT *m_font;
    ALLEGRO_EVENT_QUEUE *m_event;
    AI *m_ai[4];
    Snake *m_snakes[4];
    Collision_Table *m_collision_table;
    Pellet *m_pellet;
    Music *m_music;
    Scoreboard *m_scoreboard;
};

#endif

