#include "SpeakerTaak.hpp"

enum class SpeakerState_t               {WAIT_FOR_INPUT, SHOOTING, HIT};
enum class SubShootingState_t           {SHOOT1, SHOOT2};
enum class SubHitState_t                {HIT1, HIT2};

struct noot{ 
    int                Frequentie;
    int                duration;
};

void SpeakerTaak::main(){
    SpeakerState_t SpeakerState         = SpeakerState_t::WAIT_FOR_INPUT;
    SubShootingState_t SubShootingState = SubShootingState_t::SHOOT1;
    SubHitState_t       SubHitState     = SubHitState_t::HIT1;
    
    auto            lsp                 = hwlib::target::pin_out( hwlib::target::pins::d7 );
    int             half_period;
    unsigned int    end;
    unsigned int    TempFreq;
    const std::array<noot, 2> Shooting{ noot{200, 6 0000}, noot{1500, 695000} };
    const std::array<noot, 2> Hit{ noot{200, 100000}, noot{130, 600000} };
    
    for(;;){
        
        switch(SpeakerState){
            case SpeakerState_t::WAIT_FOR_INPUT:{
                auto evt1           = wait(ShootFlag + HitFlag);
                if(evt1 == ShootFlag){
                    SpeakerState    = SpeakerState_t::SHOOTING;
                    SubShootingState = SubShootingState_t::SHOOT1;
                    TempFreq                = Shooting[0].Frequentie;
                    half_period             = (1'000'000 / (2 * TempFreq));
                    end                     = hwlib::now_us() + Shooting[0].duration;
                    break;
                    }
                else if(evt1 == HitFlag){
                    SpeakerState    = SpeakerState_t::HIT;
                    SubHitState = SubHitState_t::HIT1;
                    TempFreq                = Hit[0].Frequentie;
                    half_period             = (1'000'000 / (2 * TempFreq));
                    end                     = hwlib::now_us() + Hit[0].duration;
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
                        hwlib::wait_us(half_period);
                        //hwlib::cout<<"hij komt in shoot1\n";
                        if(end < hwlib::now_us()){
                            //hwlib::cout<< "komt in if shooting\n";
                            TempFreq                = Shooting[1].Frequentie;
                            half_period             = (1'000'000 / (3 * TempFreq));
                            SubShootingState = SubShootingState_t::SHOOT2;
                            end                     = hwlib::now_us() + Shooting[1].duration;
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
                        hwlib::wait_us(half_period);
                        TempFreq       -= 4;
                        half_period     = (1'000'000 / (3 * TempFreq));
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
                        hwlib::wait_us(half_period);
                        
                        if(end < hwlib::now_us()){
                            TempFreq                = Hit[1].Frequentie;
                            half_period             = (1'000'000 / (2 * TempFreq));
                            SubHitState             = SubHitState_t::HIT2;
                            end                     = hwlib::now_us() + Hit[1].duration;
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
                        hwlib::wait_us(half_period);
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