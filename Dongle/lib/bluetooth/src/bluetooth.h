#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <persistence.h>
namespace bluetooth{
#define IN_BUFFER             10
#define MAX_RESPONSE_DELAY    1000
#define SIZE_OF_DATE_STRING   9       //28102018\0

class BtUploader{
  public:
  void log_upload_postion();
  void upload_bt();
  void init(persistence::Persistence *p);
  void read_upload_size_and_date();
  char date[SIZE_OF_DATE_STRING] = "";
  uint8_t upload_size = 0;
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
