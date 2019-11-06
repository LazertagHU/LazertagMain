#ifndef _INPUTCONTROLTAAK_HPP
#define _INPUTCONTROLTAAK_HPP
#include <array>
#include "rtos.hpp"
#include "KeypadListener.hpp"
#include "KeypadTaak.hpp"
#include "InputListener.hpp"
#include "ButtonTaak.hpp"


class inputControlTaak : public rtos::task<>, keypadListener{
private:
    inputListener                                       *inPutListener;
    rtos::channel<char, 18>                             inputChannel;
    char                                                key  = '\0';
    keypadTaak                                          keyPadTaak;
    buttonTaak                                          butTonTaak;

     /// \brief
    /// Main() for the Input Control task.
    /// \details
    /// this task waits on the channel for new presses, decodes them and sends them on to the listener
    void main();

public:
    /// \brief
    /// Default constructor for the InputControlTaak
    /// \details    
    /// Set task name, binds the listener, inits the channel and makes the button and keypad tasks
    inputControlTaak(int prio, inputListener *inPutListener, const char * name):
    task                (prio, name),
    inPutListener       (inPutListener),
    inputChannel        (this, "inputChannel"),
    keyPadTaak          ("keypadTaak", this),
    butTonTaak          ("buttonTaak", this)
    {}
    /// \brief
    /// Interface for writing pressed buttons
    /// \details    
    /// Public function to write the pressed key to. This function internally uses a channel as waitable to save this incoming data.                                                                                
    void keyPressed(char key){inputChannel.write(key);};
};





#endif