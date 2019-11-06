#ifndef _PLAYERINFO_HPP
#define _PLAYERINFO_HPP

#include "weapon.hpp"
#include "hwlib.hpp"
//#include "hit.hpp"

class playerInfo {
private:
    unsigned int playerID;
    std::array<weapon, 10> weapons{weapon{"pistol", 5, 4'000'000, 500'000, 3'000'000, 20}, weapon{"sniper", 40, 10'000'000, 3'000'000, 5'000'000, 10}};
    unsigned int currentWeapon;
    //hit hits[10]; // 100 breekt alles, 50 breek het op een andere manier. 10 werkt volledig
    int health;
    int hitAmount;
    
public:
    /// \brief
    /// The default constructor for the PlayerInfo entity.
    /// \details
    /// Sets starting amounts of health and hitAmount. During the game this entity saves all player info
    playerInfo(){
        health = 100;
        hitAmount = 0;
    }
    
    /// \brief
    /// Returns the n weapon 
    weapon getWeapon(int n);
    

    /// \brief
    /// Returns the current weapon
    unsigned int getCurrentWeapon();

    /// \brief
    /// Sets the current weapon to n
    void setWeapon(int n);

    /// \brief
    /// Returns the current PlayerID.
    unsigned int getPlayerID();

    /// \brief
    /// Sets the player ID to NewPlayerID
    void setPlayerID( unsigned int newPlayerID );

    //hit* GetHits();
    //void AddHit( int EnemyID, int Damage, int Time );

    /// \brief
    /// Set the current health to NewHealt
    void setHealth( int newHealth );

    /// \brief
    /// Returns the current healt
    int getHealth();
    //int GethitAmount();
};

#endif