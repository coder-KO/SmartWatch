#include <Arduino.h>
#include <Wire.h>

#define RTC_ADDR 0x68

#define AM1_ADDR 0x07
#define AM2_ADDR 0x0B
#define CONTROL_ADDR 0x0E
#define STATUS_ADDR 0x0F
#define TIME_START_ADDR 0x00

#define CONTROL_REG_ENABLE_AM1 0b00000101 // Use with | operator
#define CONTROL_REG_ENABLE_AM2 0b00000110 // Use with | operator

#define STATUS_REG_RESET_AM1 0b11111110 // Use with & operator
#define STATUS_REG_RESET_AM2 0b11111101 // Use with & operator
#define STATUS_REG_IS_AM2 0b00000010 // Use with & operator

//#define HOUR_FORMAT_12 0b11111111 // Use with | operator
#define HOUR_FORMAT_24 0b00111111 // Use with | operator

#define NUMBERS_DISPLAY_FONT_OFFSET 48
#define LOWER_NIBBLE_EXTRACT 0b00001111 // use with &

#define MONTH_MASK 0b01111111
#define TIME_FORMAT_MASK 0b00000100
#define SINGLE_HOUR_BIT_MASK 0b00000001
#define IS_24_HOUR 0b00000000
#define IS_12_HOUR 0b00000100
#define AM_PM_MASK 0b00000010
#define IS_AM 0b00000000
#define IS_PM 0b00000010

class RTC {
  private:
  
  /**
   * Writes 1 byte only to a register address
   * @param reg Address of register to write to
   * @param value Byte to write to the register
   */
  static void writeTo(uint8_t reg, uint8_t value);

  /**
   * Reads 1 byte only from given address
   * @param reg Address of register to read from
   */
  static uint8_t readFrom(uint8_t reg);
  
  public:
  /**
   * Enables minute alarm
   */
  static void init();

  /**
   * Reads time from hardware directly into the State class
   */
  static void readTime();

  /**
   * Writes current time from state module to the hardware
   */
  static void setTime();

  /**
   * Sets AM2 of DS3231 to give interrupt every minute. Hence AM2 should not be used for user alarms.
   */
  static void configureMinuteAlarm();

  /**
   * Resets the minute alarm only (AM2 @ every minute)
   */
  static void resetMinuteAlarm();

  /**
   * Enables the minute alarm (AM2 @ every minute)
   */
   static void enableMinuteAlarm();

   /**
    * Checks RTC registers to determine if the current alarm fired was a minute alarm.
    * Should be used to differentiate between a user set alarm or minute alarm
    */
   static bool isMinuteAlarm();

   /**
    * Sets user defined alarm. To be implemented after flash memory is added.
    */
   static void setUserAlarm(bool repeating, uint8_t hours, uint8_t mins);
};
