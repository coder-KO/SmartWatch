#include <Arduino.h>
#include "Module.h"
#include "RTC.h"

/* ###################################################################################
    Time Overlay
  ################################################################################### */

TimeOverlay::TimeOverlay(uint8_t row, uint8_t column): Module(row, column) {
}

TimeOverlay::~TimeOverlay() {

}

void TimeOverlay::onSerialInput(char *inp) {

}

void TimeOverlay::onTimeInterrupt() {
  TimeOverlay::updateDisplay();
}

void TimeOverlay::updateDisplay() {
  char *displayBuffer = new char[8];
  displayBuffer[7] = '\0';


  displayBuffer[2] = ':';
  displayBuffer[3] = (State::minutes >> 4) + NUMBERS_DISPLAY_FONT_OFFSET;
  displayBuffer[4] = (State::minutes & LOWER_NIBBLE_EXTRACT) + NUMBERS_DISPLAY_FONT_OFFSET;

  if ( ((State::hour >> 4) & TIME_FORMAT_MASK) == IS_24_HOUR) {
    displayBuffer[0] = (State::hour >> 4) + NUMBERS_DISPLAY_FONT_OFFSET;
    displayBuffer[1] = (State::hour & LOWER_NIBBLE_EXTRACT) + NUMBERS_DISPLAY_FONT_OFFSET;
    
    displayBuffer[5] = '\0';
  }

  else if ( ((State::hour >> 4) & TIME_FORMAT_MASK) == IS_12_HOUR) {
    displayBuffer[0] = ((State::hour >> 4) & SINGLE_HOUR_BIT_MASK) + NUMBERS_DISPLAY_FONT_OFFSET;
    displayBuffer[1] = (State::hour & LOWER_NIBBLE_EXTRACT) + NUMBERS_DISPLAY_FONT_OFFSET;
    if ((State::hour >> 4) & AM_PM_MASK == IS_AM) {
      displayBuffer[5] = 'a';
      displayBuffer[6] = 'm';
    }
    else if ((State::hour >> 4) & AM_PM_MASK == IS_PM) {
      displayBuffer[5] = 'p';
      displayBuffer[6] = 'm';
    }
  }

  OLED::writeString(displayBuffer, 1, this->startRow, this->startColumn);
  Serial.println(displayBuffer);
  
  delete displayBuffer;
  displayBuffer = NULL;
}

void TimeOverlay::onIteration() {

}
