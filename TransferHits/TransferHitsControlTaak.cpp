#include "TransferHitsControlTaak.hpp"
#include "hwlib.hpp"
#include "PlayerInfo.hpp"

void TransferHitsControlTaak::write(){
    TransferEnableFlag.set();
}

void TransferHitsControlTaak::main(){
    state = states::IDLE;
    for(;;){
        switch( state ){
            case states::IDLE:
                wait( TransferEnableFlag );
                PlayerInfo player = entitypool.read();
                int playerID = player.GetPlayerID();
                auto hits = player.GetHits();
                int hitNumber = player.GethitAmount();
                
                hwlib::cout << playerID << "\n";
                if( hitNumber != 0 ){
                    for( unsigned int i = hitNumber; i >= 0; i-- ){
                        hwlib::cout << "Enemy: " << hits[i].EnemyID << ", Damage: " << hits[i].Damage << ", Time: " << hits[i].Time << "\n";
                    }
                }else{
                    hwlib::cout << "No hits registered\n";
                }    
                break;
        }
    }

}
