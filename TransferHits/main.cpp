#include "hwlib.hpp"
#include "PlayerInfo.hpp"
#include "hit.hpp"
#include "weapon.hpp"
#include "rtos.hpp"

int main(){
    hwlib::wait_ms( 500 );
    hwlib::cout << "asdasffas\n";
    auto player = PlayerInfo();
    rtos::pool< PlayerInfo > entity_pool( "entity_pool" );


    // player.SetPlayerID( 10 );
    // hwlib::cout << player.GetPlayerID() << "\n";

    // player.SetHealth( 30 );
    // hwlib::cout << player.GetHealth() << "\n";

    // player.AddHit( 3, 20, 130 );
    // hwlib::cout << player.GetHits()[0].Damage << "\n";

    // weapon newWep= { "Sniper", 30 };
    // player.SetWeapon(newWep);
    // weapon curWep = player.GetWeapon();
    // hwlib::cout << curWep.Damage << "\n";
}