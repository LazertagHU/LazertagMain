#include "SpeakerTaak.hpp"

enum class SpeakerState_t               {WAIT_FOR_INPUT, SHOOTING, HIT};
enum class SubShootingState_t           {SHOOT1, SHOOT2};
enum class SubHitState_t                {HIT1, HIT2};

struct noot{ 
    int                frequentie;
    int                duration;
};

void speakerTaak::main(){
    SpeakerState_t SpeakerState         = SpeakerState_t::WAIT_FOR_INPUT;
    SubShootingState_t SubShootingState = SubShootingState_t::SHOOT1;
    SubHitState_t       SubHitState     = SubHitState_t::HIT1;
    
    auto            lsp                 = hwlib::target::pin_out( hwlib::target::pins::d7 );
    int             half_period;
    unsigned int    end;
<<<<<<< HEAD
    unsigned int    TempFreq;
    const std::array<noot, 2> Shooting{ noot{200, 100000}, noot{1500, 695000} };
    const std::array<noot, 2> Hit{ noot{200, 100000}, noot{130, 600000} };
=======
    unsigned int    tempFreq;
    const std::array<noot, 2> shooting{ noot{200, 60000}, noot{1500, 695000} };
    const std::array<noot, 2> hit{ noot{200, 100000}, noot{130, 600000} };
>>>>>>> 718c31de2c1fafdfc3c3f3cfe02d3a72f119e0eb
    
    for(;;){
        
        switch(SpeakerState){
            case SpeakerState_t::WAIT_FOR_INPUT:{
                auto evt1           = wait(shootFlag + hitFlag);
                if(evt1 == shootFlag){
                    SpeakerState    = SpeakerState_t::SHOOTING;
                    SubShootingState = SubShootingState_t::SHOOT1;
                    tempFreq                = shooting[0].frequentie;
                    half_period             = (1'000'000 / (2 * tempFreq));
                    end                     = hwlib::now_us() + shooting[0].duration;
                    break;
                    }
                else if(evt1 == hitFlag){
                    SpeakerState    = SpeakerState_t::HIT;
                    SubHitState = SubHitState_t::HIT1;
                    tempFreq                = hit[0].frequentie;
                    half_period             = (1'000'000 / (2 * tempFreq));
                    end                     = hwlib::now_us() + hit[0].duration;
                    }
                break;
            }
            case SpeakerState_t::SHOOTING:{
                switch(SubShootingState){
                    case SubShootingState_t::SHOOT1:{
                        lsp.write(1);
                        lsp.flush();
                        hwlib::wait_us(half_period);
                        lsp.write(0);
                        lsp.flush();
                        speakerTimer.set(half_period);
                        wait(speakerTimer);
                        if(end < hwlib::now_us()){
                            //hwlib::cout<< "komt in if shooting\n";
                            tempFreq                = shooting[1].frequentie;
                            half_period             = (1'000'000 / (3 * tempFreq));
                            SubShootingState = SubShootingState_t::SHOOT2;
                            end                     = hwlib::now_us() + shooting[1].duration;
                            break;
                        }
                        break;
                    }
                    
                    case SubShootingState_t::SHOOT2:{ 
                        //hwlib::cout << "hoi\n";
                        lsp.write(1);
                        lsp.flush();
                        hwlib::wait_us(half_period);
                        lsp.write(0);
                        lsp.flush();
                        speakerTimer.set(half_period);
                        wait(speakerTimer);
                        tempFreq       -= 4;
                        half_period     = (1'000'000 / (3 * tempFreq));
                        if(end < hwlib::now_us()){
                            SpeakerState = SpeakerState_t::WAIT_FOR_INPUT;
                            break;
                        }
                        break;
                    }
                }
            }
            
            
            case SpeakerState_t::HIT:{
                
                switch(SubHitState){
                    
                    case SubHitState_t::HIT1:{
                        lsp.write(1);
                        lsp.flush();
                        hwlib::wait_us(half_period);
                        lsp.write(0);
                        lsp.flush();
                        speakerTimer.set(half_period);
                        wait(speakerTimer);
                        
                        if(end < hwlib::now_us()){
                            tempFreq                = hit[1].frequentie;
                            half_period             = (1'000'000 / (2 * tempFreq));
                            SubHitState             = SubHitState_t::HIT2;
                            end                     = hwlib::now_us() + hit[1].duration;
                            break;
                        }
                        break;
                    }
                    case SubHitState_t::HIT2:{
                        lsp.write(1);
                        lsp.flush();
                        hwlib::wait_us(half_period);
                        lsp.write(0);
                        lsp.flush();
                        speakerTimer.set(half_period);
                        wait(speakerTimer);
                        if(end < hwlib::now_us()){
                            SpeakerState = SpeakerState_t::WAIT_FOR_INPUT;
                            break;
                        }
                        break;
                        
                    }
                    
                }
            }
        }   
    }
};
