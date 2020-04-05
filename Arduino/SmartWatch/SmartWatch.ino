#include "Module.h"

#define BUTTON_INTERRUPT_PIN 2
#define TIME_INTERRUPT_PIN 3

State state(2); // Initialize with overlay count
char *serialInputBuffer;

int count = 0;

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  OLED::init();
  OLED::clearDisplay();

  pinMode(BUTTON_INTERRUPT_PIN, INPUT_PULLUP);
  pinMode(TIME_INTERRUPT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_INTERRUPT_PIN), onButtonInterrupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(TIME_INTERRUPT_PIN), onButtonInterrupt, FALLING);
  
  // TODO : Add overlays and set currentApp
  state.overlays[0] = new TimeOverlay(&state, 0, 2);
  state.overlays[1] = new BatteryOverlay(&state, 0, 110);

  state.updateTime();

  for (uint8_t i = 0; i < state.overlayCount; i++)
  {
    state.overlays[i]->updateDisplay();
  }

}

void loop()
{
  if (Serial.available())
  {
      String input = Serial.readString();
      serialInputBuffer = new char[input.length()];
      input.toCharArray(serialInputBuffer, input.length());
      for (uint8_t i = 0; i < state.overlayCount; i++)
      {
          state.overlays[i]->onSerialInput(serialInputBuffer);
      }
      state.currentApp->onSerialInput(serialInputBuffer);
  }
  for (uint8_t i = 0; i < state.overlayCount; i++)
  {
      state.overlays[i]->onIteration();
  }
//  state.currentApp->onIteration();
}

/**
   On interrupt, checks the button pins to check which button was pressed
*/
void onButtonInterrupt()
{
  for (uint8_t i = 0; i < state.overlayCount; i++)
  {
    state.overlays[i]->onButtonInput(getButtonPressed());
  }
  state.currentApp->onButtonInput(getButtonPressed());
}

/**
   On interrupt, updates time in state and notifies every overlay and current app
*/
void onTimeInterrupt()
{
  state.updateTime();
  for (uint8_t i = 0; i < state.overlayCount; i++)
  {
    state.overlays[i]->onTimeInterrupt();
  }
  
  state.currentApp->onTimeInterrupt();
}

/**
   Returns id of button pressed.
*/
uint8_t getButtonPressed()
{
  return 0;
}
