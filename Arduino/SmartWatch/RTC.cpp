#include "RTC.h"
#include "Module.h"

// Private methods
void RTC::writeTo(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(RTC_ADDR);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
  delay(10);
}

uint8_t RTC::readFrom(uint8_t reg) {
  Wire.beginTransmission(RTC_ADDR);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom(RTC_ADDR, 1);
  return Wire.read();
}

// Public methods
void RTC::init() {

  RTC::readTime();
  // Enable and minute alarm
  RTC::enableMinuteAlarm();
  // Configure AM2 to alert every minute
  RTC::configureMinuteAlarm();
  // Reset once
  RTC::resetMinuteAlarm();

}

void RTC::readTime() {
  Wire.beginTransmission(RTC_ADDR);
  Wire.write(TIME_START_ADDR);
  Wire.endTransmission();
  Wire.requestFrom(RTC_ADDR, 7);  // 7 registers

  if (Wire.available() >= 7) {
    uint8_t seconds = Wire.read(); // Fake it. Maybe use in future
    State::minutes = Wire.read();
    State::hour = Wire.read();
    State::dayOfWeek = Wire.read();
    State::date = Wire.read();
    State::month = Wire.read() & MONTH_MASK;
    State::year = Wire.read();
  }
  delay(10);
}

void RTC::setTime() {
  Wire.beginTransmission(RTC_ADDR);     // Start I2C protocol with DS3231 address
  Wire.write(TIME_START_ADDR);                              // Send register address
  Wire.write(0);                              // Reset seconds and start oscillator
  Wire.write(State::minutes);                         // Write minute
  Wire.write(State::hour);                           // Write hour
  Wire.write(State::dayOfWeek);                    // Write day
  Wire.write(State::date);                           // Write date
  Wire.write(State::month);                          // Write month
  Wire.write(State::year);                           // Write year
  Wire.endTransmission();                     // Stop transmission and release the I2C bus
  delay(10);
}

void RTC::configureMinuteAlarm() {
  uint8_t a2m2, a2m3, a2m4;

  Wire.beginTransmission(RTC_ADDR);
  Wire.write(AM2_ADDR);
  Wire.write(0b10000000);
  Wire.write(0b10000000);
  Wire.write(0b10000000);
  Wire.endTransmission();
  delay(10);

}

void RTC::resetMinuteAlarm() {
  uint8_t statusReg = RTC::readFrom(STATUS_ADDR);
  statusReg &= STATUS_REG_RESET_AM2;
  RTC::writeTo(STATUS_ADDR, statusReg);
}

void RTC::enableMinuteAlarm() {
  // Enable AM2 and INTCN
  uint8_t control = RTC::readFrom(CONTROL_ADDR);
  control |= CONTROL_REG_ENABLE_AM2;
  RTC::writeTo(CONTROL_ADDR, control);
}

bool RTC::isMinuteAlarm() {
  uint8_t statusReg = RTC::readFrom(STATUS_ADDR);
  if (statusReg & (~STATUS_REG_RESET_AM2) == STATUS_REG_IS_AM2) {
    return true;
  }
  else {
    return false;
  }
}

// TODO : after implementing Flash
void RTC::setUserAlarm(bool repeating, uint8_t hours, uint8_t mins) {

}
