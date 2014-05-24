#ifndef MENU_H
#define MENU_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <vector>

class Rectangle;
class Game;

class Menu
{
  public:
    enum Screen
    {
      NONE,
      TITLE,
      PLAYERS,
      GAMETYPE,
      OPTIONS
    };
    Menu(ALLEGRO_EVENT_QUEUE *event, float screen_width, float screen_height, float snake_width);
    ~Menu();
    void show();
    void handle_input();
  private:
    void draw();
    void draw_loading();
    void draw_title_logo();
    void show_title();
    void create_t(float x, float y);
    void create_r(float x, float y);
    void create_a(float x, float y);
    void create_k(float x, float y);
    void create_e(float x, float y);
    void create_title(float x, float y);
    ALLEGRO_EVENT_QUEUE *m_event;
    int m_selection;
    float m_screen_width;
    float m_screen_height;
    float m_snake_width;
    float m_thickness;
    Screen m_menu_screen;
    ALLEGRO_FONT *m_font;
    ALLEGRO_SAMPLE *m_move_sound_down;
    ALLEGRO_SAMPLE *m_move_sound_up;
    Game *m_game;
    std::vector<Rectangle*> *m_title;
};

#endif

