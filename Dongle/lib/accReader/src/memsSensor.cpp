#include "memsSensor.h"
#include <Arduino.h>
#include <Wire.h>

bool MemsSensor::memsInit(){
  bool retVal = false;
  Wire.begin();
  Wire.setClock(400000);

  byte c = readByte(WHO_AM_I_MPU9250);  // Read WHO_AM_I register for MPU-9250
  if (c == 0x68 || c == 0x71){
    retVal = initMPU9250();
    if(retVal){
      retVal = initAK8963();
    }
  }
  return retVal;
}

bool MemsSensor::memsRead(float *accelValues, float *gyroValues, float *magValues){
  uint8_t rawData[7];   // x/y/z register data stored here
  int16_t accCtr[3];    //x/y/z acceleration counter value stored here
  int16_t gyrCtr[3];    //x/y/z gyroscope counter value stored here
  int16_t magCtr[3];    //x/y/z magnetometer counter value stored here

  if(accelValues!= 0){
    readBytes(ACCEL_XOUT_H, 6, rawData);  // Read the six acceleration raw data registers into data array
    accCtr[0] = ((int16_t)rawData[0] << 8) | rawData[1] ;  // Turn the MSB and LSB into a signed 16-bit value
    accCtr[1] = ((int16_t)rawData[2] << 8) | rawData[3] ;
    accCtr[2] = ((int16_t)rawData[4] << 8) | rawData[5] ;
  }

  if(gyroValues !=0){
    readBytes(GYRO_XOUT_H, 6, rawData);
    gyrCtr[0] = ((int16_t)rawData[0] << 8) | rawData[1] ;  // Turn the MSB and LSB into a signed 16-bit value
    gyrCtr[1] = ((int16_t)rawData[2] << 8) | rawData[3] ;
    gyrCtr[2] = ((int16_t)rawData[4] << 8) | rawData[5] ;
  }

  if(magValues != 0 && readByteAK(AK8963_ST1) & 0x01) { // wait for magnetometer data ready bit to be set
    readBytesAK(AK8963_XOUT_L, 7, rawData);  // Read the six raw data and ST2 registers sequentially into data array
    uint8_t c = rawData[6]; // End data read by reading ST2 register
    if(!(c & 0x08)) { // Check if magnetic sensor overflow set, if not then report data
      magCtr[0] = ((int16_t)rawData[1] << 8) | rawData[0] ;  // Turn the MSB and LSB into a signed 16-bit value
      magCtr[1] = ((int16_t)rawData[3] << 8) | rawData[2] ;  // Data stored as little Endian
      magCtr[2] = ((int16_t)rawData[5] << 8) | rawData[4] ;
   }
  }

  //convert counter values to real Values by multiplying with factor
  for(int i = 0; i < 3; ++i){
    if(accelValues!= 0)
      accelValues[i] = (float)accCtr[i] * 2.0f / 32768.0f; // 2 because of +-2g Measurement Range; 32768 because of 16-bit with of Counter

    if(gyroValues!= 0)
      gyroValues[i] = (float)gyrCtr[i] * 250.0f / 32768.0f; // 250 because of +-250dps Measurement Range; 32768 because of 16-bit with of counter

    if(magValues!= 0)
      magValues[i] = (float)magCtr[i] * 4912.0f / 32760.0f; // 4912 because of +-4912µTesla Measurement Range; 32760 because of maximum value
                                                        //see Register Description of MPU-9250 page 50
  }

  return true;
}

bool MemsSensor::memsDeinit(){
  //deinit Sensors
  deinitAK8963();
  deinitMPU9250();
  //end I²C
  Wire.end();

  return true;
}

bool MemsSensor::initAK8963(){
  if (readByteAK(WHO_AM_I_AK8963) != 0x48) {
      return false;
  }
  writeByte(AK8963_CNTL, 0x00); // Power down magnetometer
  delay(10);
  // Configure the magnetometer for continuous read and highest resolution
  // set Mscale bit 4 to 1 (0) to enable 16 (14) bit resolution in CNTL register,
  // and enable continuous mode data acquisition Mmode (bits [3:0]), 0010 for 8 Hz and 0110 for 100 Hz sample rates
  writeByte(AK8963_CNTL, 0x1 << 4 | 0x02); // Set magnetometer data resolution and sample ODR
  delay(10);
  return true;
}

