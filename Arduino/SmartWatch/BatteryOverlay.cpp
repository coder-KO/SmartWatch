#include "Module.h"

/* ################################################################################### 
 *  Battery Overlay
 ################################################################################### */
// Set to occupy row 0, column 114 onwards - implemented to consume 7 columns - 1 start, 1 end, 5 levels

BatteryOverlay::BatteryOverlay(uint8_t row, uint8_t column):Module(row, column) {
  this->batteryLevel = readBatteryLevel(); // read level
}

BatteryOverlay::~BatteryOverlay() {

}

void BatteryOverlay::onSerialInput(char *inp) {

}

void BatteryOverlay::onTimeInterrupt() {

}

void BatteryOverlay::onButtonInput(int buttonId) {

}

/**
 * Forms display buffer (an array of chars) to represent a battery.
 */
void BatteryOverlay::updateDisplay() {
  
  OLED::clearDisplayAt(this->startRow, this->startColumn, 2);
  
  uint8_t bufferSize = (this->maxBatteryLevel * 2) + 8;
  char displayBuffer[bufferSize];

  // Tail of battery
  displayBuffer[0] = pgm_read_word_near(battery + 0);
  displayBuffer[1] = pgm_read_word_near(battery + 2);

  // Head of battery
  displayBuffer[bufferSize - 1] = '\0';
  displayBuffer[bufferSize - 2] = pgm_read_word_near(battery + 3);
  displayBuffer[bufferSize - 3] = pgm_read_word_near(battery + 3);
  displayBuffer[bufferSize - 4] = pgm_read_word_near(battery + 3);
  displayBuffer[bufferSize - 5] = pgm_read_word_near(battery + 0);
  displayBuffer[bufferSize - 6] = pgm_read_word_near(battery + 2);
  
  
  
  // Filled out bars
  for (uint8_t i = 1; i <= (this-> batteryLevel * 2); i += 2) {
    displayBuffer[i + 1] = pgm_read_word_near(battery + 1);
    displayBuffer[i + 2] = pgm_read_word_near(battery + 1);
  }

  // Un filled bars
  for (uint8_t i = (this->batteryLevel * 2) + 1; i < (this-> maxBatteryLevel * 2); i += 2) {
    displayBuffer[i + 1] = pgm_read_word_near(battery + 2);
    displayBuffer[i + 2] = pgm_read_word_near(battery + 2);
  }

  OLED::writeDisplayByte(displayBuffer, 1, this->startRow, this->startColumn);
  
}

void BatteryOverlay::onIteration() {
    
}

/**
 * Reads voltage using ADC. Return battery level after consulting Battery's datasheet
 */
uint8_t BatteryOverlay::readBatteryLevel() {
  return 1;
}
