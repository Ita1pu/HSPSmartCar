#ifndef MEMSSENSOR_H
#define MEMSSENSOR_H

#include <stdint.h>

//Magnetometer Registers
#define AK8963_ADDRESS   0x0C
#define WHO_AM_I_AK8963  0x00 // should return 0x48
#define INFO             0x01
#define AK8963_ST1       0x02  // data ready status bit 0
#define AK8963_XOUT_L    0x03  // data
#define AK8963_XOUT_H    0x04
#define AK8963_YOUT_L    0x05
#define AK8963_YOUT_H    0x06
#define AK8963_ZOUT_L    0x07
#define AK8963_ZOUT_H    0x08
#define AK8963_ST2       0x09  // Data overflow bit 3 and data read error status bit 2
#define AK8963_CNTL      0x0A  // Power down (0000), single-measurement (0001), self-test (1000) and Fuse ROM (1111) modes on bits 3:0
#define AK8963_CNTL2      0x0B
#define AK8963_ASTC      0x0C  // Self test control
#define AK8963_I2CDIS    0x0F  // I2C disable
#define AK8963_ASAX      0x10  // Fuse ROM x-axis sensitivity adjustment value
#define AK8963_ASAY      0x11  // Fuse ROM y-axis sensitivity adjustment value
#define AK8963_ASAZ      0x12  // Fuse ROM z-axis sensitivity adjustment value

#define SELF_TEST_X_GYRO 0x00
#define SELF_TEST_Y_GYRO 0x01
#define SELF_TEST_Z_GYRO 0x02

#define SELF_TEST_X_ACCEL 0x0D
#define SELF_TEST_Y_ACCEL 0x0E
#define SELF_TEST_Z_ACCEL 0x0F

#define SELF_TEST_A       0x10

#define XG_OFFSET_H       0x13  // User-defined trim values for gyroscope
#define XG_OFFSET_L       0x14
#define YG_OFFSET_H       0x15
#define YG_OFFSET_L       0x16
#define ZG_OFFSET_H       0x17
#define ZG_OFFSET_L       0x18
#define SMPLRT_DIV        0x19
#define CONFIG            0x1A
#define GYRO_CONFIG       0x1B
#define ACCEL_CONFIG      0x1C
#define ACCEL_CONFIG2     0x1D
#define LP_ACCEL_ODR      0x1E
#define WOM_THR           0x1F

// Duration counter threshold for motion interrupt generation, 1 kHz rate,
// LSB = 1 ms
#define MOT_DUR           0x20
// Zero-motion detection threshold bits [7:0]
#define ZMOT_THR          0x21
// Duration counter threshold for zero motion interrupt generation, 16 Hz rate,
// LSB = 64 ms
#define ZRMOT_DUR         0x22

#define FIFO_EN            0x23
#define I2C_MST_CTRL       0x24
#define I2C_SLV0_ADDR      0x25
#define I2C_SLV0_REG       0x26
#define I2C_SLV0_CTRL      0x27
#define I2C_SLV1_ADDR      0x28
#define I2C_SLV1_REG       0x29
#define I2C_SLV1_CTRL      0x2A
#define I2C_SLV2_ADDR      0x2B
#define I2C_SLV2_REG       0x2C
#define I2C_SLV2_CTRL      0x2D
#define I2C_SLV3_ADDR      0x2E
#define I2C_SLV3_REG       0x2F
#define I2C_SLV3_CTRL      0x30
#define I2C_SLV4_ADDR      0x31
#define I2C_SLV4_REG       0x32
#define I2C_SLV4_DO        0x33
#define I2C_SLV4_CTRL      0x34
#define I2C_SLV4_DI        0x35
#define I2C_MST_STATUS     0x36
#define INT_PIN_CFG        0x37
#define INT_ENABLE         0x38
#define DMP_INT_STATUS     0x39  // Check DMP interrupt
#define INT_STATUS         0x3A
#define ACCEL_XOUT_H       0x3B
#define ACCEL_XOUT_L       0x3C
#define ACCEL_YOUT_H       0x3D
#define ACCEL_YOUT_L       0x3E
#define ACCEL_ZOUT_H       0x3F
#define ACCEL_ZOUT_L       0x40
#define TEMP_OUT_H         0x41
#define TEMP_OUT_L         0x42
#define GYRO_XOUT_H        0x43
#define GYRO_XOUT_L        0x44
#define GYRO_YOUT_H        0x45
#define GYRO_YOUT_L        0x46
#define GYRO_ZOUT_H        0x47
#define GYRO_ZOUT_L        0x48
#define EXT_SENS_DATA_00   0x49
#define EXT_SENS_DATA_01   0x4A
#define EXT_SENS_DATA_02   0x4B
#define EXT_SENS_DATA_03   0x4C
#define EXT_SENS_DATA_04   0x4D
#define EXT_SENS_DATA_05   0x4E
#define EXT_SENS_DATA_06   0x4F
#define EXT_SENS_DATA_07   0x50
#define EXT_SENS_DATA_08   0x51
#define EXT_SENS_DATA_09   0x52
#define EXT_SENS_DATA_10   0x53
#define EXT_SENS_DATA_11   0x54
#define EXT_SENS_DATA_12   0x55
#define EXT_SENS_DATA_13   0x56
#define EXT_SENS_DATA_14   0x57
#define EXT_SENS_DATA_15   0x58
#define EXT_SENS_DATA_16   0x59
#define EXT_SENS_DATA_17   0x5A
#define EXT_SENS_DATA_18   0x5B
#define EXT_SENS_DATA_19   0x5C
#define EXT_SENS_DATA_20   0x5D
#define EXT_SENS_DATA_21   0x5E
#define EXT_SENS_DATA_22   0x5F
#define EXT_SENS_DATA_23   0x60
#define MOT_DETECT_STATUS  0x61
#define I2C_SLV0_DO        0x63
#define I2C_SLV1_DO        0x64
#define I2C_SLV2_DO        0x65
#define I2C_SLV3_DO        0x66
#define I2C_MST_DELAY_CTRL 0x67
#define SIGNAL_PATH_RESET  0x68
#define MOT_DETECT_CTRL    0x69
#define USER_CTRL          0x6A  // Bit 7 enable DMP, bit 3 reset DMP
#define PWR_MGMT_1         0x6B // Device defaults to the SLEEP mode
#define PWR_MGMT_2         0x6C
#define DMP_BANK           0x6D  // Activates a specific bank in the DMP
#define DMP_RW_PNT         0x6E  // Set read/write pointer to a specific start address in specified DMP bank
#define DMP_REG            0x6F  // Register in DMP from which to read or to which to write
#define DMP_REG_1          0x70
#define DMP_REG_2          0x71
#define FIFO_COUNTH        0x72
#define FIFO_COUNTL        0x73
#define FIFO_R_W           0x74
#define WHO_AM_I_MPU9250   0x75 // Should return 0x71
#define XA_OFFSET_H        0x77
#define XA_OFFSET_L        0x78
#define YA_OFFSET_H        0x7A
#define YA_OFFSET_L        0x7B
#define ZA_OFFSET_H        0x7D
#define ZA_OFFSET_L        0x7E

