/**
 * \class Persitence
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
#include <vid_mapper.h>
#include <persistence_error_codes.h>
#include <FreematicsSD.h>
#include <clock.h>

using namespace obd;
using namespace ourTypes;
#define SIZE_OF_CURRENT_DATE          8
#define SIZE_OF_LOGGING_ENTRY         15
namespace persistence {
class Persistence{
  public:
    /**
     * @brief The Constructor for the Logging Class
     * Here all initalization for the class is done..
     * 
     * @param current_vid the Vehicle Identification number of the vehicle the software is running in
     * @param current_time the time when the Logging starts
     * @param mapper The Mapp handler for VID-->MVID
     * @param file_system The handler for the filesystem
     */
    Persistence(const vid *current_vid, Clock *clock, Vid_mapper *mapper,
                File_System_Handler *file_system);
    stdRetVal create_logging_entry( uint64_t time,
                              uint16_t data_id, uint32_t data_value);
    stdRetVal init();
    stdRetVal update_file_name();/// Checks if the time passes 23:59 and opens a new file 
    stdRetVal close_logging_file();
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
    /// The mapfile for VID --> MVID
    /** The mapping is stored in a file. This is the name of the mapfile*/
    File _vid_map_table;
    /// The class handling the mapping of the VIDs
    /** This filed holds the class used for VID mapping*/
    Vid_mapper *_vid_mapper;
    /// The clock getting the time from GPS
    Clock *_clock;
    //Functions
    stdRetVal set_mapped_vehicle_id(); /// Sets the field _current_mvid to the vars MVID
    stdRetVal find_last_written_file(File *ret_file); /// Finds the last wirtten file
    stdRetVal create_and_open_logging_file(char *folder, char *file_name);
    stdRetVal open_logging_file(); /// Creates a logging file like: 18JAN018.log for 18th of January 2018
    stdRetVal setOpenFileDate(uint32_t file_name); /// Setter for the date
    char* getMonthNumber(char *month); /// Retruns the number of the Monnth JAN --> 01
};//End class Persitence
};//End namespace persistence

#endif
