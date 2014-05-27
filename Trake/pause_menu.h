#ifndef PAUSE_MENU_H
#define PAUSE_MENU_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

class Rectangle;

class Pause_Menu
{
  public:
    Pause_Menu(ALLEGRO_EVENT_QUEUE *event, float screen_width, float screen_height, ALLEGRO_SAMPLE *move_sound_down, ALLEGRO_SAMPLE *move_sound_up, float sound_effects_level, bool *quit, int *rounds, int total_rounds);
    ~Pause_Menu();
    void draw();
    void show();
  private:
    float m_screen_width;
    float m_screen_height;
    int m_selection;
    Rectangle *m_rectangle;
    ALLEGRO_EVENT_QUEUE *m_event;
    ALLEGRO_FONT *m_font;
    ALLEGRO_SAMPLE *m_move_sound_down;
    ALLEGRO_SAMPLE *m_move_sound_up;
    float m_sound_effects_level;
    bool *m_quit;
    int *m_rounds;
    int m_total_rounds;
};

#endif

