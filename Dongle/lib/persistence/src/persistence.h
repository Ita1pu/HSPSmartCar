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
#include <SD.h>

using namespace obd;
using namespace ourTypes;
#define SIZE_OF_CURRENT_DATE          5
namespace persistence {
class Persistence{
  public:
    //Constructor
    Persistence(const vid *current_vid, uint32_t current_time, Vid_mapper *mapper,
                File_System_Handler *file_system);
    stdRetVal create_logging_entry( uint32_t time,
                              uint16_t data_id, uint32_t data_value);
    stdRetVal init();
    /*Getter*/
    stdRetVal GetInitStatus();
    /*Setter*/
  protected:
  private:
    //Variables
    File_System_Handler *_file_system;
    stdRetVal _initStatus = 0x00;
    uint8_t _current_mvid = 0;
    char _open_file_date[SIZE_OF_CURRENT_DATE]; //Has to change if logging passes 11:59 pm
    File _open_file;
    File _vid_map_table;
    Vid_mapper *_vid_mapper;
    //Functions
    stdRetVal update_file_name( uint32_t current_time );
    stdRetVal set_mapped_vehicle_id();
    stdRetVal find_last_written_file(uint32_t current_time, File *ret_file);
    stdRetVal create_logging_file(uint32_t current_time, File *ret_file);
    stdRetVal open_logging_file(uint32_t logging_start_time, File *ret_file);
    void setOpenFileDate(char *file_name);
};//End class Persitence
};//End namespace persistence

#endif
