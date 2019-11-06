#include "runGameTaak.hpp"



int main(){
    namespace target = hwlib::target;

    hwlib::wait_ms(500);

    /* constructing pins */
    auto lasersight         = hwlib::target::pin_out( hwlib::target::pins::d1 );
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
    auto player = PlayerInfo();
    auto hits = hit();
    // HWLIB_TRACE << "player_t: " << sizeof(PlayerInfo);
    // HWLIB_TRACE << "hit_t: " <<sizeof(hit);
    // HWLIB_TRACE << "playerObj: " << sizeof(player);
    // HWLIB_TRACE << "hitObj: " <<sizeof(hit);
    // HWLIB_TRACE;

    auto playerpool         = rtos::pool<PlayerInfo>("playerpool");
    
    /* constructing tasks */
    auto display            = DisplayTaak(2, "DisplayTaak");
    //auto transfer           = TransferHitsControlTaak(9, "TransferHitsControlTaak",playerpool);
    auto transmitter        = SendTask(8, "InputTask", led, lasersight, 1000 );
    auto Speaker            = SpeakerTaak(4, "SpeakerTaak");
    auto runGame            = RunGameTaak(6, "RunGameTaak",display, transmitter, playerpool, Speaker);   
    auto decoder            = msg_decoder(1, "decoder", runGame);
    auto pause_detection    = pause_detector(0, "pause_detector", ir_sensor, decoder);

    /* run rtos */
    rtos::run();
}