#include "music.h"
#include <allegro5/allegro.h>
#include <string>

Music::Music(float volume)
{
  m_volume = volume*0.1;
  m_tron = false;
  m_position = 0;
  m_song = al_load_sample("./music/menu_song.ogg");
  if (m_song)
    m_song_instance = al_create_sample_instance(m_song);
  else
    m_song_instance = NULL;
  if (m_song_instance)
  {
    al_set_sample_instance_gain(m_song_instance, m_volume);
    al_set_sample_instance_playmode(m_song_instance, ALLEGRO_PLAYMODE_LOOP);
    al_attach_sample_instance_to_mixer(m_song_instance, al_get_default_mixer());
  }
}

Music::Music(float volume, bool tron)
{
  m_volume = volume*0.1;
  m_tron = tron;
  int random = (rand() % 4) + 1;
  std::string song_name = "./music/";
  m_position = 0;
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
    al_set_sample_instance_gain(m_song_instance, m_volume);
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
  if (m_song_instance && m_volume > 0)
    al_play_sample_instance(m_song_instance);
}

void Music::speed_up()
{
  double speed = al_get_sample_instance_speed(m_song_instance) + 0.0001;
  al_set_sample_instance_speed(m_song_instance, speed);
}

void Music::slow_to_stop()
{
  if (m_song_instance)
  {
    for (float i = al_get_sample_instance_speed(m_song_instance); i > 0; i -= 0.01)
    {
      al_set_sample_instance_speed(m_song_instance, i);
      al_rest(0.02);
    }
    al_rest(0.2);
    al_stop_sample_instance(m_song_instance);
  }
  else
    al_rest(2);
}

void Music::fade_to_stop()
{
  if (m_song_instance)
  {
    float increment = 0.01 / m_volume;
    for (float i = al_get_sample_instance_gain(m_song_instance); i > 0 ; i -= increment)
    {
      al_set_sample_instance_gain(m_song_instance, i);
      al_rest(0.02);
    }
    al_rest(0.02);
    al_stop_sample_instance(m_song_instance);
  }
  else
    al_rest(2);
}

void Music::pause()
{
  if (m_song_instance && m_volume > 0)
  {
    m_position = al_get_sample_instance_position(m_song_instance);
    al_stop_sample_instance(m_song_instance);
  }
}

void Music::stop()
{
  if (m_song_instance)
  {
    al_stop_sample_instance(m_song_instance);
  }
}

void Music::resume()
{
  if (m_song_instance && m_volume > 0)
  {
    al_set_sample_instance_gain(m_song_instance, m_volume);
    al_set_sample_instance_position(m_song_instance, m_position);
    al_play_sample_instance(m_song_instance);
  }
}

void Music::set_volume(float volume)
{
  m_volume = volume * 0.1;
  if (m_song_instance)
    al_set_sample_instance_gain(m_song_instance, m_volume);
  else
    al_rest(2);
}

bool Music::is_playing()
{
  if (m_song_instance && al_get_sample_instance_playing(m_song_instance))
    return true;
  else
    return false;
}

