#include <vid_mapper.h>
using namespace vid_mapper;

void Vid_mapper::set_map_file(File map_file)
{
  this->_map_file = map_file;
}

stdRetVal Vid_mapper::search_for_current_vid()
{
  this->_mvid = MVID_MAP_ERROR;
  return UNDEFINED_ERROR;
}

stdRetVal Vid_mapper::create_new_map_entry()
{
  this->_mvid = MVID_MAP_ERROR;
  return UNDEFINED_ERROR;
}

Vid_mapper::Vid_mapper(const vid *current_vid)
{
  this->_current_vid = current_vid;
}

stdRetVal Vid_mapper::get_map_value(uint8_t *mvid_result)
{
  uint8_t map_result = this->search_for_current_vid();
  if (map_result != NO_ERROR)
  {// VID has not been mapped in mapfile
    map_result = this->create_new_map_entry();
  }
  *mvid_result = this->_mvid;
  return map_result;
}
