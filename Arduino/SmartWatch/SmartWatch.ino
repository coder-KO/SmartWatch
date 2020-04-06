#include "Module.h"
#include "RTC.h"

char *serialInputBuffer;
bool buttonInterrupt, timeInterrupt;

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  OLED::init();
  OLED::clearDisplay();

  State::init( 2, STANDARD_WATCH_APP); // Initialize with 2 overlays, get time once and set the first app to display
  State::overlays[0] = new TimeOverlay(0, 0);
  State::overlays[1] = new BatteryOverlay(0, 110);

  delay(1000);
  RTC::init();

  pinMode(TIME_INTERRUPT_PIN, INPUT);
  pinMode(BUTTON_INTERRUPT_PIN, INPUT);

  for (uint8_t i = 0; i < State::overlayCount; i++)
  {
    State::overlays[i]->updateDisplay();
  }
  State::currentApp->updateDisplay();

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
  State::currentApp->onIteration();

  // Servicing events
  if (digitalRead(TIME_INTERRUPT_PIN) == LOW) {
    
    if (1) {
      
      RTC::readTime();
      RTC::resetMinuteAlarm();
      
      for (uint8_t i = 0; i < State::overlayCount; i++)
      {
        State::overlays[i]->onTimeInterrupt();
      }
      
      State::currentApp->onTimeInterrupt();
      Serial.println("Time interrupt");
      
    }
    timeInterrupt = false;
  }

  if (digitalRead(TIME_INTERRUPT_PIN) == LOW) {
    buttonInterrupt = false;
  }
}
