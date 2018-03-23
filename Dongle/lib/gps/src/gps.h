#ifndef GPS_H
#define GPS_H

#include <stdint.h>
#include "FreematicsONE.h"

extern uint8_t timerFlags;
extern uint16_t clockDeviation;

namespace gps{
/**
 *  Location and Time Class;
 *  This class provides methods for operations which have to do sth with location or time.
 *  E. g. setting timers, get the Time of day from GPS or location from GPS.
 *  Attention: Timer 1 is used to set the timerFlag and Timer 2 is used to update the clockDeviation
 */
 class LocationTimeService{
  public:
   LocationTimeService(COBDSPI* coProc);

   /**
    * Method to initialize
    * Initializes gps communication and starts Timer 2 to measure deviation since last gps update.
    * @param the baud rate the uart communication to the gps module shall use
    * @return wether the initialization was successfull
    */
   bool Initialize(unsigned long baud);

   /**
    * Method to uninitialize
    * Powers down the gps receiver
    */
   void UnInit();

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
    * Method return altitude above mean sea level
    * Measured average time: 1 µSecond.
    * @return meters above mean sea level
    */
   uint16_t GetAltitude();
   /**
    * Method return the initialization status of the gps module
    * Measured average time: 1 µSecond.
    * @return Answers the question if the gps module is initialized
    */
   bool IsInitialized();
    /**
     * Receive the UTC time of day;
     * Measured average time: 40 µSeconds
     * @return Time of day as integer in following Format: HHMMSSmmm
     */
   uint32_t GetTime();

    /**
     * Receive the UTC _date
     * Measured average time: 1 µSecond
     * @return Date of the current day in following Format: DDMMYY
     */
    uint32_t GetDate();

    /**
     * Receive the UTC time in ms since 1.1.1970
     * Measured average time: 617 µSecond
     * @return milliseconds since 1.1.1970
     */
   uint64_t GetEpochMs();
   /**
    * Receive the Flag of Timer
    * @return pointer to timer flag
    */
   uint8_t* GetFlag();
    /**
     * Start the timer which sets the timerFlags
     * @return Wether the setup was successfull
     */
   bool StartFlagTimer();
   /**
    * Start the timer which sets the timerFlags
    */
   void StopFlagTimer();
    /**
     * Method to renew the GPS information available by the get-methods
     * Measured average time: 39.4 Milliseconds; WCET: 80 Milliseconds
     */
   bool RenewGPSData();
  protected:
  private:
    COBDSPI* _coProc;
    bool _isInit;
    GPS_DATA _gData;
  };
}
#endif
