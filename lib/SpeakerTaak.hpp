#ifndef _SPEAKERTAAK_HPP
#define _SPEAKERTAAK_HPP
#include <array>
#include "hwlib.hpp"
#include "rtos.hpp"


class speakerTaak : public rtos::task<>{
private:
    
    rtos::flag              shootFlag;
    rtos::flag              hitFlag;
    rtos::timer             speakerTimer;
    
    /// \brief
    /// Main() of the Speaker task.
    /// \details    
    /// This task waits until a flag is set, after which it will create the sound, corresponding to the flag, using preprogrammed 
    /// frequencies and durations.
    void main() override;
public:
    /// \brief
    /// Default constructor for the SpeakerTaak
    /// \details    
    /// Set task name and priority, initiates the flags and timer.
    speakerTaak(int prio, const char * name):
        task                (prio, name),
        shootFlag           (this, "ShootFlag"),
        hitFlag             (this, "HitFlag"),
        speakerTimer        (this, "SpeakerTimer")
    {}
    
     /// \brief
    /// Interface to create shooting sound.
    /// \details    
    /// Calling this function will set the Shootflag, telling the task to create a shooting sound.
    void shootSound()   {shootFlag.set();};
     /// \brief
    /// Interface to create the sound of being hit.
    /// \details    
    /// Calling this function will set the Hitflag, telling the task to create the sound of being hit.
    void hitSound()     {hitFlag.set();};
   
};





#endif
