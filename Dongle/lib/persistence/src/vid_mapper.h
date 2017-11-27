#ifndef VID_MAPPER
#define VID_MAPPER

#include <stdint.h>
#include <file_system_handler.h>
using namespace ourTypes;

namespace vid_mapper{
  //Return Error Codes
  #define MAP_SUCCESS           0x00
  #define MAPFILE_Full          0xFE
  #define UNDEFINED_ERROR       0xFF

  //VID defined values
  #define MVID_MAP_ERROR        0xFF

  class Vid_mapper{
    private:
      //Variables
      File _map_file;
      vid *_current_vid;
      uint8_t _mvid;
      //Functions
      stdRetVal search_for_current_vid();
      stdRetVal create_new_map_entry();
      //Setter
      void set_map_file(File map_file);
    public:
      Vid_mapper(const vid *current_vid);
      Vid_mapper();
      stdRetVal get_map_value(uint8_t *mvid_result);
  };
};
#endif
