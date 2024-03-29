#ifndef MSG_DECODER_HPP
#define MSG_DECODER_HPP

#include "msg_listener.hpp"
#include "pause_listener.hpp"
#include "rtos.hpp"
#include "hwlib.hpp"

class msg_decoder : public pause_listener, rtos::task<> {
protected:
    msg_listener & listener;
    rtos::channel < int, 128 > pauseChannel; 
    rtos::timer reset_timer;

    /// \brief
    /// Main() for the msg_decoder task.
    /// \details
    /// The main recieves pauses with the pauseChannel. It uses these to decode incoming messages following our transmission protocol.
    /// The decoded messages are checked for validity and passed on to the msg_listener.
    void main() override;

public:
    /// \brief
    /// Default constructor for the msg_decoder task.
    /// \details
    /// Names the task, binds the listener, inits the pauseChannel and inits the reset_timer.
    msg_decoder(int prio,
        const char * name,
        msg_listener & listener
    ):
        task( prio, name ),
        listener(listener),
        pauseChannel(this, "pauseChannel"),
        reset_timer(this, "reset_timer")
    {}

    /// \brief
    /// Interface for writing detected pauses.
    /// \details    
    /// Public function that is found in the pause_listener. This can be called from outside the class to write data into the pauseChannel.
    void pause_detected( int n ) override { pauseChannel.write( n );}


};

#endif // MSG_DECODER_HPP
