#include "InputControlTaak.hpp"

void inputControlTaak::main(){
    enum class  InputState_t       {WAIT_FOR_INPUT};
    InputState_t   inputState =   InputState_t::WAIT_FOR_INPUT;
    for(;;){
        switch(inputState){
            
            case InputState_t::WAIT_FOR_INPUT:
                key = inputChannel.read();
                switch(key){
                    case '0':
                        inPutListener->inputMessage(buttonid::zeroButton);
                        break;
                    case '1':
                        inPutListener->inputMessage(buttonid::oneButton);
                        break;
                    case '2':
                        inPutListener->inputMessage(buttonid::twoButton);
                        break;
                    case '3':
                        inPutListener->inputMessage(buttonid::threeButton);
                        break;
                    case '4':
                        inPutListener->inputMessage(buttonid::fourButton);
                        break;
                    case '5':
                        inPutListener->inputMessage(buttonid::fiveButton);
                        break;
                    case '6':
                        inPutListener->inputMessage(buttonid::sixButton);
                        break;
                    case '7':
                        inPutListener->inputMessage(buttonid::sevenButton);
                        break;
                    case '8':
                        inPutListener->inputMessage(buttonid::eightButton);
                        break;
                    case '9':
                        inPutListener->inputMessage(buttonid::nineButton);
                        break;
                    case 'A':
                        inPutListener->inputMessage(buttonid::aButton);
                        break;
                    case 'B':
                        inPutListener->inputMessage(buttonid::bButton);
                        break;
                    case 'C':
                        inPutListener->inputMessage(buttonid::cButton);
                        break;
                    case 'D':
                        inPutListener->inputMessage(buttonid::dButton);
                        break;
                    case 'E':
                        inPutListener->inputMessage(buttonid::eButton);
                        break;
                    case 'F':
                        inPutListener->inputMessage(buttonid::fButton);
                        break;
                    case '*':
                        inPutListener->inputMessage(buttonid::starButton);
                        break;
                    case '#':
                        inPutListener->inputMessage(buttonid::hastagButton);
                        break;
                    default: 
                        break;
                        
                }

        }
        
    }
    
    
}