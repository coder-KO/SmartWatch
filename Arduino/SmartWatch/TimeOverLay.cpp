#include "Module.h"
#include <avr/pgmspace.h>

/* ################################################################################### 
 *  Time Overlay
 ################################################################################### */

TimeOverlay::TimeOverlay(uint8_t row, uint8_t column):Module(row, column) {
}

TimeOverlay::~TimeOverlay() {

}

void TimeOverlay::onSerialInput(char *inp) {

}

void TimeOverlay::onTimeInterrupt() {

}

void TimeOverlay::onButtonInput(int buttonId) {

}

void TimeOverlay::updateDisplay() {

}

void TimeOverlay::onIteration() {
    
}
