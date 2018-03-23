/**
 * \class Persistence
 *
 *
 * \brief Provide an example
 *
 * This class is meant as an example.  It is not useful by itself
 * rather its usefulness is only a function of how much it helps
 * the reader.  It is in a sense defined by the person who reads it
 * and otherwise does not exist in any real form.
 *
 * \note Attempts at zen rarely work.
 *
 * \author (last to touch it) $Author: bv $
 *
 * \version $Revision: 1.5 $
 *
 * \date $Date: 2005/04/14 14:16:20 $
 *
 * Contact: bv@bnl.gov
 *
 * Created on: Wed Apr 13 18:39:37 2005
 *
 * $Id: doxygen-howto.html,v 1.5 2005/04/14 14:16:20 bv Exp $
 *
 */
#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include <stdint.h>

#include <file_system_handler.h>
#include <signals.h>
#include <types.h>
#include <persistence_error_codes.h>
#include <FreematicsSD.h>
#include <gps.h>

using namespace obd;
using namespace ourTypes;
using namespace gps;
#define SIZE_OF_CURRENT_DATE          8
#define SIZE_OF_LOGGING_ENTRY         15
 //Size of MVID in Bytes
#define SIZE_OF_MVID_COUNTER          1
//Folder name is the hex value of MVID_COUNTER + String-termination
#define SIZE_OF_MVID_FOLDER_NAME      (SIZE_OF_MVID_COUNTER * 2) + 1
#define SIZE_OF_FILE_PATH             16
#if SIZE_OF_MVID_COUNTER == 1
  #define MVID_TYPE uint8_t
  #define MVID_MAX  0xFF
#elif SIZE_OF_MVID_COUNTER == 2
  #define MVID_TYPE uint16_t
  #define MVID_MAX  0xFFFF
#elif SIZE_OF_MVID_COUNTER == 4
  #define MVID_TYPE uint32_t
  #define MVID_MAX  0xFFFFFFFF
#else
  #define MVID_TYPE uint8_t
  #define MVID_MAX  0xFF
#endif

#define SIZE_OF_BT_UPLOAD_LOG_ENTRY   14  //Car+Date+position
namespace persistence {
class Persistence{
  public:
    /**
     * @brief The Constructor for the Logging Class
     * Here all initalization for the class is done..
     *
     * @param current_vid the Vehicle Identification number of the vehicle the software is running in
     * @param current_time the time when the Logging starts
     * @param file_system The handler for the filesystem
     */
    void init(LocationTimeService *clock,
                File_System_Handler *file_system);
    stdRetVal create_logging_entry( uint64_t time,
                              uint16_t data_id, uint32_t data_value);
    stdRetVal update_file_name();/// Checks if the time passes 23:59 and opens a new file
    stdRetVal close_logging_file();
    uint8_t get_next_entry(uint16_t *position, uint8_t mvid, char *log_file, uint8_t *entry);
    /**
     * @brief writes the last uploaded file and its postition to the Bt-logfile
     *
     */
    void log_bt_upload_position(uint8_t car, char *date, uint16_t position);
    /*Getter*/
    stdRetVal GetInitStatus();
    /*Setter*/
  protected:
  private:
    //Variables

    /// The Handler for file system
    /** This field encapsulates a class that provides functions for file wirting and handling*/
    File_System_Handler *_file_system;
    ///
    /***/
    stdRetVal _initStatus = 0x00;
    /// The mapped vid value of the current car
    /** The Vecicle ID is mapped to a smaler value to safe memory space. This is the vaalue of the mapped VID*/
    uint8_t _current_mvid = 0;
    /// The Date of the File that is currently openend
    /** Each logfile is named after the current date this field saves the date for the current logfile*/
    uint32_t _open_file_date; //Has to change if logging passes 11:59 pm
    /// File handle for the current logfile
    /** The file handler for the current logfile for writing, creating and deleting*/
    File _open_file;
    /// The clock getting the time from GPS
    LocationTimeService *_clock;
    //Functions
    stdRetVal set_mapped_vehicle_id(); /// Sets the field _current_mvid to the vars MVID
    stdRetVal find_last_written_file(File *ret_file); /// Finds the last wirtten file
    stdRetVal create_and_open_logging_file(char *folder, char *file_name);
    stdRetVal open_logging_file(); /// Creates a logging file like: 18JAN018.log for 18th of January 2018
    stdRetVal setOpenFileDate(uint32_t file_name); /// Setter for the date
    char* getMonthNumber(char *month); /// Retruns the number of the Monnth JAN --> 01
};//End class Persistence
};//End namespace persistence

#endif
