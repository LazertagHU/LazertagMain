#ifndef _SPEAKERTAAK_HPP
#define _SPEAKERTAAK_HPP
#include <array>
#include "hwlib.hpp"
#include "rtos.hpp"


class SpeakerTaak : public rtos::task<>{
private:
    
    rtos::flag              ShootFlag;
    rtos::flag              HitFlag;
    rtos::timer             SpeakerTimer;
    void main() override;
public:
    SpeakerTaak(int prio, const char * name):
        task                (prio, name),
        ShootFlag           (this, "ShootFlag"),
        HitFlag             (this, "HitFlag"),
        SpeakerTimer        (this, "SpeakerTimer")
    {}
    
    
    void ShootSound()   {ShootFlag.set();};
    void HitSound()     {HitFlag.set();};
   
};





#endif