#ifndef PAUSE_MENU_H
#define PAUSE_MENU_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

class Controls;
class Simple_Rectangle;

class Pause_Menu
{
  public:
    Pause_Menu(ALLEGRO_EVENT_QUEUE *event, Controls *controls, float screen_width, float screen_height, float font_large_incrementor, ALLEGRO_FONT *font_large, ALLEGRO_SAMPLE *move_sound_down, ALLEGRO_SAMPLE *move_sound_up, float sound_effects_level, bool *quit, bool *hide_standing, int *rounds, int total_rounds);
    ~Pause_Menu();
    void draw();
    void show();
  private:
    float m_screen_width;
    float m_screen_height;
    float m_font_large_incrementor;
    int m_selection;
    Simple_Rectangle *m_rectangle;
    ALLEGRO_EVENT_QUEUE *m_event;
    ALLEGRO_FONT *m_font;
    ALLEGRO_SAMPLE *m_move_sound_down;
    ALLEGRO_SAMPLE *m_move_sound_up;
    float m_sound_effects_level;
    bool *m_quit;
    bool *m_hide_standing;
    int *m_rounds;
    int m_total_rounds;
    Controls *m_controls;
};

#endif

