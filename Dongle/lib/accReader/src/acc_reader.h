#ifndef ACC_READER_H
#define ACC_READER_H

#include "memsSensor.h"

/**
 *  Intermediate Class for the Acceleration Sensor;
 *  This class provides methods to get the Information about Position relative
 *  to Ground and Acceleration.
 */
class AccReader{
public:
  /**
   * Initialization Method Should be done at startup
   * @return Wether the initialization was successfull
   */
  bool Initialize();

  /**
   * Getter for the acceleration data of one of the 3 axes
   * @param  ct The number (0, 1 or 2) of the axis which is wanted
   * @return The measured Data of the axis in g
   */
  float GetAccelerationAxis(uint8_t ct);

  /**
   * Getter for the total acceleration
   * @return The magnitude of the acceleration vector in g
   */
   float GetAccelerationMag();

  /**
   * Getter for the gyroscope data of one of the 3 axes
   * @param ct The number (0, 1 or 2) of the axis which is wanted
   * @return The measured Data of the gyroscope-axis im degree per second
   */
  float GetAngle(uint8_t ct);

  /**
   * Getter for the magnetometer data of one of the 3 axes
   * @param ct The number (0, 1 or 2) of the axis which is wanted
   * @return The measured Data of the magnetometer-axis in µTesla
   */
  float GetMagnet(uint8_t ct);
private:
  bool _isFirst;
  bool _isInit;
  MemsSensor _sensor;
};

#endif
