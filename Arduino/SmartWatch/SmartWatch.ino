// Must be called at beginning
#include <Wire.h>
#include "StandardFont.h"

#define OLED_I2C_ADDRESS   0x3C
#define ANDER 128

// The SSD1306 datasheet (pg.20) says that a control byte has to be sent before sending a command
// Control byte consists of 
// bit 7		: Co   : Continuation bit - If 0, then it assumes all the next bytes are data (no more control bytes).
//				:		 You can send a stream of data, ie: gRAM dump - if Co=0
//				:        For Command, you'd prolly wanna set this - one at a time. Hence, Co=1 for commands
//				:		 For Data stream, Co=0 :)
// bit 6    	: D/C# : Data/Command Selection bit, Data=1/Command=0
// bit [5-0] 	: lower 6 bits have to be 0
#define OLED_CONTROL_BYTE_CMD_SINGLE	0x80
#define OLED_CONTROL_BYTE_CMD_STREAM	0x00
#define OLED_CONTROL_BYTE_DATA_STREAM	0x40

// Fundamental commands (pg.28)
#define OLED_CMD_SET_CONTRAST			0x81	// follow with 0x7F
#define OLED_CMD_DISPLAY_RAM			0xA4
#define OLED_CMD_DISPLAY_ALLON			0xA5
#define OLED_CMD_DISPLAY_NORMAL			0xA6
#define OLED_CMD_DISPLAY_INVERTED 		0xA7
#define OLED_CMD_DISPLAY_OFF			0xAE
#define OLED_CMD_DISPLAY_ON				0xAF

// Addressing Command Table (pg.30)
#define OLED_CMD_SET_MEMORY_ADDR_MODE	0x20	// follow with 0x00 = HORZ mode = Behave like a KS108 graphic LCD
#define OLED_CMD_SET_COLUMN_RANGE		0x21	// can be used only in HORZ/VERT mode - follow with 0x00 + 0x7F = COL127
#define OLED_CMD_SET_PAGE_RANGE			0x22	// can be used only in HORZ/VERT mode - follow with 0x00 + 0x07 = PAGE7

// Hardware Config (pg.31)
#define OLED_CMD_SET_DISPLAY_START_LINE	0x40
#define OLED_CMD_SET_SEGMENT_REMAP		0xA1	
#define OLED_CMD_SET_MUX_RATIO			0xA8	// follow with 0x3F = 64 MUX
#define OLED_CMD_SET_COM_SCAN_MODE		0xC8	
#define OLED_CMD_SET_DISPLAY_OFFSET		0xD3	// follow with 0x00
#define OLED_CMD_SET_COM_PIN_MAP		0xDA	// follow with 0x12
#define OLED_CMD_NOP 					0xE3	// NOP

// Timing and Driving Scheme (pg.32)
#define OLED_CMD_SET_DISPLAY_CLK_DIV	0xD5	// follow with 0x80
#define OLED_CMD_SET_PRECHARGE			0xD9	// follow with 0xF1
#define OLED_CMD_SET_VCOMH_DESELCT		0xDB	// follow with 0x30

// Charge Pump (pg.62)
#define OLED_CMD_SET_CHARGE_PUMP        0x8D    // follow with 0x14

void loop() {

}



void setup() {
    // Init the I2C interface (pins A4 and A5 on the Arduino Uno board) in Master Mode.
    Wire.begin();

    // Set the I2C to HS mode - 400KHz!
    // TWBR = (CPU_CLK / I2C_CLK) -16 /2
    // TWBR = ((16,000,000 / 400,000) - 16) / 2 = 12
    // TWBR=12;

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
    // Wire.write(0xA0); - //in case pins are south - default
    Wire.write(OLED_CMD_SET_SEGMENT_REMAP);
        
    // Mirror the y-axis. In case you set it up such that the pins are north.
    Wire.write(0xC0); //- in case pins are south - default
    // Wire.write(OLED_CMD_SET_COM_SCAN_MODE);
        
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


    // Begin writing data on screen
    // setCursor( 0, 8, 0, 128);
    clearDisplay();

    char* str = "Hi meme";
    writeString(str, 1, 3, 10);
    delay(1000);
    clearDisplay();
    writeString(str, 2, 3, 10);
    
}

void writeString(char* str, int scaleFactor, int row, int column) {
    int length = strlen(str), index;
    setCursor(row, row + scaleFactor, column, column + (length * scaleFactor * 8) );
    
    uint8_t currentByte;
    uint8_t* scaledBytes;
    for (int i = 0; i < length; i++) {
        index = str[i] * 8;
        // we have 8 byte fonts
        for (int j = 0; j < 8; j++) {
            // Keep the wire interaction inside this loop, i.e. 1 transaction for 1 character otherwise display gets messed up
            Wire.beginTransmission(OLED_I2C_ADDRESS);
            Wire.write(OLED_CONTROL_BYTE_DATA_STREAM);
            currentByte = pgm_read_word_near(font + j + index);
            scaledBytes = scale(currentByte, scaleFactor);
            // Working with verticle addressing mode
            for (int y = 0; y < scaleFactor; y++) {
                for (int x = scaleFactor - 1; x >= 0; x--) {
                    Wire.write(scaledBytes[x]);
                }
            }
            Wire.endTransmission();
        }
    }
}

void clearDisplay() {
    // Set the GDDRAM to (Row0, Col0), ie: top-left and establish range as the whole screen - 128x64
    Wire.beginTransmission(OLED_I2C_ADDRESS);
    Wire.write(OLED_CONTROL_BYTE_CMD_STREAM);
    // column 0 to 127
    Wire.write(OLED_CMD_SET_COLUMN_RANGE);
    Wire.write(0x00);		
    Wire.write(0x7F);
    // page 0 to 7
    Wire.write(OLED_CMD_SET_PAGE_RANGE);
    Wire.write(0x00);
    Wire.write(0x07);
    Wire.endTransmission();

    for (uint8_t x = 0; x < 7; x++)
    {
        for (uint8_t j = 0; j < 8; j++)
        {
            Wire.beginTransmission(OLED_I2C_ADDRESS);
            Wire.write(OLED_CONTROL_BYTE_DATA_STREAM);
            for(uint8_t i=0;i<16;i++){
                // Serial.println("Writing : 1");
                Wire.write(0);
            }
            Wire.endTransmission();
        }  
    }
}

void setCursor(int rowStart, int rowEnd, int columnStart, int columnEnd) {
    Wire.beginTransmission(OLED_I2C_ADDRESS);
    Wire.write(OLED_CONTROL_BYTE_CMD_STREAM);
    // column 0 to 127
    Wire.write(OLED_CMD_SET_COLUMN_RANGE);
    Wire.write(columnStart); // Start column
    Wire.write(columnEnd);
    // page 0 to 7
    Wire.write(OLED_CMD_SET_PAGE_RANGE);
    // Cause we inverted the x axis
    Wire.write(0x07 - rowStart); // Start row
    Wire.write(rowEnd);
    Wire.endTransmission();
}

uint8_t* scale(uint8_t inp, int scale) {
    uint8_t* toReturn = new uint8_t[scale];
    memset( toReturn, 0 , sizeof(toReturn));

    uint8_t currentByte = 0, bitPoint = 0;
    for(int i = 0; i < 8; i++) {
        uint8_t temp = inp & ANDER;
        temp = temp >> bitPoint;
        for(int j = 0; j < scale; j++) {
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
