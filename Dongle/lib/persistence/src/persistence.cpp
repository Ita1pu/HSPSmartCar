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
Persistence::Persistence(const vid *current_vid, Clock *clock,
                        Vid_mapper *mapper, File_System_Handler *file_system){

  this->_vid_mapper = mapper;
  this->_file_system = file_system;
  this->_clock = clock;
  this->_initStatus |= this->set_mapped_vehicle_id();
  this->_initStatus |= this->open_logging_file();
}


stdRetVal Persistence::GetInitStatus()
{
  return this->_initStatus;
}
/**
 * @brief Assigns the first 5 chars of the filename to the date: YYYYMMDD
 * 
 */
stdRetVal Persistence::setOpenFileDate(uint32_t open_date){
  this->_open_file_date = open_date;
  return NO_ERROR;
}

stdRetVal Persistence::close_logging_file()
{
  this->_open_file.flush();
  this->_open_file.close();
  return NO_ERROR;
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
  uint8_t i = 0, offset = 0, buf[SIZE_OF_LOGGING_ENTRY];
  buf[offset] = this->_current_mvid;
  offset++;

  for ( i = 0; i < sizeof(logging_time); i++)
  {
    buf[offset] = logging_time >> ((sizeof(logging_time) - (i+1)) * 8 );
    offset++;
  }
  for ( i = 0; i <  sizeof(data_id); i++ )
  {
    buf[offset] = data_id >> ((sizeof(data_id) - (i+1)) * 8 );
    offset++;
  }
  for ( i = 0; i < sizeof(data_value); i++)
  {
    buf[offset] = data_value >> ((sizeof(data_value) - (i+1)) * 8);
    offset++;
  }
  this->_open_file.write(buf, SIZE_OF_LOGGING_ENTRY);
  this->_open_file.flush();
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
  //TDOD Think about if it is neccessary or change to last sent file..
  *ret_file = File();
  return NO_ERROR;
}
/**
 * @brief Creates the logging file for the current car and date
 * 
 * @param folder MVID
 * @param file_name Date
 * @return stdRetVal Error code
 */
stdRetVal Persistence::create_and_open_logging_file(char *folder, char *file_name){  
  stdRetVal ret = NO_ERROR;
  char file_path[16] = {0};
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
  if (!(this->_file_system->exists(file_path)))
  {
    this->_file_system->open_file(file_path, 'w');
    this->_open_file = _file_system->getCurrentFile();
    this->create_logging_entry(0,0,0);//For initializing the file
    ret = NEW_LOGGING_FILE_CREATED;
  }
  else
  {
    this->_file_system->open_file(file_path, 'w');
    this->_open_file = _file_system->getCurrentFile();
  }
  return ret;
}

/**
 * @brief opens the loggfile for the passed looging_start_time
 * This function opens a file for the passed logging time. If no file is found then a new one will be created
 * 
 * @param start_date the Date generated from GPS-Module
 * @return stdRetVal 
 */
stdRetVal Persistence::open_logging_file(){
  char folder[3]; //Mapped vehicle ID
  char file_name[SIZE_OF_CURRENT_DATE + 1]; //YYYYMMDD
  uint32_t start_date = this->_clock->GetDate(); 
  sprintf(file_name, "%lu", start_date);
  sprintf(folder, "%x/", this->_current_mvid);

  this->create_and_open_logging_file(folder, file_name);
  this->setOpenFileDate(start_date);
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
  stdRetVal ret = NO_ERROR;
  if(this->_clock->GetDate() != this->_open_file_date)
  {
    this->close_logging_file();
    this->open_logging_file();//Create logging file with new date
    ret = NEW_LOGGING_FILE_CREATED;
  }
  else
  {
    this->_open_file.flush();
  }
  return ret;
}

