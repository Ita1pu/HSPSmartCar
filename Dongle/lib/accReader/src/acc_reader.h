#ifndef ACC_READER_H
#define ACC_READER_H

#include <FreematicsMEMS.h>

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
   * @return The measured Data of the axis
   */
  float GetAcceleration(uint8_t ct);

  /**
   * Getter for the gyroscope data of one of the 3 axes
   * @param ct The number (0, 1 or 2) of the axis which is wanted
   * @return The measured Data of the gyroscope-axis
   */
  float GetAngle(uint8_t ct);

  /**
   * Getter for the collective Sensordata as a quarternion
   * @param pointer to the float-array where to store the data;
   * The Array must be at least 9 elements big. The size will not be checked.
   * @return Boolean value if the operation was successfull and
   * the quarternion is stored as following:
   * quart[0] = Accel. x-Axis
   * quart[1] = Accel. y-Axis
   * quart[2] = Accel. z-Axis
   * quart[3] = Gyro. x-Axis
   * quart[4] = Gyro. y-Axis
   * quart[5] = Gyro. z-Axis
   * quart[6] = Magnet. x-Axis
   * quart[7] = Magnet. y-Axis
   * quart[8] = Magnet. z-Axis
   */
  bool GetQuarternion(float* qart);
private:
  bool _isFirst;
  bool _isInit;
  int8_t accCorrFactor;
  MPU9250_9DOF _sensor;
};

#endif