bool MemsSensor::initMPU9250(){
  // wake up device
   writeByte(PWR_MGMT_1, 0x00); // Clear sleep mode bit (6), enable all sensors
   delay(100); // Wait for all registers to reset

  // get stable time source
   writeByte(PWR_MGMT_1, 0x01);  // Auto select clock source to be PLL gyroscope reference if ready else
   delay(200);

  // Configure Gyro and Thermometer
  // Disable FSYNC and set thermometer and gyro bandwidth to 41 and 42 Hz, respectively;
  // minimum delay time for this setting is 5.9 ms, which means sensor fusion update rates cannot
  // be higher than 1 / 0.0059 = 170 Hz
  // DLPF_CFG = bits 2:0 = 011; this limits the sample rate to 1000 Hz for both
  // With the MPU9250, it is possible to get gyro sample rates of 32 kHz (!), 8 kHz, or 1 kHz
   writeByte(CONFIG, 0x03);

  // Set sample rate = gyroscope output rate/(1 + SMPLRT_DIV)
   writeByte(SMPLRT_DIV, 0x04);  // Use a 200 Hz rate; a rate consistent with the filter update rate
                                     // determined inset in CONFIG above

  // Set gyroscope full scale range
  // Range selects FS_SEL and AFS_SEL are 0 - 3, so 2-bit values are left-shifted into positions 4:3
   uint8_t c = readByte(GYRO_CONFIG); // get current GYRO_CONFIG register value
  // c = c & ~0xE0; // Clear self-test bits [7:5]
   c = c & ~0x02; // Clear Fchoice bits [1:0]
   c = c & ~0x18; // Clear AFS bits [4:3]
   c = c | 0x0 << 3; // Set full 250dps range for the gyro
  // c =| 0x00; // Set Fchoice for the gyro to 11 by writing its inverse to bits 1:0 of GYRO_CONFIG
   writeByte(GYRO_CONFIG, c ); // Write new GYRO_CONFIG value to register

  // Set accelerometer full-scale range configuration
   c = readByte(ACCEL_CONFIG); // get current ACCEL_CONFIG register value
  // c = c & ~0xE0; // Clear self-test bits [7:5]
   c = c & ~0x18;  // Clear AFS bits [4:3]
   c = c | 0x0 << 3; // Set full +-2g range for the accelerometer
   writeByte(ACCEL_CONFIG, c); // Write new ACCEL_CONFIG register value

  // Set accelerometer sample rate configuration
  // It is possible to get a 4 kHz sample rate from the accelerometer by choosing 1 for
  // accel_fchoice_b bit [3]; in this case the bandwidth is 1.13 kHz
   c = readByte(ACCEL_CONFIG2); // get current ACCEL_CONFIG2 register value
   c = c & ~0x0F; // Clear accel_fchoice_b (bit 3) and A_DLPFG (bits [2:0])
   c = c | 0x03;  // Set accelerometer rate to 1 kHz and bandwidth to 41 Hz
   writeByte(ACCEL_CONFIG2, c); // Write new ACCEL_CONFIG2 register value
  // The accelerometer, gyro, and thermometer are set to 1 kHz sample rates,
  // but all these rates are further reduced by a factor of 5 to 200 Hz because of the SMPLRT_DIV setting

   // Configure Interrupts and Bypass Enable
   // Set interrupt pin active high, push-pull, hold interrupt pin level HIGH until interrupt cleared,
   // clear on read of INT_STATUS, and enable I2C_BYPASS_EN so additional chips
   // can join the I2C bus and all can be controlled by the Arduino as master
    writeByte(INT_PIN_CFG, 0x22);
    writeByte(INT_ENABLE, 0x01);  // Enable data ready (bit 0) interrupt
    delay(100);

  return true;
}

void MemsSensor::deinitAK8963(){
  writeByteAK(AK8963_CNTL, 0x10); //powerdown Magnetometer
  delay(10);
}
void MemsSensor::deinitMPU9250(){
  writeByte(PWR_MGMT_1, 0x40); //set to sleep mode
  delay(10);
}

void MemsSensor::writeByte(uint8_t subAddress, uint8_t data){
  Wire.beginTransmission(MPU9250_ADDRESS);  // Initialize the Tx buffer
  Wire.write(subAddress);           // Put slave register address in Tx buffer
  Wire.write(data);                 // Put data in Tx buffer
  Wire.endTransmission();           // Send the Tx buffer
}

uint8_t MemsSensor::readByte(uint8_t subAddress){
  uint8_t data = 0x0;
  Wire.beginTransmission(MPU9250_ADDRESS);         // Initialize the Tx buffer
  Wire.write(subAddress);                  // Put slave register address in Tx buffer
  Wire.endTransmission(false);             // Send the Tx buffer, but send a restart to keep connection alive
  Wire.requestFrom((uint8_t)MPU9250_ADDRESS, (uint8_t) 1);  // Read one byte from slave register address
  data = Wire.read();                      // Fill Rx buffer with result
  return data;
}

void MemsSensor::readBytes(uint8_t subAddress, uint8_t count, uint8_t *data){
  Wire.beginTransmission(MPU9250_ADDRESS);   // Initialize the Tx buffer
  Wire.write(subAddress);            // Put slave register address in Tx buffer
  Wire.endTransmission(false);       // Send the Tx buffer, but send a restart to keep connection alive
  Wire.requestFrom((uint8_t)MPU9250_ADDRESS, count);  // Read bytes from slave register address
  uint8_t i = 0;
  while (Wire.available() && i < count) {
        data[i++] = Wire.read();
  }
}

void MemsSensor::writeByteAK(uint8_t subAddress, uint8_t data){
  Wire.beginTransmission(AK8963_ADDRESS);  // Initialize the Tx buffer
  Wire.write(subAddress);           // Put slave register address in Tx buffer
  Wire.write(data);                 // Put data in Tx buffer
  Wire.endTransmission();           // Send the Tx buffer
}

uint8_t MemsSensor::readByteAK(uint8_t subAddress){
  uint8_t data = 0x0;
  Wire.beginTransmission(AK8963_ADDRESS);
  Wire.write(subAddress);
  Wire.endTransmission(false);
  Wire.requestFrom((uint8_t)AK8963_ADDRESS, (uint8_t)1);
  data = Wire.read();
  return data;
}

void MemsSensor::readBytesAK(uint8_t subAddress, uint8_t count, uint8_t *data){
  Wire.beginTransmission(AK8963_ADDRESS);   // Initialize the Tx buffer
  Wire.write(subAddress);            // Put slave register address in Tx buffer
  Wire.endTransmission(false);       // Send the Tx buffer, but send a restart to keep connection alive
  Wire.requestFrom((uint8_t)AK8963_ADDRESS, count);  // Read bytes from slave register address
  uint8_t i = 0;
  while (Wire.available() && i < count) {
        data[i++] = Wire.read();
  }
}
