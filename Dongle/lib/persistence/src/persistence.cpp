#include <persistence.h>
#include <stdio.h>
#include <stdlib.h>
using namespace persistence;

/**
 * @brief The Constructor for the Logging Class
 * Here all initalization for the class is done..
 * 
 * @param current_vid the Vehicle Identification number of the vehicle the software is running in
 * @param current_time the time when the Logging starts
 * @param mapper The Mapp handler for VID-->MVID
 * @param file_system The handler for the filesystem
 */
Persistence::Persistence(const vid *current_vid, Clock *clk,
                        Vid_mapper *mapper, File_System_Handler *file_system){

  this->_vid_mapper = mapper;
  this->_file_system = file_system;
  this->_clk = clk;
  this->_initStatus |= this->set_mapped_vehicle_id();
  this->_initStatus |= this->open_logging_file();
}

/**
 * @brief Gets the mvid to the passed current_vid
 * 
 * @return stdRetVal 
 */
stdRetVal Persistence::set_mapped_vehicle_id()
{
  this->_vid_mapper->get_map_value(&this->_current_mvid);
  return NO_ERROR;
}

/**
 * @brief create a logging entry in the current open file
 * 
 * @param time the  time when the value has been logged
 * @param data_id The ID of the logdata
 * @param data_value The value of the data
 * @return stdRetVal 
 */
stdRetVal Persistence::create_logging_entry(uint64_t logging_time, uint16_t data_id, uint32_t data_value){
  return NO_ERROR;
  }

  /**
   * @brief open the last fiel which hast been written to the file system
   *
   * Search the filesystem for the last entry that has been written
   * 
   * @param current_time the current time
   * @param ret_file A pointer to the last wirrten file. Will be set by the class
   * @return stdRetVal 
   */
stdRetVal Persistence::find_last_written_file(File *ret_file){

  *ret_file = File();
  return NO_ERROR;
}
//TODO remove func
stdRetVal Persistence::create_logging_file( File *ret_file){
  /** \brief create a new logfile
    *
    * Creates a new logfile in the fileystem according to the current time
    * The file is named like the date it was created e.g. 010117.log.
    * It is stored in folder named after the corresponding mapped vehicle ID
    *
    * \param current_time the current time
    *
    * \return The created file
  */
  *ret_file = File();
  return NO_ERROR;
}

/**
 * @brief opens the loggfile for the passed looging_start_time
 * This function opens a file for the passed logging time. If no file is found then a new one will be created
 * 
 * @param start_date the Date generated from GPS-Module
 * @return stdRetVal 
 */
stdRetVal Persistence::open_logging_file(){
  char file_path[16] = {0};
  char folder[3];
  char file_name[SIZE_OF_CURRENT_DATE + 1];
  uint32_t start_date = this->_clk->GetDate();
  sprintf(file_name, "%lu", start_date);
  sprintf(folder, "%x/", this->_current_mvid);
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
/**
 * @brief Checks if date has changed and creates a new file if neccessary
 * 
 * When the vehicle was driving through midnight then a new file has to be
 * created. This function checks it and creates a new file if neccessary
 * @param current_time the current time
 * @return stdRetVal 
 */
stdRetVal Persistence::update_file_name(){
  return NO_ERROR;
}

stdRetVal Persistence::GetInitStatus()
{
  return this->_initStatus;
}
/**
 * @brief Assigns the first 5 chars of the filename to the date: E.g. 18JAN for 18th of January
 * 
 */
void Persistence::setOpenFileDate(char *file_name){
  strcpy(this->_open_file_date, file_name);
}
