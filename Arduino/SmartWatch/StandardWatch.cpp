#include "Module.h"

/* ################################################################################### 
 *  Standard Watch
 ################################################################################### */

StandardWatch::StandardWatch(uint8_t row, uint8_t column):Module(row, column) {
}

StandardWatch::~StandardWatch() {

}

void StandardWatch::onSerialInput(char *inp) {

}

void StandardWatch::onTimeInterrupt() {

}

void StandardWatch::onButtonInput(int buttonId) {

}

void StandardWatch::updateDisplay() {

}

void StandardWatch::onIteration() {
    
}
