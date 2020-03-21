#include <stdint.h>
class Module;

/** 
 * Keeps track of the system's state as a whole.
 * Also provides API for setting overlays and apps.
*/
class State
{
public:
    Module **overlays;
    Module *currentApp;
    uint8_t overlayCount;
    
    State(uint8_t oc)
    {
        this->overlayCount = overlayCount;
        overlays = new Module *[overlayCount];
    }
};

/**
 * This class is a pure abstract class which must be overriden by any new
 * module (app or overlay) that is to be added to the SmartWatch
*/
class Module
{
public:
    const char *icon;
    State *state;
    uint8_t startRow, startColumn;
    Module(State *state, uint8_t row, uint8_t column)
    {
        this->state = state;
        this->startRow = row;
        this->startColumn = column;
    }

    // Required to endure that the destructor of base class is called when delete is called on base class pointer,
    // specially in this case where base class in abstract and hence at runtime, you must tell the system that the
    // derived class has a deallocator.
    virtual ~Module() {
        
    }

    /**
     * Called when some data has been collected on the serial interface from the main system loop.
     * You can call new Serial functions from here.
     * @param inp input string from Serial port
    */
    virtual void onSerialInput(char *inp) = 0;

    /**
     * Called when minute interrupt has been received. You probably want to update your UI based on time.
     * DO NOT use Serial operations here.
    */
    virtual void onTimeInterrupt() = 0;

    /**
     * Called when a button has been pressed by user. You get the button id as the only parameter.
     * @param buttonId int Id of the button that was pressed.
    */
    virtual void onButtonInput(int buttonId) = 0;

    /** 
     * Makes changes to the display by actually drawing elements
    */
    virtual void updateDisplay() = 0;

    /** 
     * I/O or other operations that must be done repeatedly should be specified here.
     * This function is called in the Arduino loop() method.
    */
    virtual void onIteration() = 0;
};

/**
 * Declare the headers for you app here in the form of a class.
 * Remember, it must override the {@link Module} abstract class.
 * */

/** 
 * Shows time in top left corner always, regardless of the app in focus.
*/
class TimeOverlay : public Module {
    public:
    uint8_t batteryLevel; // x/5
    TimeOverlay(State *state, uint8_t row, uint8_t column);
    ~TimeOverlay();
    void onSerialInput(char *inp);
    void onTimeInterrupt();
    void onButtonInput(int buttonId);
    void updateDisplay();
    void onIteration();
    void drawBattery();
};

/** 
 * Shows battery bar in top right corner always, regardless of the app in focus.
*/
class BatteryOverlay : public Module {
    public:
    BatteryOverlay(State *state, uint8_t row, uint8_t column);
    ~BatteryOverlay();
    void onSerialInput(char *inp);
    void onTimeInterrupt();
    void onButtonInput(int buttonId);
    void updateDisplay();
    void onIteration();
};
