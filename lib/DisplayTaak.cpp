#include "DisplayTaak.hpp"
#include "hwlib.hpp"
#include "rtos.hpp"
#include <array>


void displayTaak::main(){
    auto scl                = hwlib::target::pin_oc( hwlib::target::pins::scl);
    auto sda                = hwlib::target::pin_oc( hwlib::target::pins::sda);
    auto i2c_bus            = hwlib::i2c_bus_bit_banged_scl_sda( scl, sda);
    auto oled               = hwlib::glcd_oled(i2c_bus, 0x3c);
    
    auto wMessage           = hwlib::part(oled, hwlib::xy(3, 20), hwlib::xy(123,25));
    auto wName              = hwlib::part(oled, hwlib::xy(3, 5), hwlib::xy(32,10));
    auto wWeaponT           = hwlib::part(oled, hwlib::xy(38, 5), hwlib::xy(61,9));
    auto wAmmo              = hwlib::part(oled, hwlib::xy(100, 5), hwlib::xy(26,9));
    auto wHealth            = hwlib::part(oled, hwlib::xy(3, 53), hwlib::xy(60, 10));
    auto wTime              = hwlib::part(oled, hwlib::xy(67, 53), hwlib::xy(60,10));
    auto wRectangle         = hwlib::part(oled, hwlib::xy(0, 0), hwlib::xy(128,64));
    
 
    auto fMessage           = hwlib::font_default_8x8();
    auto fName              = hwlib::font_default_8x8();
    auto fAmmo              = hwlib::font_default_8x8();
    auto fHealth            = hwlib::font_default_8x8();
    auto fTime              = hwlib::font_default_8x8();
    
    auto messageDisplay     = hwlib::terminal_from(wMessage, fMessage);
    auto nameDisplay        = hwlib::terminal_from(wName, fName);
    auto ammoDisplay        = hwlib::terminal_from(wAmmo, fAmmo);
    auto healthDisplay      = hwlib::terminal_from(wHealth, fHealth);
    auto timeDisplay        = hwlib::terminal_from(wTime, fTime);
    auto weaponTDisplay     = hwlib::terminal_from(wWeaponT, fAmmo);
    
                                                                                    //////////////////////////////////////////////////////////////
    hwlib::rectangle messageRectangle(hwlib::xy(0, 15), hwlib::xy(127,49));                                   //Maak alle rectangles aan
    hwlib::rectangle nameRectangle(hwlib::xy(0, 0), hwlib::xy(35, 15));                                      //
    hwlib::rectangle ammoRectangle(hwlib::xy(35, 0), hwlib::xy(127, 15));                                     //
    hwlib::rectangle healthRectangle(hwlib::xy(0, 49), hwlib::xy(64,63));                                    //
    hwlib::rectangle timeRectangle(hwlib::xy(64, 49), hwlib::xy(127,63));                                      //

    
    oled.clear(); 
    //oled.flush();
    messageRectangle.draw(wRectangle);  
    nameRectangle.draw(wRectangle);  
    ammoRectangle.draw(wRectangle);   
    healthRectangle.draw(wRectangle);  
    timeRectangle.draw(wRectangle); 
    //oled.flush();
    enum class display_state_t        {IDLE};
    display_state_t         state   = display_state_t::IDLE;
 
   
    for(;;){
        switch(state){
            case display_state_t::IDLE:{
                auto message = inputChannel.read();
            // break;
                if(message.type == 'M'){
                   
                    hwlib::wait_us(10); wMessage.clear();
                    hwlib::wait_us(10); messageDisplay  << "\t0000"; 
                    hwlib::wait_us(10);messageDisplay <<  message.stringToWrite;
                    hwlib::wait_us(10); //MessageDisplay; 
                    messageDisplay.flush();
                }
                else if(message.type == 'N'){
                    hwlib::wait_us(10); wName.clear();
                    hwlib::wait_us(10); nameDisplay << "\t0000";
                    hwlib::wait_us(10); nameDisplay << "#";
                    hwlib::wait_us(10); nameDisplay << message.intToWrite;
                    hwlib::wait_us(10); nameDisplay << hwlib::flush;
                    nameDisplay.flush();
                }
                else if(message.type == 'A'){
                    hwlib::wait_us(10); wAmmo.clear();
                    hwlib::wait_us(10); ammoDisplay << "\t0000";
                    hwlib::wait_us(10); ammoDisplay << message.intToWrite;
                    hwlib::wait_us(10);// AmmoDisplay;// << hwlib::flush;
                    ammoDisplay.flush();
                }
                else if(message.type == 'H'){
                    hwlib::wait_us(10); wHealth.clear();
                    hwlib::wait_us(10); healthDisplay   << "\t0000";
                    hwlib::wait_us(10); healthDisplay << "HP: ";
                    hwlib::wait_us(10); healthDisplay << message.intToWrite;
                    hwlib::wait_us(10);// HealthDisplay;// << hwlib::flush;
                    healthDisplay.flush();
                }
                else if(message.type == 'T'){
                    hwlib::wait_us(10); wTime.clear();
                    hwlib::wait_us(10); timeDisplay     << "\t0000";
                    hwlib::wait_us(10); timeDisplay << message.intToWrite;
                    hwlib::wait_us(10); //TimeDisplay;// <<  hwlib::flush;
                    timeDisplay.flush();
                }
                else if(message.type == 'G'){
                    hwlib::wait_us(10); wWeaponT.clear();
                    hwlib::wait_us(10); weaponTDisplay << "\t0000"; 
                    hwlib::wait_us(10); weaponTDisplay << message.stringToWrite; 
                    hwlib::wait_us(10);// WeaponTDisplay;// << hwlib::flush;
                    weaponTDisplay.flush();
                }
                break;
            }   
            default:
                break;
        }
    }
};


void displayTaak::showMessage(const char* message, char oledRec){  
                            //- Public functie voor het verwerken van een dataType String
    typeMessage sendMessage;                                                  //- creeer een TypeMessage struct object voor de Channel
    sendMessage.stringToWrite   = message;                                      //- Vul de struct met de Message en het dataType
    sendMessage.type            = oledRec;                                      //
    inputChannel.write(sendMessage); 
}          

void displayTaak::showMessage(int message, char oledRec){                                    //- Public functie voor het verwerken van een dataType Int
    typeMessage sendMessage;                                                    //- Creeer een TypeMessage struct object voor de Channel
    sendMessage.intToWrite      = message;                                         //- Vul de struct met de Message en het dataType    
    sendMessage.type            = oledRec;                                      //    
    inputChannel.write(sendMessage);                                            //- Write de struct in de channel. Nu kan de task het verwerken                         
}           