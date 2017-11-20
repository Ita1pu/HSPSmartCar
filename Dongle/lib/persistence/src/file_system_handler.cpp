#include "file_system_handler.h"

uint8_t File_System_Handler::init()
{
  this->status = 1;
  this->card = new SDClass();
  return card->begin(SD_CHIP_SELECT_PIN);
}

uint8_t File_System_Handler::stop()
{
  return this->status = 0;

}