#define MPU9250_ADDRESS 0x68  // Device address when ADO = 0
#define AK8963_ADDRESS  0x0C   // Address of magnetometer

class MemsSensor{
public:
  /**
   * Initialization Method Should be done at startup
   * @return Wether the initialization was successfull
   */
  bool memsInit();

  /**
   * Deinitialization Method Should be done when sleeping
   * @return always true
   */
  bool memsDeinit();

  /**
   * Getter for the Sensor data of all modules
   * The Acceleration values give the data in the range of +-2g; 1.5 means an acceleration of 1.5 g => 14,71 m/s²
   * The Gyroscope values give the data in the range of +-250 degrees per second; 140 means a rotation of 140 dps
   * The Magnetometer values give the data in the range of +- 4912 µTesla; 11.1 means a magnetic flux of 11.1 µTesla
   * @param accelValues The pointer to a float array with 3 elements where the acceleration data can be stored; can be 0;
   * @param gyroValues The pointer to a float array with 3 elements where the rotation data can be stored; can be 0;
   * @param magValues The pointer to a float array with 3 elements where the magnet data can be stored; can be 0;
   * @return always true
   */
  bool memsRead(float* accelValues, float* gyroValues, float* magValues);

  /**
   * Set offset Values in gyroscope
   * The values must be given in degrees per second. The size of the array will not be checked.
   * @param offsetVals The pointer to a float array with 3 elements in order [x-Axis, y-Axis, z-Axis]
   */
  void offsetGyro(float* offsetVals);
protected:
private:
  /**
   * Initialization Method of Acc- and Gyro-Sensor and should be done in memsInit
   * @return always true
   */
  bool initMPU9250();
  /**
   * Initialization Method of Magnetometer and should be done in memsInit
   * @return always true
   */
  bool initAK8963();
  /**
   * Deinitialization Method of Acc- and Gyro-Sensor and should be done in memsDeinit
   */
  void deinitMPU9250();
  /**
   * Deinitialization Method of Magnetometer and should be done in memsDeinit
   */
  void deinitAK8963();
  /**
   * Write 8 Bits into register of Acc- and Gyro-Sensor
   * @param subAddress The registeraddress where to be written
   * @param data The data to be written
   */
  void writeByte(uint8_t subAddress, uint8_t data);
  /**
   * Read 1 Byte from register of Acc- and Gyro-Sensor
   * @param subAddress The registeraddress to be read
   * @return The read value
   */
  uint8_t readByte(uint8_t subAddress);
  /**
   * Read n Bytes from Register of Acc- and Gyro-Sensor
   * The registeraddresses will be incremented
   * @param subAddress The registeraddress where to start
   * @param count The number of Bytes to be ready
   * @param data The pointer to an array where the read data is stored; The size will not be checked
   */
  void readBytes(uint8_t subAddress, uint8_t count, uint8_t *data);
  /**
   * Write 8 Bits into register of Magnetometer
   * @param subAddress The registeraddress where to be written
   * @param data The data to be written
   */
  void writeByteAK(uint8_t subAddress, uint8_t data);
  /**
   * Read 1 Byte from register of Magnetometer
   * @param subAddress The Registeraddress to be read
   * @return The read value
   */
  uint8_t readByteAK(uint8_t subAddress);
  /**
   * Read n Bytes from register of Magnetometer
   * The registeraddresses will be incremented
   * @param subAddress The registeraddress where to start
   * @param count The number of Bytes to be ready
   * @param data The pointer to an array where the read data is stored; The size will not be checked
   */
  void readBytesAK(uint8_t subAddress, uint8_t count, uint8_t *data);
};

#endif
