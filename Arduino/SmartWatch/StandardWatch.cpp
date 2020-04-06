#include "Module.h"
#include "RTC.h"

/* ###################################################################################
    Standard Watch
  ################################################################################### */

StandardWatch::StandardWatch(uint8_t row, uint8_t column): Module(row, column) {
}

StandardWatch::~StandardWatch() {

}

void StandardWatch::onSerialInput(char *inp) {

}

void StandardWatch::onTimeInterrupt() {
  StandardWatch::updateDisplay();
}

void StandardWatch::updateDisplay() {
  char *displayBufferTime = new char[8];
  displayBufferTime[7] = '\0';

  uint8_t datePrintOffset;
  displayBufferTime[2] = ':';
  displayBufferTime[3] = (State::minutes >> 4) + NUMBERS_DISPLAY_FONT_OFFSET;
  displayBufferTime[4] = (State::minutes & LOWER_NIBBLE_EXTRACT) + NUMBERS_DISPLAY_FONT_OFFSET;

  if ( ((State::hour >> 4) & TIME_FORMAT_MASK) == IS_24_HOUR) {
    displayBufferTime[0] = (State::hour >> 4) + NUMBERS_DISPLAY_FONT_OFFSET;
    displayBufferTime[1] = (State::hour & LOWER_NIBBLE_EXTRACT) + NUMBERS_DISPLAY_FONT_OFFSET;
    displayBufferTime[5] = '\0';
    datePrintOffset = 17;
  }

  else if ( ((State::hour >> 4) & TIME_FORMAT_MASK) == IS_12_HOUR) {
    displayBufferTime[0] = ((State::hour >> 4) & SINGLE_HOUR_BIT_MASK) + NUMBERS_DISPLAY_FONT_OFFSET;
    displayBufferTime[1] = (State::hour & LOWER_NIBBLE_EXTRACT) + NUMBERS_DISPLAY_FONT_OFFSET;
    if ((State::hour >> 4) & AM_PM_MASK == IS_AM) {
      displayBufferTime[5] = 'a';
      displayBufferTime[6] = 'm';
    }
    else if ((State::hour >> 4) & AM_PM_MASK == IS_PM) {
      displayBufferTime[5] = 'p';
      displayBufferTime[6] = 'm';
    }
    datePrintOffset = 0;
  }

  Serial.print("Set time: ");
  Serial.println(displayBufferTime);

  char *displayBufferDate = new char[10];
  displayBufferDate[9] = '\0';

  displayBufferDate[0] = (State::date >> 4) + NUMBERS_DISPLAY_FONT_OFFSET; // Get the upper nibble, then convert it to font using offset
  displayBufferDate[1] = (State::date & LOWER_NIBBLE_EXTRACT) + NUMBERS_DISPLAY_FONT_OFFSET;
  displayBufferDate[2] = ' ';

  uint8_t month = ((State::date >> 4) * 10) + (State::date & LOWER_NIBBLE_EXTRACT);

  for (uint8_t i = 0; i < 3; i++) {
    displayBufferDate[3 + i] = pgm_read_word( &monthNames[State::month - 1][i] );
  }

  displayBufferDate[6] = ' ';
  displayBufferDate[7] = (State::year >> 4) + NUMBERS_DISPLAY_FONT_OFFSET; // Get the upper nibble, then convert it to font using offset
  displayBufferDate[8] = (State::year & LOWER_NIBBLE_EXTRACT) + NUMBERS_DISPLAY_FONT_OFFSET;
  Serial.print("Set Date: ");
  Serial.println(displayBufferDate);

  OLED::writeString(displayBufferTime, 2, this->startRow, this->startColumn + datePrintOffset);
  Serial.println("Display done time");
  OLED::writeString(displayBufferDate, 1, this->startRow + 3, this->startColumn + 20);
  Serial.println("Display done date");

  delete displayBufferDate;
  displayBufferDate = NULL;
  delete displayBufferTime;
  displayBufferTime = NULL;
}

void StandardWatch::onIteration() {

}
