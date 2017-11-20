#ifndef FILE_SYSTEM_HANDLER_H
#define FILE_SYSTEM_HANDLER_H

#include <FreematicsBase.h>
#include <FreematicsSD.h>
#include <types.h>

class File_System_Handler{
  public:
    /*Getters*/
    File Current_file();
    SDClass Card();
    /*Setter*/

    //Functions
    uint8_t init();
    uint8_t stop();
    uint8_t mkdir( char *name, uint16_t name_len);
    uint8_t rmdir( char *dir_path, uint16_t dir_path_len);
    File open_file(char *file_path, uint16_t file_path_len, char mode);
    uint8_t close_current_file();

    //Persitence();

  private:
    //A Pointer to the SD_Card class
    SDClass *card;
    //The file which is currently opened
    File current_file;
    //The satus of the class
    uint8_t status;
};


#endif
