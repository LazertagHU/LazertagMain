#include "PlayerInfo.hpp"


weapon playerInfo::getWeapon(int n){ 
    return weapons[n-1]; 
}

unsigned int playerInfo::getCurrentWeapon(){ 
    return currentWeapon;
}

void playerInfo::setWeapon( int weaponID ){ 
    currentWeapon = weaponID; 
}
    
unsigned int playerInfo::getPlayerID(){ 
    return playerID; 
}

void playerInfo::setPlayerID( unsigned int newPlayerID ){ 
    playerID = newPlayerID; 
}

void playerInfo::setHealth( int newHealth ){ 
    health = newHealth; 
}

int playerInfo::getHealth(){ 
    return health; 
}

//int PlayerInfo::GethitAmount(){ 
//    return hitAmount; 
//}