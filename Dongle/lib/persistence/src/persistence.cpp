#include "persistence.h"

char Persitence::init()
{
  this->status = 1;
  this->card = new SDClass();
  return card->begin(SD_CHIP_SELECT_PIN);
}

char Persitence::stop()
{
  return this->status = 0;

}
