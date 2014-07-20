#ifndef SAVE_FILE_MANAGER_H
#define SAVE_FILE_MANAGER_H

class Controls;

class Save_File_Manager
{
  public:
    Save_File_Manager() {}
    ~Save_File_Manager() {}
    void save(int music_level, int sound_level, Controls *controls);
    void load(int &music_level, int &sound_level, Controls *controls);
  private:
};

#endif

