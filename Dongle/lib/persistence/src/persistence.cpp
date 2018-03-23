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
 * @param file_system The handler for the filesystem
 */
void Persistence::init(LocationTimeService *clock, File_System_Handler *file_system){

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
  //MVID_TYPE depends on SIZE_OF_MVID_COUNTER
  MVID_TYPE mvid_counter = 0;
  char mvid_counter_str[SIZE_OF_MVID_FOLDER_NAME];
  sprintf(mvid_counter_str, "%x", mvid_counter);
  //Loop over existing folders to get new MVID
  while(this->_file_system->exists(mvid_counter_str))
  {
    mvid_counter++;
    sprintf(mvid_counter_str, "%x", mvid_counter);
    // If Number of Maximum Loggings is reached continue logging to MVID_MAX
    if (mvid_counter == MVID_MAX)
    {
      this->_current_mvid = mvid_counter;
      return MVID_COUNTER_FULL;
    }
  }
  this->_current_mvid = mvid_counter;
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
  this->_file_system->open_file("FILES.LOG", 'w');
  this->_file_system->getCurrentFile().write(file_path);
  this->_file_system->getCurrentFile().write("\n");
  this->_file_system->getCurrentFile().close();
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
  char folder[SIZE_OF_MVID_FOLDER_NAME]; //Mapped vehicle ID
  char file_name[SIZE_OF_CURRENT_DATE + 1]; //DDMMYYY
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

void Persistence::log_bt_upload_position(uint8_t car, char *date, uint16_t position){
  char buf[SIZE_OF_BT_UPLOAD_LOG_ENTRY+1];
  int i ;
  buf[0] = car;
  buf[1] = date[0];
  buf[2] = date[1];
  buf[3] = date[2];
  buf[4] = date[3];
  buf[5] = date[4];
  buf[6] = date[5];
  buf[7] = '.';
  buf[8] = 'L';
  buf[9] = 'O';
  buf[10] = 'G';
  buf[11] = '\n';
  buf[12] = (uint8_t) position >> 8;
  buf[13] = (uint8_t) position;
  buf[14] = 0;
  for(i = 0; i< SIZE_OF_BT_UPLOAD_LOG_ENTRY; i++){
    Serial.print(buf[i]);
  }

  this->_file_system->open_file("BT.LOG", 'w');
  this->_file_system->getCurrentFile().write(buf, SIZE_OF_BT_UPLOAD_LOG_ENTRY);
  this->_file_system->getCurrentFile().close();
}

uint8_t Persistence::get_next_entry(uint16_t *position, uint8_t mvid, char *log_file, uint8_t *entry){
  uint8_t i = 0;
  char file_path[SIZE_OF_FILE_PATH] = "";

  sprintf(file_path, "%x/", mvid);
  strcat(file_path, log_file);

  this->_file_system->open_file(file_path, 'r');
  this->_file_system->getCurrentFile().seek(*position * SIZE_OF_LOGGING_ENTRY);
  if(this->_file_system->getCurrentFile().read(entry, SIZE_OF_LOGGING_ENTRY) != -1){
    i++;
  }
  else{
    return UNDEFINED_ERROR;
  }
  *position = *position + 1;
  return 0;
}
