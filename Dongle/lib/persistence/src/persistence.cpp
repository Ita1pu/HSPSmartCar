#include <persistence.h>
using namespace persistence;

Persistence::Persistence(const vid *current_vid, uint32_t current_time, Vid_mapper *mapper){
  /** \brief The Constructor for the Logging Class
    * Here all initalization for the class is done..
    *
    * \param *current_vid the Vehicle Identification number of the vehicle the software is running in
    * \param current_time the time when the Logging starts
    *
  */
  this->_vid_mapper = mapper;
  this->_current_mvid = this->get_mapped_vehicle_id(current_vid);
  this->_open_file_date = current_time;
  this->_open_file = open_logging_file(current_time);
}

bool Persistence::create_logging_entry(uint8_t mapped_vehicle_id, uint32_t time,
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
  return false;
  }

File Persistence::find_last_written_file(uint32_t current_time){
  /** \brief open the last fiel which hast been written to the file system
    *
    * Search the filesystem for the last entry that has been written-
    *
    *\param current_time the current time
    *
    *\return the last written File
  */
  File ret = File();
  return ret;
}

File Persistence::create_logging_file(uint32_t current_time){
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
  File ret = File();
  return ret;
}

uint8_t Persistence::get_mapped_vehicle_id(const vid *current_vid){
  /** \Gets the mvid to the passed current_vid
    *
    * For memory space reasosn each VID is mapped to an mvid on the file system
    * This function finds the MVID in the table and returns the mapped one
    *
    * \pram current_vid the VID of the car where the Dongle is used
  */
  uint8_t mvid;
  this->_vid_mapper->get_map_value(&mvid);

  return mvid;
}

File Persistence::open_logging_file(uint32_t logging_start_time){
  /** \brief opens the loggfile for the passed looging_start_time
    *
    * This function opens a file for the passed logging time. If no file is
    * found then a new one will be created
    *
    * \param logging_start_time the time when the logging starts
  */
  File ret = File();
  return ret;
}

bool Persistence::update_file_name(uint32_t current_time){
  /** \Checks if date has changed and creates a new file if neccessary
    *
    * When the vehicle was driving through midnight then a new file has to be
    * created. This function checks it and creates a new file if neccessary
    *
    *\ param current_time the current time
    *
  */
  return false;
}
