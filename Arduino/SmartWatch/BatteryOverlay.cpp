#include "Module.h"

BatteryOverlay::BatteryOverlay(State *state, uint8_t row, uint8_t column):Module(state, row, column) {
  BatteryOverlay::batteryLevel = 5; // read level
}

BatteryOverlay::~BatteryOverlay() {

}

void BatteryOverlay::onSerialInput(char *inp) {

}

void BatteryOverlay::onTimeInterrupt() {

}

void BatteryOverlay::onButtonInput(int buttonId) {

}

void BatteryOverlay::updateDisplay() {

}

void BatteryOverlay::onIteration() {
    
}

void BatteryOverlay::drawBattery() {
  
}
