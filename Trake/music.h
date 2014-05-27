#ifndef MUSIC_H
#define MUSIC_H

#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

class Music
{
  public:
    Music(float volume); //plays menu music
    Music(float volume, bool tron);
    ~Music();
    void play();
    void speed_up();
    void slow_to_stop();
    void fade_to_stop();
    void pause();
    void resume();
    void set_volume(float volume);
  private:
    bool m_tron;
    unsigned int m_position;
    ALLEGRO_SAMPLE *m_song;
    float m_volume;
    ALLEGRO_SAMPLE_INSTANCE *m_song_instance;
};

#endif

