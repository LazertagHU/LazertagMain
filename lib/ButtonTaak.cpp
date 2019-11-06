#include "ButtonTaak.hpp"

void buttonTaak::main(){
    enum class ButtonState_t        {WAIT_FOR_INPUT, PRESSED};
    ButtonState_t buttonState       = ButtonState_t::WAIT_FOR_INPUT;
    auto button = hwlib::target::pin_in(hwlib::target::pins::d3);
    auto reloadButton = hwlib::target::pin_in(hwlib::target::pins::d4);
    
    for(;;){
        
        switch(buttonState){
            case ButtonState_t::WAIT_FOR_INPUT:
                wait(buttonClock);
                if(!button.read() && !buttonPressed ){
                    buttonPressed = true;
                    keypadlistener->keyPressed('E');
                    buttonState = ButtonState_t::PRESSED;
                    break;
                }
                if(!reloadButton.read() && !reloadPressed ){
<<<<<<< HEAD
                    buttonPressed = true;
                    keypadlistener->keyPressed('F');
                    buttonState = ButtonState_t::PRESSED;
=======
                    reloadPressed = true;
                    Keypadlistener->KeyPressed('F');
                    ButtonState = ButtonState_t::PRESSED;
>>>>>>> f7c7a7d312dbcb45a6f4fd373e7cbb1b676286ee
                    break;
                }
                break;
                
            case ButtonState_t::PRESSED:
                wait(buttonClock);
                if(button.read() && reloadButton.read()){
                    buttonPressed = false;
<<<<<<< HEAD
                    buttonState = ButtonState_t::WAIT_FOR_INPUT;
=======
                    reloadPressed = false;
                    ButtonState = ButtonState_t::WAIT_FOR_INPUT;
>>>>>>> f7c7a7d312dbcb45a6f4fd373e7cbb1b676286ee
                    break;
                }
                break;
            
            default: break;

        }
    }
};
