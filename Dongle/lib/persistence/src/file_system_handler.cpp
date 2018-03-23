#include "file_system_handler.h"

using namespace persistence;
#define SD_CS_PIN                          10

stdRetVal File_System_Handler::init(SDClass *sdClass)
{
  this->card = sdClass;
  this->status = NO_ERROR;
  this->status = 1;
  return card->begin(SD_CS_PIN);
}

stdRetVal File_System_Handler::stop()
{
  return this->status = 0;
}

stdRetVal File_System_Handler::mkdir( char *name)
{
  bool res = this->card->mkdir(name);
  if(res)
  {
    return NO_ERROR;
  }
  else
  {
    return res;
  }
}

stdRetVal File_System_Handler::rmdir( char *dir_path)
{
  bool res = this->card->rmdir(dir_path);
  if(res)
  {
    return NO_ERROR;
  }
  else
  {
    return res;
  }
}

stdRetVal File_System_Handler::rmFile( char *file_path){
  bool res = this->card->remove(file_path);
  if(res)
  {
    return NO_ERROR;
  }
  else
  {
    return res;
  }
}

stdRetVal File_System_Handler::open_file(char *file_path, char mode){
  /*Dont use filenames longer than 10 chars for writng..*/
  uint8_t intMode = 0,i = 0;
  switch (mode) {
    case 'w':
      intMode = FILE_WRITE;
      break;
    case 'r':
      intMode = FILE_READ;
      break;
    case 'a':
      intMode = FILE_WRITE;
      break;
    default:
      //TODO add errorhandling
      intMode = FILE_READ;
    }
  while (!(this->current_file) && i < 10)
  {
    this->current_file = this->card->open(file_path, intMode);
    i++;
  }
  if (this->current_file)
  {
    return NO_ERROR;
  }
  else
  {
    return UNDEFINED_ERROR;
  }
}

stdRetVal File_System_Handler::close_current_file()
{
  this->current_file.close();
  return NO_ERROR;
}

stdRetVal File_System_Handler::flush_current_file()
{
  this->current_file.flush();
  return NO_ERROR;
}

File File_System_Handler::getCurrentFile()
{
  return this->current_file;
}
// stdRetVal

bool File_System_Handler::exists( char *path)
{
  return this->card->exists(path);
}
