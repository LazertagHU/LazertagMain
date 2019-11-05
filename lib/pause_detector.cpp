#include "pause_detector.hpp"

void pause_detector::main(){
    int n = 0;
    for(;;){
        wait( interval_clock );
        if(!irsensor.read()){ n+=150;}
        else { 
            listener.pause_detected( n ); 
            while( irsensor.read() ){
                wait( interval_clock );
            }
            n = 0;
        } 
    }
}