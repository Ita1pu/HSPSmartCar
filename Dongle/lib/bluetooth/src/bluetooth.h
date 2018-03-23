#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <persistence.h>
namespace bluetooth{
#define IN_BUFFER_SIZE        20
#define MAX_RESPONSE_DELAY    200
#define SIZE_OF_DATE_STRING   9       //0/28102018\0

class BtUploader{
  public:
  void log_upload_position();
  void upload_bt();
  void init(persistence::Persistence *p);
  void read_upload_size_and_date();
  char upload_file_date[SIZE_OF_DATE_STRING+4] = "01011999.LOG";
  uint8_t upload_size = 0;
  ~BtUploader();
private:
  uint8_t _current_car;
  char _current_file[7];
  uint16_t _position;
  persistence::Persistence *_p;
  uint8_t upload_entry();
  void delete_entry();
  void get_last_upload_position();
};
};
#endif
