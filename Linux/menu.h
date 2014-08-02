#ifndef MENU_H
#define MENU_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <vector>

class Controls;
class Rectangle;
class Game;
class Music;

namespace Music_Thread
{
  void *Music_Fade_Thread(ALLEGRO_THREAD *thread, void *arg);
  Music *Music_Load_Thread(ALLEGRO_THREAD *thread, void *arg);
};

class Menu
{
  public:
    Menu(ALLEGRO_EVENT_QUEUE *event, float screen_width, float screen_height, float snake_width);
    ~Menu();
    void show();
  private:
    void show_title();
    void show_game_setup();
    void show_options();
    void show_controls();
    void show_control_setup(int player_num);
    void show_credits();
    void assign_key(int player_num, int selection);
    void draw_title_logo();
    void create_t(float x, float y);
    void create_r(float x, float y);
    void create_a(float x, float y);
    void create_k(float x, float y);
    void create_e(float x, float y);
    void create_title(float x, float y);
    ALLEGRO_THREAD *m_music_fade_thread;
    ALLEGRO_EVENT_QUEUE *m_event;
    int m_gametype_selection;
    int m_win_selection;
    int m_rounds;
    int m_human_players;
    int m_ai_players;
    int m_ai_difficulty;
    float m_screen_width;
    float m_screen_height;
    float m_title_snake_width;
    float m_snake_width;
    float m_thickness;
    float m_font_small_incrementor;
    float m_font_medium_incrementor;
    float m_font_large_incrementor;
    int m_music_level;
    int m_sound_effects_level;
    ALLEGRO_FONT *m_font_small;
    ALLEGRO_FONT *m_font_medium;
    ALLEGRO_FONT *m_font_large;
    ALLEGRO_SAMPLE *m_move_sound_down;
    ALLEGRO_SAMPLE *m_move_sound_up;
    Controls *m_controls;
    Game *m_game;
    Music *m_music;
    std::vector<Rectangle*> *m_title;
};

#endif

