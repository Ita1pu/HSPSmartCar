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
