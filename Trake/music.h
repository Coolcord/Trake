#ifndef MUSIC_H
#define MUSIC_H

#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

class Music
{
  public:
    Music(bool tron);
    ~Music();
    void play();
    void speed_up();
    void slow_to_stop();
    void pause();
    void resume();
  private:
    bool m_tron;
    unsigned int m_position;
    ALLEGRO_SAMPLE *m_song;
    ALLEGRO_SAMPLE_INSTANCE *m_song_instance;
};

#endif

