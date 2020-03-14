/**
 * This class is a pure abstract class which must be overriden by any new
 * module that is to be added to the SmartWatch
*/
class Module {
    public:

    /**
     * Called when some data has been collected on the serial interface from the main system loop.
     * You can call new Serial functions from here.
    */
    virtual void onSerialInput(char* inp) = 0;
    
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
};