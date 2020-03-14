class Module;
class State;
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

    /** 
     * Set's the main app deleting the already existing one.
    */
    void set_current_app(Module *app)
    {
        delete currentApp;
        currentApp = app;
    }

    /** 
     * Sets the overlay at given index, deleting current if already assigned.
    */
    void set_overlay(Module *overlay, uint8_t index)
    {
        if (index >= 0 && index < overlayCount)
        {
            delete overlays[index];
            overlays[index] = overlay;
        }
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
    State &state;
    Module(State &state)
    {
        this->state = state;
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
};

/**
 * Declare the headers for you app here in the form of a class.
 * Remember, it must override the {@link Module} abstract class.
 * */
// TODO : Implement Battery indicator and time overlays