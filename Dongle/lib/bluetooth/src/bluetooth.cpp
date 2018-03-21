#include <bluetooth.h>

using namespace bluetooth;
  /**
   * @brief Uploads a Single entry via bluetooth
   *
   */
  void BtUploader::upload_entry(){
    return;
  }

  /**
   * @brief Uploads a Single entry via bluetooth
   *
   */
  void BtUploader::delete_entry(){
    return;
  }

  /**
   * @brief Writes to a Log-file wich data has been uploaded already
   *
   */
  void BtUploader::log_upload_postion(){
    this->_p->log_bt_upload_position(this->_current_car, this->_current_file, this->_postition);
  }

  /**
   * @brief Uploadtask for Bluetooth
   *
   */
  void BtUploader::upload_bt(){
    return;
  }

  void BtUploader::init(persistence::Persistence *p){
    this->_p = p;
  }

  BtUploader::~BtUploader(){
    this->log_upload_postion();
  }

  void BtUploader::read_upload_size_and_date(){
    uint8_t i = 0;
    uint8_t in[IN_BUFFER] = {0};

    //Recive Request like: "##<size>"
    Serial.readBytes(in, IN_BUFFER);

    while(i < IN_BUFFER){
      if(in[i] == '#' and in[i+1] == '#'){
        Serial.print("##");
        Serial.println(in[i+2]);
        upload_size = in[i+2];
        break;
      }
      i++;
    }
    in[IN_BUFFER] = {0}; //Clear buffer
    delay(MAX_RESPONSE_DELAY);
    Serial.readBytes(in, IN_BUFFER);//Read date like ;;20101999

    while ( i < IN_BUFFER){
      if(in[i] == ';' and in[i+1] == ';'){
        if (i <= IN_BUFFER - 10){
          this->date[0] = in[i+2];
          this->date[1] = in[i+3];
          this->date[2] = in[i+4];
          this->date[3] = in[i+5];
          this->date[4] = in[i+6];
          this->date[5] = in[i+7];
          this->date[6] = in[i+8];
          this->date[7] = in[i+9];
          this->date[8] = 0;
          Serial.print(";;");
          Serial.println(this->date);
          break;
        }
        else{
          Serial.println("No date received starting with oldest one");
        }
      }
      i++;
    }
  }
