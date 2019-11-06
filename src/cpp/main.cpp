#include "runGameTaak.hpp"



int main(){
    namespace target = hwlib::target;

    hwlib::wait_ms(500);

    /* constructing pins */
    auto laserSight         = hwlib::target::pin_out( hwlib::target::pins::d1 );
    auto led                = hwlib::target::d2_36kHz();
    auto reload             = hwlib::target::pin_in( hwlib::target::pins::d4);
    auto ir_sensor          = target::pin_in( target::pins::d8 );   
    auto tsop_gnd           = target::pin_out( target::pins::d9 );
    auto tsop_vdd           = target::pin_out( target::pins::d10 );
    tsop_gnd.write( 0 );
    tsop_vdd.write( 1 );
    tsop_gnd.flush();
    tsop_vdd.flush();
    
    /* constructing player pool */
    auto player = playerInfo();
    auto hits = hit();
    auto playerpool         = rtos::pool<playerInfo>("playerpool");


    /* constructing tasks */
    auto display            = displayTaak(8, "DisplayTaak");
    auto transmitter        = sendTask(1, "sendTask", led, laserSight, 1000 );
    auto speaker            = speakerTaak(4, "SpeakerTaak");
    auto runGame            = runGameTaak(6, "RunGameTaak",display, transmitter, playerpool, speaker);   
    auto decoder            = msg_decoder(2, "decoder", runGame);
    auto pause_detection    = pause_detector(0, "pause_detector", ir_sensor, decoder);

    /* run rtos */
    rtos::run();
}