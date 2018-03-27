#include <bluetooth.h>

using namespace bluetooth;
  /**
   * @brief Uploads a Single entry via bluetooth
   *
   */
  uint8_t BtUploader::upload_entry(){
    uint8_t entry[SIZE_OF_LOGGING_ENTRY];
    uint8_t i = 0;
    if(this->_p->get_next_entry(&this->_position, &this->_p->getFile_System()->getCurrentFile(), entry) == 0){
      for(i = 0; i < SIZE_OF_LOGGING_ENTRY;i++){
        Serial.print(entry[i]);
      }
      Serial.print("\n");
      return NO_ERROR;
    }
    else{
      return UNDEFINED_ERROR;
    }
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
  void BtUploader::log_upload_position(){
    this->_p->log_bt_upload_position(this->_current_car, this->_current_file, this->_position);
  }

  /**
   * @brief Uploadtask for Bluetooth
   *
   */
  void BtUploader::upload_bt(){
    uint8_t uploaded = 0;
    while(uploaded < this->upload_size && this->upload_entry() == 0){
      uploaded++;
    }

    Serial.print(uploaded);
    Serial.println(" entries uploaded");
    return;
  }

  void BtUploader::init(persistence::Persistence *p){
    this->_p = p;
  }

  BtUploader::~BtUploader(){
    this->log_upload_position();
  }

  void BtUploader::read_upload_size_and_date(){
    this->upload_size = 0;
    uint8_t i = 0;
    uint8_t in[IN_BUFFER_SIZE] = {0};
    this->upload_file_date[0] = '0';
    this->upload_file_date[1] = '/';
    this->upload_file_date[2] = '1';
    this->upload_file_date[3] = '1';
    this->upload_file_date[4] = '0';
    this->upload_file_date[5] = '1';
    this->upload_file_date[6] = '1';
    this->upload_file_date[7] = '9';
    this->upload_file_date[8] = '.';
    this->upload_file_date[9] = 'L';
    this->upload_file_date[10] = 'O';
    this->upload_file_date[11] = 'G';
    this->upload_file_date[12] = 0;

    //Recive Request like: "##<size>"
    Serial.readBytes(in, IN_BUFFER_SIZE);


    while(i < IN_BUFFER_SIZE){
      if(in[i] == '#' and in[i+1] == '#'){
        Serial.print("##");
        Serial.println(in[i+2]);
        this->upload_size = in[i+2];
        break;
      }
      i++;
    }
    in[IN_BUFFER_SIZE] = {0}; //Clear buffer
    delay(MAX_RESPONSE_DELAY);
    Serial.readBytes(in, IN_BUFFER_SIZE);//Read date like ;;20101999

    while ( i < IN_BUFFER_SIZE){
      if(in[i] == ';' and in[i+1] == ';'){
        if (i <= IN_BUFFER_SIZE - 10){
          this->upload_file_date[2] = in[i+2];
          this->upload_file_date[3] = in[i+3];
          this->upload_file_date[4] = in[i+4];
          this->upload_file_date[5] = in[i+5];
          this->upload_file_date[6] = in[i+6];
          this->upload_file_date[7] = in[i+7];
          Serial.print(";;");
          break;
        }
      }
      i++;
    }
    this->_p->getFile_System()->open_file((char*)this->upload_file_date, 'r');
    this->_current_car = 0;
  }
