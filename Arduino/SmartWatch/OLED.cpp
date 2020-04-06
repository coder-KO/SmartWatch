#include "OLED.h"

void OLED::init() {

  // Set the I2C to HS mode - 400KHz!
  // TWBR = (CPU_CLK / I2C_CLK) -16 /2
  // TWBR = ((16,000,000 / 400,000) - 16) / 2 = 12
//  TWBR = 12;

  // keywords:
  // SEG = COL = segment = column byte data on a page
  // Page = 8 pixel tall row. Has 128 SEGs and 8 COMs
  // COM = row

  // Begin the I2C comm with SSD1306's address (SLA+Write)
  Wire.beginTransmission(OLED_I2C_ADDRESS);

  // Tell the SSD1306 that a command stream is incoming
  Wire.write(OLED_CONTROL_BYTE_CMD_STREAM);

  // Follow instructions on pg.64 of the dataSheet for software configuration of the SSD1306
  // Turn the Display OFF
  Wire.write(OLED_CMD_DISPLAY_OFF);
  // Set mux ration tp select max number of rows - 64
  Wire.write(OLED_CMD_SET_MUX_RATIO);
  Wire.write(0x3F);
  // Set the display offset to 0
  Wire.write(OLED_CMD_SET_DISPLAY_OFFSET);
  Wire.write(0x00);
  // Display start line to 0
  Wire.write(OLED_CMD_SET_DISPLAY_START_LINE);

  // Mirror the x-axis. In case you set it up such that the pins are north.
  //     Wire.write(0xA0); - //in case pins are south - default
  Wire.write(OLED_CMD_SET_SEGMENT_REMAP);

  // Mirror the y-axis. In case you set it up such that the pins are north.
  Wire.write(0xC0); //- in case pins are south - default
  //     Wire.write(OLED_CMD_SET_COM_SCAN_MODE);

  // Default - alternate COM pin map
  Wire.write(OLED_CMD_SET_COM_PIN_MAP);
  Wire.write(0x12);
  // set contrast
  Wire.write(OLED_CMD_SET_CONTRAST);
  Wire.write(0x7F);
  // Set display to enable rendering from GDDRAM (Graphic Display Data RAM)
  Wire.write(OLED_CMD_DISPLAY_RAM);
  // Normal mode!
  Wire.write(OLED_CMD_DISPLAY_NORMAL);
  // Default oscillator clock
  Wire.write(OLED_CMD_SET_DISPLAY_CLK_DIV);
  Wire.write(0x80);
  // Enable the charge pump
  Wire.write(OLED_CMD_SET_CHARGE_PUMP);
  Wire.write(0x14);
  // Set precharge cycles to high cap type
  Wire.write(OLED_CMD_SET_PRECHARGE);
  Wire.write(0x22);
  // Set the V_COMH deselect volatage to max
  Wire.write(OLED_CMD_SET_VCOMH_DESELCT);
  Wire.write(0x30);
  // Horizonatal addressing mode - same as the KS108 GLCD
  Wire.write(OLED_CMD_SET_MEMORY_ADDR_MODE);
  Wire.write(0x01);

  // Turn the Display ON
  Wire.write(OLED_CMD_DISPLAY_ON);

  // End the I2C comm with the SSD1306
  Wire.endTransmission();

  OLED::clearDisplay();

}

void OLED::writeString(char* str, int scaleFactor, int row, int column) {
  int length = strlen(str), index;
  uint8_t rowMax, columnMax;
  rowMax = row + scaleFactor - 1;
  columnMax = column + (length * scaleFactor * 8);
//  rowMax += uint8_t(columnMax / 127);

  setCursor(row, rowMax, column, columnMax);

  uint8_t currentByte;
  uint8_t* scaledBytes;
  for (int i = 0; i < length; i++) {
    index = str[i] * 8;
    // we have 8 byte fonts
    for (int j = 0; j < 8; j++) {
      // Keep the wire interaction inside this loop, i.e. 1 transaction for 1 character otherwise display gets messed up
      currentByte = pgm_read_word_near(font + j + index);
      scaledBytes = scale(currentByte, scaleFactor);
      Wire.beginTransmission(OLED_I2C_ADDRESS);
      Wire.write(OLED_CONTROL_BYTE_DATA_STREAM);
      // Working with verticle addressing mode
      for (int y = 0; y < scaleFactor; y++) {
        for (int x = scaleFactor - 1; x >= 0; x--) {
          Wire.write(scaledBytes[x]);
        }
      }
      Wire.endTransmission();
      delay(10);
    }
  }
}

