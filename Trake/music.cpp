#include "music.h"
#include <allegro5/allegro.h>
#include <string>

Music::Music(bool tron)
{
  m_tron = tron;
  int random = (rand() % 3) + 1;
  std::string song_name = "./music/";
  if (m_tron)
    song_name += "tron";
  else
    song_name += "snake";
  song_name += "_song_" + std::to_string(random) + ".ogg";
  m_song = al_load_sample(song_name.c_str());
  if (m_song)
    m_song_instance = al_create_sample_instance(m_song);
  else
    m_song_instance = NULL;
  if (m_song_instance)
  {
    al_set_sample_instance_playmode(m_song_instance, ALLEGRO_PLAYMODE_LOOP);
    al_attach_sample_instance_to_mixer(m_song_instance, al_get_default_mixer());
  }
}

Music::~Music()
{
  al_destroy_sample_instance(m_song_instance);
  al_destroy_sample(m_song);
}

void Music::play()
{
  if (m_song_instance) al_play_sample_instance(m_song_instance);
}

void Music::slow_to_stop()
{
  for (float i = al_get_sample_instance_speed(m_song_instance); i > 0; i -= 0.01)
  {
    al_set_sample_instance_speed(m_song_instance, i);
    al_rest(0.02);
  }
  al_rest(0.1);
}
