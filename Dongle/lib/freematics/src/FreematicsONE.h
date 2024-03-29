/*************************************************************************
* Arduino Library for Freematics ONE/ONE+
* Distributed under BSD license
* Visit http://freematics.com/products/freematics-one for more information
* (C)2012-2017 Stanley Huang <support@freematics.com.au
*************************************************************************/

#ifndef FREEMATICSONE_H
#define FREEMATICSONE_H

#include "FreematicsBase.h"
#include "FreematicsMEMS.h"
#include "FreematicsDMP.h"
#include "FreematicsSD.h"
#ifdef ESP32
#include "FreematicsPlus.h"
#endif

#ifndef ARDUINO_ARCH_AVR
#define sprintf_P sprintf
#endif

#define OBD_TIMEOUT_SHORT 2000 /* ms */
#define OBD_TIMEOUT_LONG 10000 /* ms */
#define OBD_SERIAL_BAUDRATE 38400

#ifdef ARDUINO_ARCH_AVR
#define SPI_PIN_CS 7
#define SPI_PIN_READY 6
#else
#define SPI_PIN_CS 2
#define SPI_PIN_READY 13
#endif

#define TARGET_OBD 0
#define TARGET_GPS 1
#define TARGET_BEE 2
#define TARGET_RAW 3

class COBDSPI : public virtual CFreematics {
public:
	COBDSPI():dataMode(1),errors(0),m_state(OBD_DISCONNECTED) {}
	byte begin();
  // initialize OBD-II connection
	bool init(OBD_PROTOCOLS protocol = PROTO_AUTO);
	void uninit();
	void reset();
	// un-initialize OBD-II connection
	void end();
	// set SPI data target
	void setTarget(byte target) { m_target = target; }
	// receive data (up to 255 bytes) from SPI bus
	int receive(char* buffer, int bufsize, unsigned int timeout = OBD_TIMEOUT_SHORT);
	// read specified OBD-II PID value
	bool readPID(byte pid, int& result);
	// read multiple (up to 4) OBD-II PID value
	byte readPID(const byte pid[], byte count, int result[]);
	// write data to SPI bus
	void write(const char* s);
	void write(byte* data, int len);
	// send AT command and receive response
	byte sendCommand(const char* cmd, char* buf, byte bufsize, unsigned int timeout = OBD_TIMEOUT_LONG);
	// initialize GPS (set baudrate to 0 to power off GPS)
	bool gpsInit(unsigned long baudrate = 115200L);
	// get parsed GPS data
	bool gpsGetData(GPS_DATA* gdata);
	// get GPS NMEA data
	int gpsGetRawData(char* buf, int bufsize);
	// send command string to GPS
	void gpsSendCommand(const char* cmd);
	// whether internal GPS is present
	bool internalGPS() { return m_internalGPS; }
	// start xBee UART communication
	bool xbBegin(unsigned long baudrate = 115200L);
	// read data to xBee UART
	int xbRead(char* buffer, int bufsize, unsigned int timeout = 1000);
	// send string data to xBee UART
	void xbWrite(const char* data);
  // send data to xBee UART
	void xbWrite(const char* data, int len);
	// receive data from xBee UART (returns 0/1/2)
	virtual int xbReceive(char* buffer, int bufsize, unsigned int timeout = 1000, const char** expected = 0, byte expectedCount = 0);
	// purge xBee UART buffer
	void xbPurge();
	// toggle xBee module power
	void xbTogglePower();
	// get connection state
	OBD_STATES getState() { return m_state; }
	// hardware sleep (timer counter will stop for AVR)
	void sleepSec(unsigned int seconds);
	// delay specified number of ms
	void sleep(unsigned int ms);
	// enter low power mode
	void enterLowPowerMode();
	// leave low power mode
	void leaveLowPowerMode();
	// read diagnostic trouble codes (return number of DTCs read)
	byte readDTC(uint16_t codes[], byte maxCodes = 1);
	// clear diagnostic trouble code
	void clearDTC();
	// get battery voltage (works without ECU)
	float getVoltage();
	// get VIN as a string, buffer length should be >= OBD_RECV_BUF_SIZE
	bool getVIN(char* buffer, byte bufsize);
	// send query for specified PID
	void sendQuery(byte pid);
	// determine if the PID is supported
	bool isValidPID(byte pid);
	// set current PID mode
	byte dataMode;
	// occurrence of errors
	byte errors;
	// bit map of supported PIDs
	byte pidmap[4 * 4];
protected:
	void debugOutput(const char* s);
	int normalizeData(byte pid, char* data);
	OBD_STATES m_state;
private:
	// get firmware version
	byte getVersion();
	// PID data conversion functions
	uint8_t getPercentageValue(char* data)
	{
		return (uint16_t)hex2uint8(data) * 100 / 255;
	}
	uint16_t getLargeValue(char* data)
	{
		return hex2uint16(data);
	}
	uint8_t getSmallValue(char* data)
	{
		return hex2uint8(data);
	}
	int16_t getTemperatureValue(char* data)
	{
		return (int)hex2uint8(data) - 40;
	}
	byte checkErrorMessage(const char* buffer);
	byte m_target;
	bool m_internalGPS;
#ifdef ESP32
	bool m_newGPSData;
#endif
};

#endif
