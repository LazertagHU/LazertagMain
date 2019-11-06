#ifndef _SENDTASK_HPP
#define _SENDTASK_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
#include "../../rtos/rtos.hpp"
#include "../../hwlib/library/hwlib.hpp"

class sendTask : public rtos::task<> {
private:
    enum class states { IDLE, SENDHIGH1, SENDHIGH0, SENDLOW1, SENDLOW0 };
    states state;
    hwlib::target::d2_36kHz led;
    hwlib::target::pin_out laser;
    int weaponDelay;
    rtos::flag sendFlag;
    rtos::pool<uint32_t> sendpool;

    /// \brief
    /// The main() of the SendTaak.
    /// \details
    /// Recieves messages by SendMessage(). Encodes these messages according to the protocol to send to the IR led.
    void main() override;
    
public:
    /// \brief
    /// Default constructor of the SendTask
    /// \details
    /// Names its task, binds its pin and inits the flag and the pool.
    sendTask(int prio, const char * name, hwlib::target::d2_36kHz led, hwlib::target::pin_out laser, int weaponDelay ):
        task( prio, name ),
        led( led ),
        laser( laser ),
        weaponDelay( weaponDelay ),
        sendFlag( this, "sendFlag" ),
        sendpool( "sendpool" )
        {}

    /// \brief
    /// Interface for writing messages.
    /// \details    
    /// Public function to write messages to that need to be send by the IR led. This function internally uses a flag and a pool to save this incoming data. 
    void sendMessage( uint32_t commandID );
};

#endif