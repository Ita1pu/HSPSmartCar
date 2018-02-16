#ifndef VID_MAPPER
#define VID_MAPPER

#include <stdint.h>
#include <file_system_handler.h>
#include <persistence_error_codes.h>

using namespace ourTypes;

namespace persistence{
  class Vid_mapper{
    private:
      //Variables
      File _map_file;
      const vid *_current_vid;
      uint8_t _mvid;
      //Functions
      persistence::stdRetVal search_for_current_vid();
      persistence::stdRetVal create_new_map_entry();
      //Setter
      void set_map_file(File map_file);
    public:
      void initialize(const vid *current_vid);
      persistence::stdRetVal get_map_value(uint8_t *mvid_result);
  };
};
#endif
