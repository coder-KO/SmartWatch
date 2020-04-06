#include "Module.h"

// Cause, well, C++
Module **State::overlays = new Module *[1];
Module *State::currentApp = new StandardWatch( 1, 20);
uint8_t State::overlayCount = 0;

uint8_t State::minutes = 0, State::hour = 0, State::dayOfWeek = 0, State::date = 0, State::month = 0, State::year = 0;

void State::init(uint8_t oc, uint8_t currentAppId) {
  updateTime();
  State::overlayCount = oc;
  
  delete State::overlays;
  State::overlays = NULL;
  State::overlays = new Module *[State::overlayCount];
  
  delete State::currentApp;
  State::currentApp = NULL;
  State::currentApp = State::getAppById(currentAppId);
}

void State::updateTime() {
  
}

Module* State::getAppById(uint8_t id) {
  switch(id) {
    case STANDARD_WATCH_APP:
         return new StandardWatch( 2, 10);
  }
}
