#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <persistence.h>
namespace bluetooth{

class BtUploader{
  public:
  void log_upload_postion();
  void upload_bt();
  void init(persistence::Persistence *p);
  ~BtUploader();
private:
  uint8_t _current_car;
  char _current_file[7];
  uint16_t _postition;
  persistence::Persistence *_p;
  void upload_entry();
  void delete_entry();
  void get_last_upload_position();
};
};
#endif
