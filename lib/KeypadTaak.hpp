#ifndef _KEYPADTAAK_HPP
#define _KEYPADTAAK_HPP
#include <array>
#include "hwlib.hpp"
#include "rtos.hpp"
#include "KeypadListener.hpp"


class keypadTaak : public rtos::task<>{
private:

    rtos::clock                                         keyClock;
    int                                                 temp = 0;
    char                                                key  = '\0';
    keypadListener                                      *keyPadListener;
    
    /// \brief
    /// Main() for the Keypad task.
    /// \details
    /// uses the hwlib::keypad to read the keypad buttons on a 100ms interval.
    /// It uses its listener to send the pressed button.
    void main() override;
    
public:
    /// \brief
    /// Default constructor for the KeypadTaak
    /// \details
    /// Names the task, starts a 100ms clock and binds the listener
    keypadTaak(const char * name, keypadListener *keyPadListener):
    task(5, name),
    keyClock(this, 100'000, "keyClock"),
    keyPadListener(keyPadListener)
    {}
};





#endif