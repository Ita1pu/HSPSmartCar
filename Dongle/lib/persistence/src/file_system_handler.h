#ifndef FILE_SYSTEM_HANDLER_H
#define FILE_SYSTEM_HANDLER_H

//#include <FreematicsBase.h>
#include <FreematicsSD.h>
#include <persistence_error_codes.h>
#include <types.h>



namespace persistence{
class File_System_Handler{
  public:
    File_System_Handler(SDClass *sdClass);
    /*Getters*/
    /*Setter*/

    //Functions
    stdRetVal stop();
    stdRetVal mkdir( char *name);
    stdRetVal rmdir( char *dir_path);
    bool exists( char *dir_path);
    stdRetVal rmFile( char *file_path);
    stdRetVal open_file(char *file_path, char mode);
    stdRetVal close_current_file();
    stdRetVal flush_current_file();
    File getCurrentFile();

  private:
    stdRetVal init();
    //A Pointer to the SD_Card class
    SDClass *card;
    //The file which is currently opened
    File current_file;
    void convertPath(char *file_path);
    //The satus of the class
    uint8_t status;
};
};

#endif
