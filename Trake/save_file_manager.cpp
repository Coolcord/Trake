#include "save_file_manager.h"
#include "controls.h"
#include <fstream>
#include <iterator>

void Save_File_Manager::save(int music_level, int sound_level, Controls *controls)
{
  std::ofstream save_file;
  save_file.open("TrakeSettings.cfg");
  save_file << music_level << std::endl;
  save_file << sound_level << std::endl;
  if (controls)
  {
    std::unordered_map<int, int> *map = controls->get_map();
    for (std::unordered_map<int, int>::iterator iter = map->begin(); iter != map->end(); ++iter)
    {
      save_file << (*iter).first << std::endl;
      save_file << (*iter).second << std::endl;
    }
  }
  save_file.close();
}

void Save_File_Manager::load(int &music_level, int &sound_level, Controls *controls)
{
  
}

