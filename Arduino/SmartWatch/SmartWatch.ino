#include "Module.h"

#define BUTTON_INTERRUPT_PIN 2
#define TIME_INTERRUPT_PIN 3

char *serialInputBuffer;

int count = 0;

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  OLED::init();
  OLED::clearDisplay();
  
  State::init( 2, STANDARD_WATCH_APP); // Initialize with 2 overlays, get time once and set the first app to display
  State::overlays[0] = new TimeOverlay(0, 2);
  State::overlays[1] = new BatteryOverlay(0, 110);

  pinMode(BUTTON_INTERRUPT_PIN, INPUT_PULLUP);
  pinMode(TIME_INTERRUPT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_INTERRUPT_PIN), onButtonInterrupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(TIME_INTERRUPT_PIN), onButtonInterrupt, FALLING);
  

  for (uint8_t i = 0; i < State::overlayCount; i++)
  {
    State::overlays[i]->updateDisplay();
  }

}

void loop()
{
  if (Serial.available())
  {
      String input = Serial.readString();
      serialInputBuffer = new char[input.length()];
      input.toCharArray(serialInputBuffer, input.length());
      for (uint8_t i = 0; i < State::overlayCount; i++)
      {
          State::overlays[i]->onSerialInput(serialInputBuffer);
      }
      State::currentApp->onSerialInput(serialInputBuffer);
  }
  for (uint8_t i = 0; i < State::overlayCount; i++)
  {
      State::overlays[i]->onIteration();
  }
//  State::currentApp->onIteration();
}

/**
   On interrupt, checks the button pins to check which button was pressed
*/
void onButtonInterrupt()
{
  for (uint8_t i = 0; i < State::overlayCount; i++)
  {
    State::overlays[i]->onButtonInput(getButtonPressed());
  }
  State::currentApp->onButtonInput(getButtonPressed());
}

/**
   On interrupt, updates time in state and notifies every overlay and current app
*/
void onTimeInterrupt()
{
  State::updateTime();
  for (uint8_t i = 0; i < State::overlayCount; i++)
  {
    State::overlays[i]->onTimeInterrupt();
  }
  
  State::currentApp->onTimeInterrupt();
}

/**
   Returns id of button pressed.
*/
uint8_t getButtonPressed()
{
  return 0;
}
