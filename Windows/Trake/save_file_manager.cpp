#include "save_file_manager.h"
#include "controls.h"
#include <allegro5/allegro.h>
#include <assert.h>
#include <fstream>
#include <iterator>

void Save_File_Manager::save(int music_level, int sound_level, Controls *controls)
{
  assert(controls);
  std::ofstream save_file;
  save_file.open("TrakeSettings.cfg");
  save_file << music_level << std::endl;
  save_file << sound_level << std::endl;
  std::unordered_map<int, int> *map = controls->get_map();
  for (std::unordered_map<int, int>::iterator iter = map->begin(); iter != map->end(); ++iter)
  {
    save_file << (*iter).first << std::endl;
    save_file << (*iter).second << std::endl;
  }
  save_file.close();
}

void Save_File_Manager::load(int &music_level, int &sound_level, Controls &controls)
{
  std::ifstream save_file;
  save_file.open("TrakeSettings.cfg");
  if (save_file.good())
  {
    save_file >> music_level;
    save_file >> sound_level;
    while (!save_file.eof())
    {
      int key = 0;
      int control = 0;
      save_file >> key;
      save_file >> control;
      controls.set_control(key, control);
    }
  }
  else
  {
    music_level = 10;
    sound_level = 10;
    controls.set_control(ALLEGRO_KEY_ENTER, Controls::PLAYER_1_CONFIRM);
    controls.set_control(ALLEGRO_KEY_ESCAPE, Controls::PLAYER_1_CANCEL);
    controls.set_control(ALLEGRO_KEY_LEFT, Controls::PLAYER_1_LEFT);
    controls.set_control(ALLEGRO_KEY_RIGHT, Controls::PLAYER_1_RIGHT);
    controls.set_control(ALLEGRO_KEY_DOWN, Controls::PLAYER_1_DOWN);
    controls.set_control(ALLEGRO_KEY_UP, Controls::PLAYER_1_UP);
	controls.set_control(ALLEGRO_KEY_PAD_ENTER, Controls::PLAYER_2_CONFIRM);
	controls.set_control(ALLEGRO_KEY_BACKSPACE, Controls::PLAYER_2_CANCEL);
    controls.set_control(ALLEGRO_KEY_A, Controls::PLAYER_2_LEFT);
    controls.set_control(ALLEGRO_KEY_D, Controls::PLAYER_2_RIGHT);
    controls.set_control(ALLEGRO_KEY_S, Controls::PLAYER_2_DOWN);
    controls.set_control(ALLEGRO_KEY_W, Controls::PLAYER_2_UP);
    controls.set_control(ALLEGRO_KEY_J, Controls::PLAYER_3_LEFT);
    controls.set_control(ALLEGRO_KEY_L, Controls::PLAYER_3_RIGHT);
    controls.set_control(ALLEGRO_KEY_K, Controls::PLAYER_3_DOWN);
    controls.set_control(ALLEGRO_KEY_I, Controls::PLAYER_3_UP);
    controls.set_control(ALLEGRO_KEY_PAD_4, Controls::PLAYER_4_LEFT);
    controls.set_control(ALLEGRO_KEY_PAD_6, Controls::PLAYER_4_RIGHT);
    controls.set_control(ALLEGRO_KEY_PAD_5, Controls::PLAYER_4_DOWN);
    controls.set_control(ALLEGRO_KEY_PAD_8, Controls::PLAYER_4_UP);
  }
  save_file.close();
}

