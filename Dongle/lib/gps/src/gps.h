#ifndef GPS_H
#define GPS_H

#include <stdint.h>
#include "FreematicsONE.h"

#define GPS_TEST_CMD "Test"

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
   * Measured average time: 1 µSecond.
   * @return longitude in the following format: Location in degree * 1000000; e.g. 48215559 => 48,215559°
   */
  int32_t GetLongitude();

  /**
   * Method to get the latitude of the device's current position
   * A positive return value means northern latitude and a negative value means southern latitude
   * Measured average time: 1 µSecond.
   * @return latitude in the following format: Location in degree * 1000000; e.g. 48215559 => 48,215559°
   */
  int32_t GetLatitude();

  /**
   * Method return the number of used satellites
   * Measured average time: 1 µSecond.
   * @return number of satellites used to locate vehicle
   */
   uint8_t GetSat();
  /**
   * Method return the initialization status of the gps module
   * Measured average time: 1 µSecond.
   * @return Answers the question if the gps module is initialized
   */
  bool IsInitialized();

  /**
   * Method to renew the GPS information available by the get-methods
   * Measured average time: 39.4 Milliseconds; WCET: 259 Milliseconds
   */
  bool RenewGPSData();
protected:
private:
  COBDSPI* _coProc;
  bool _isInit;
  GPS_DATA _gData;
};

#endif