void OLED::writeDisplayByte(uint8_t* str, int scaleFactor, int row, int column) {
  int length = strlen(str);
  setCursor(row, row + scaleFactor - 1, column, column + (length * scaleFactor) );

  uint8_t* scaledBytes;
  for (int i = 0; i < length; i++) {
    // Keep the wire interaction inside this loop, i.e. 1 transaction for 1 character otherwise display gets messed up
    scaledBytes = scale(str[i], scaleFactor);
    Wire.beginTransmission(OLED_I2C_ADDRESS);
    Wire.write(OLED_CONTROL_BYTE_DATA_STREAM);
    // Working with verticle addressing mode
    for (int y = 0; y < scaleFactor; y++) {
      for (int x = scaleFactor - 1; x >= 0; x--) {
        Wire.write(scaledBytes[x]);
      }
    }
    Wire.endTransmission();
    delay(10);
  }
}

void OLED::clearDisplay() {
  // Set the GDDRAM to (Row0, Col0), ie: top-left and establish range as the whole screen - 128x64
  Wire.beginTransmission(OLED_I2C_ADDRESS);
  Wire.write(OLED_CONTROL_BYTE_CMD_STREAM);
  // column 0 to 127
  Wire.write(OLED_CMD_SET_COLUMN_RANGE);
  Wire.write(0x00);
  Wire.write(127);
  // page 0 to 7
  Wire.write(OLED_CMD_SET_PAGE_RANGE);
  Wire.write(0x00);
  Wire.write(0x07);
  Wire.endTransmission();
  delay(10);
  for (uint8_t x = 0; x < 64; x++)
  {
    Wire.beginTransmission(OLED_I2C_ADDRESS);
    Wire.write(OLED_CONTROL_BYTE_DATA_STREAM);
    for (uint8_t i = 0; i < 16; i++) {
      // Serial.println("Writing : 1");
      Wire.write(0);
    }
    Wire.endTransmission();
    delay(10);
  }
}

void OLED::clearDisplayAt(uint8_t row, uint8_t column, uint8_t count) {
  // Set the GDDRAM to (Row0, Col0), ie: top-left and establish range as the whole screen - 128x64
  Wire.beginTransmission(OLED_I2C_ADDRESS);
  Wire.write(OLED_CONTROL_BYTE_CMD_STREAM);
  // column 0 to 127
  Wire.write(OLED_CMD_SET_COLUMN_RANGE);
  Wire.write(column);
  Wire.write(column + count);
  // page 0 to 7
  Wire.write(OLED_CMD_SET_PAGE_RANGE);
  Wire.write(7 - row);
  Wire.write(7 - row);
  Wire.endTransmission();

  for (uint8_t i = 0; i < count; i++) {
    Wire.beginTransmission(OLED_I2C_ADDRESS);
    Wire.write(OLED_CONTROL_BYTE_DATA_STREAM);
    for (uint8_t i = 0; i < 16; i++) {
      // Serial.println("Writing : 1");
      Wire.write(0);
    }
    Wire.endTransmission();
    delay(10);
  }
}

void OLED::setCursor(int rowStart, int rowEnd, int columnStart, int columnEnd) {
  Wire.beginTransmission(OLED_I2C_ADDRESS);
  Wire.write(OLED_CONTROL_BYTE_CMD_STREAM);
  // column 0 to 127
  Wire.write(OLED_CMD_SET_COLUMN_RANGE);
  Wire.write(columnStart); // Start column
  Wire.write(columnEnd);
  // page 0 to 7
  Wire.write(OLED_CMD_SET_PAGE_RANGE);
  // Cause we inverted the x axis
  Wire.write(7 - rowStart - (rowEnd - rowStart));
  Wire.write(7 - rowStart); // Start row

  Wire.endTransmission();
  delay(10);
}

uint8_t* OLED::scale(uint8_t inp, int scale) {
  uint8_t* toReturn = new uint8_t[scale];
  memset( toReturn, 0 , sizeof(toReturn));

  uint8_t currentByte = 0, bitPoint = 0;
  for (int i = 0; i < 8; i++) {
    uint8_t temp = inp & ANDER;
    temp = temp >> bitPoint;
    for (int j = 0; j < scale; j++) {
      toReturn[currentByte] |= temp;
      temp = temp >> 1;
      bitPoint ++;

      if (bitPoint > 7) {
        bitPoint = 0;
        currentByte++;
        temp = inp & ANDER;
      }
    }
    inp = inp << 1;
  }
  return toReturn;
}
