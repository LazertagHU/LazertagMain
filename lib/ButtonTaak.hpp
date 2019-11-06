#ifndef _BUTTONTAAK_HPP
#define _BUTTONTAAK_HPP
#include "hwlib.hpp"
#include "rtos.hpp"
#include "KeypadListener.hpp"

class buttonTaak : public rtos::task<>{
private:

    rtos::clock                                         buttonClock;
    keypadListener                                      *keypadlistener;
    bool                                                buttonPressed = false;
    bool                                                reloadPressed = false;

    /// \brief
    /// Main() for the ButtonTask
    /// \details
    /// This task's job is to notify listening task of changes to the button. 
    /// When it reads a input, the KeyPressed(char) function of the listening class is called
    void main() override;

public:

    /// \brief
    /// Constructor for ButtonTaak
    /// \details
    /// This constructor names it's task, starts it's clock and binds the listener.
    buttonTaak(
        const char * name, 
        keypadListener *keypadlistener
    ):
        task(3, name),
        buttonClock(this, 100'000, "buttonClock"),
        keypadlistener(keypadlistener)
    {}
    
};





#endif