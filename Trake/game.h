#ifndef GAME_H
#define GAME_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>


class AI;
class Controls;
class Collision_Table;
class Pellet;
class Snake;
class Music;
class Scoreboard;
class Pause_Menu;

class Game
{
  public:
    Game(ALLEGRO_EVENT_QUEUE *event, Controls *controls, ALLEGRO_THREAD *music_fade_thread, float screen_width, float screen_height, float snake_width, float font_small_incrementor, float font_medium_incrementor, float font_large_incrementor, ALLEGRO_FONT *font_small, ALLEGRO_FONT *font_medium, ALLEGRO_FONT *font_large, float music_level, float sound_effects_level, int human_players, int ai_players, int gametype, int win_condition, int rounds, ALLEGRO_SAMPLE *move_sound_down, ALLEGRO_SAMPLE *move_sound_up);
    ~Game();
    void run();
    void draw_loading();
  private:
    bool is_anyone_alive();
    int how_many_are_alive();
    void draw_loading(int round);
    void show_current_standing(bool hide_standing, bool game_over);
    float m_screen_width;
    float m_screen_height;
    float m_snake_width;
    float m_font_small_incrementor;
    float m_font_medium_incrementor;
    float m_font_large_incrementor;
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
    int m_player_scores[4];
    ALLEGRO_FONT *m_font_small;
    ALLEGRO_FONT *m_font_medium;
    ALLEGRO_FONT *m_font_large;
    ALLEGRO_EVENT_QUEUE *m_event;
    ALLEGRO_THREAD *m_music_fade_thread;
    AI *m_ai[4];
    Snake *m_snakes[4];
    Controls *m_controls;
    Collision_Table *m_collision_table;
    Pellet *m_pellet;
    Music *m_music;
    Scoreboard *m_scoreboard;
    Pause_Menu *m_pause_menu;
    ALLEGRO_SAMPLE *m_move_sound_down;
    ALLEGRO_SAMPLE *m_move_sound_up;
};

#endif

