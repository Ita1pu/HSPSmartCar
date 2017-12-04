#include <persistence.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
using namespace persistence;

Persistence::Persistence(const vid *current_vid, uint32_t current_time,
                        Vid_mapper *mapper,File_System_Handler *file_system){
  /** \brief The Constructor for the Logging Class
    * Here all initalization for the class is done..
    *
    * \param *current_vid the Vehicle Identification number of the vehicle the software is running in
    * \param current_time the time when the Logging starts
    *
  */

  this->_vid_mapper = mapper;
  this->_file_system = file_system;
  this->_initStatus |= this->set_mapped_vehicle_id();
  this->_initStatus |= this->open_logging_file(current_time, &this->_open_file);
}

stdRetVal Persistence::set_mapped_vehicle_id()
{
   /** \Gets the mvid to the passed current_vid
    *
    * For memory space reasosn each VID is mapped to an mvid on the file system
    * This function finds the MVID in the table and returns the mapped one
    *
    * \pram current_vid the VID of the car where the Dongle is used
  */
  this->_vid_mapper->get_map_value(&this->_current_mvid);
  return NO_ERROR;
}

stdRetVal Persistence::create_logging_entry(uint32_t time,
  uint16_t data_id, uint32_t data_value){
  /**\brief create a logging entry on the file_system
    *
    * One entry is written to the file_system.
    * An entry looks like: **MVID_DATE_DATAID_VALUE**
    *\param mapped_vehicle_id the mapped vehilce id which leads to the real one
    *\param time the time for when the logging data was caputred
    *\param data_id the id for the data to be logged
    *\param data_value
    *
    *\return True if the logging was successful else False
  */
  return NO_ERROR;
  }

stdRetVal Persistence::find_last_written_file(uint32_t current_time,
                                              File *ret_file){
  /** \brief open the last fiel which hast been written to the file system
    *
    * Search the filesystem for the last entry that has been written-
    *
    *\param current_time the current time
    *
    *\return the last written File
  */
  *ret_file = File();
  return NO_ERROR;
}

stdRetVal Persistence::create_logging_file(uint32_t current_time,
                                              File *ret_file){
  /** \brief create a new logfile
    *
    * Creates a new logfile in the fileystem according to the current time
    * The file is named like the date it was created e.g. 01012017.log.
    * It is stored in folder named after the corresponding mapped vehicle ID
    *
    * \param current_time the current time
    *
    * \return The created file
  */
  *ret_file = File();
  return NO_ERROR;
}


stdRetVal Persistence::open_logging_file(uint32_t logging_start_time, File *ret_file){
  /** \brief opens the loggfile for the passed looging_start_time
    *
    * This function opens a file for the passed logging time. If no file is
    * found then a new one will be created
    *
    * \param logging_start_time the time when the logging starts
  */
  time_t logtime = logging_start_time;
  char file_path[13] = {0};
  char folder[4];
  char chrtime[26];
  sprintf(folder, "%x/", this->_current_mvid);
  //asctime returns: Sat Jan 01 00:04:16 2000
  sprintf(chrtime,asctime(gmtime(&logtime)));
  //Filename is for example: 18JAN118.log for 18th of January 2118
  char file_name[10] = { chrtime[8], chrtime[9], chrtime[4], chrtime[5], chrtime[6], chrtime[21], chrtime[22], chrtime[23], '\0'};
  this->setOpenFileDate(file_name);
  strcat(file_path, folder);
  strcat(file_path, file_name);
  strcat(file_path, ".log");

  if (!(this->_file_system->exists(folder)))//Check if folder for MVID already exists
  {
    //Folder does not exist
    uint8_t res = this->_file_system->mkdir(folder);
    if (res != NO_ERROR)
    {//Could not create folder
      return UNDEFINED_ERROR;
    }
    //FOlder exists now
  }
  this->_file_system->open_file(file_path, 'w');
  this->_open_file = _file_system->getCurrentFile();
  this->_open_file.print("Test");
  this->_open_file.close();
  return NO_ERROR;
}

stdRetVal Persistence::update_file_name(uint32_t current_time){
  /** \Checks if date has changed and creates a new file if neccessary
    *
    * When the vehicle was driving through midnight then a new file has to be
    * created. This function checks it and creates a new file if neccessary
    *
    *\ param current_time the current time
    *
  */
  return NO_ERROR;
}

stdRetVal Persistence::GetInitStatus()
{
  return this->_initStatus;
}

void Persistence::setOpenFileDate(char *file_name)
{/*!Assigns the first 5 chars of the filename to the date: E.g. 18JAN for 18th of January*/
  for (int i = 0; i < SIZE_OF_CURRENT_DATE - 1; i++)
  {
    this->_open_file_date[i] = file_name[i];
  }
  this->_open_file_date[SIZE_OF_CURRENT_DATE - 1] = 0; //Terminator
}