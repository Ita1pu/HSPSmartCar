#ifndef GPS_H
#define GPS_H

#include <stdint.h>
#include "FreematicsONE.h"

#define GPS_INIT_CMD "Test"

class LocationService{
public:
  LocationService(COBDSPI* coProc);

  /**
   * Method to initialize
   * A positive return value means western longitude and a negative value means eastern longitude
   * @param the baud rate the uart communication to the gps module shall use
   * @return wether the initialization was successfull
   */
  bool Initialize(unsigned long baud);

  /**
   * Method to get the longitude of the device's current position
   * A positive return value means western longitude and a negative value means eastern longitude
   * @return longitude in the following format: Location in degree * 1000000; e.g. 48215559 => 48,215559°
   */
  int32_t GetLongitude();

  /**
   * Method to get the latitude of the device's current position
   * A positive return value means northern latitude and a negative value means southern latitude
   * @return latitude in the following format: Location in degree * 1000000; e.g. 48215559 => 48,215559°
   */
  int32_t GetLatitude();

  /**
   * Method return the number of used satellites
   * @return number of satellites used to locate vehicle
   */
   uint8_t GetSat();
  /**
   * Method return the initialization status of the gps module
   * @return Answers the question if the gps module is initialized
   */
  bool IsInitialized();

  /**
   * Method to renew the GPS information available by the get-methods
   */
  bool RenewGPSData();
protected:
private:
  COBDSPI* _coProc;
  bool _isInit;
  GPS_DATA _gData;
};

#endif
