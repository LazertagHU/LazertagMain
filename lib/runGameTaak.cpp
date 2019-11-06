#include "runGameTaak.hpp"

void runGameTaak::preGame(){
    preGame_states_t pregameState = preGame_states_t::IDLE;

    bool                        playerWeaponEntered = false;
    bool                        playerIDEntered     = false;
    bool                        gameTimeEntered     = false;
    bool                        loop                = true;
    if(gameLeader == true){
            display.showMessage(0,'N');
            player.setPlayerID(0);
            playerIDEntered = true;
        }

    while(loop)
    {
        switch (pregameState)
        {
        case preGame_states_t::IDLE:{
            display.showMessage("Game Setup", 'M') ;
            auto evt = wait(inputChannel + messageFlag); 
            if(evt == inputChannel)
            {
                bnID = inputChannel.read();
                if(bnID == buttonid::cButton && playerWeaponEntered == true && gameLeader == true)
                {
                    display.showMessage("Enter Game\nTime", 'M');
                    pregameState = preGame_states_t::ENTER_TIME_REMAINING;
                }
                else if(bnID == buttonid::bButton)
                {
                    display.showMessage("Choose weapon", 'M');
                    pregameState = preGame_states_t::WAIT_FOR_WEAPON_NUMBER;
                }else if(bnID == buttonid::aButton && gameLeader == false)
                {
                    display.showMessage("Enter a player\nID", 'M');
                    pregameState = preGame_states_t::WAIT_FOR_PLAYER_NUMBER;
                }
            }
            else
            {
                msg = messagepool.read();
                if(isGameTimeMessage(msg)) // gametime
                {
                    remainingGameTime   = 60*computeGameTime(msg); //----------
                    display.showMessage(remainingGameTime/60, 'T');
                    gameTimeEntered     = true;
                }
                else if (isStartMessage(msg) && gameTimeEntered == true && playerIDEntered == true && playerWeaponEntered == true)
                {
                    display.showMessage("Starting game\nin", 'M');
                    countdown       = 10 + computeCountdown(msg);
                    pregameState    = preGame_states_t::AFTELLEN;
                }
            }
            break;
            }
        
        case preGame_states_t::WAIT_FOR_PLAYER_NUMBER:{
            int input = waitForInput('N');
            if(input > 0 && input <= 15)
            {
                player.setPlayerID(input);
                playerIDEntered = true;
            }
            else
            {
                display.showMessage("invalid player\nid", 'M');
                display.showMessage(-1, 'N');
            }
            pregameState = preGame_states_t::IDLE;
            break;
            }

        case preGame_states_t::WAIT_FOR_WEAPON_NUMBER:{
            int input = waitForInput('A');
            if(input > 0 && input <= 15){
                player.setWeapon(input);
                playerWeaponEntered = true;
                display.showMessage(player.getWeapon(input).name, 'G');
                display.showMessage(player.getWeapon(input).bullets, 'A');
                bullets = player.getWeapon(input).bullets;
            }
            else
            {
                display.showMessage("invalid weapon id", 'M');
                display.showMessage(-1, 'A');
            }
            pregameState = preGame_states_t::IDLE;
            break;
            }

        case preGame_states_t::ENTER_TIME_REMAINING:{
            int input = waitForInput('T');
            if(input > 0 && input <= 15){
                remainingGameTime = input*60;
                setTimeCommand |= (input << 5);
                setTimeCommand |= (input << 21);
                setTimeCommand = calculateCheckSum(setTimeCommand);
                pregameState = preGame_states_t::SEND_COMMAND_STATE;
                display.showMessage("press # to send\ngame time", 'M');
            }else{
                display.showMessage("Game Time too\nhigh", 'M');
                display.showMessage(0, 'T');
                pregameState = preGame_states_t::IDLE;
            }
            break;
            }

        case preGame_states_t::SEND_COMMAND_STATE:{
            bnID = inputChannel.read();
            if(bnID == buttonid::hastagButton){
                transmitter.sendMessage(setTimeCommand);
            }else if( bnID == buttonid::starButton){
                countdown = 30;
                startCommand = computeStartCommand(countdown, startCommand);
                display.showMessage(countdown, 'T');
                display.showMessage("press * to send\nstart command", 'M');
                pregameState = preGame_states_t::START_GAME_TRANSMISSION_STATE;
            }
            break;
            }

        case preGame_states_t::START_GAME_TRANSMISSION_STATE:{
            auto evt = wait(secondClock + inputChannel);
                if(evt == inputChannel){
                    bnID = inputChannel.read();
                    if(bnID == buttonid::starButton){
                        transmitter.sendMessage(startCommand);
                    }
                }else if(evt == secondClock){
                    if(countdown > 1){
                        countdown--;
                        startCommand = computeStartCommand(countdown, command);
                        if( countdown % 5 == 0 ){
                            display.showMessage(countdown, 'T');
                        }
                    }else{
                        display.showMessage("Starting game", 'M');
                        countdown = 10;
                        display.showMessage(countdown, 'T');
                        pregameState = preGame_states_t::AFTELLEN;
                    }
                }
            break;
            }

        case preGame_states_t::AFTELLEN:{
            wait(secondClock);
            if(countdown > 1){
                display.showMessage(--countdown, 'T');
            }else{
                player.setHealth(100); // set lives
                display.showMessage(player.getHealth(), 'H');
                display.showMessage("Alive", 'M');
                computeShootCommand(shootCommand);
                loop = 0;
                messageFlag.clear();
            }
            break;
            }
        }
    }
}

void runGameTaak::gameRunning(){
    gameRunning_states_t currentState = gameRunning_states_t::ALIVE;
    bool loop = true;
    while(loop){
        switch (currentState)
        {
        case gameRunning_states_t::ALIVE:{
            auto evt = wait(messageFlag + secondClock + inputChannel);
            if(evt == messageFlag)
            {
                msg = messagepool.read();
                if(isHitMessage(msg))
                {
                    display.showMessage("hit", 'M');                
                    auto damage = computeHit(msg);
                    auto lives = player.getHealth() - damage;
                    if(lives <= 0){
                        player.setHealth(0); 
                        display.showMessage(player.getHealth(), 'H');
                        currentState = gameRunning_states_t::GAME_OVER;
                    }else{
                        player.setHealth(lives);
                        addHit(getEnemyID(msg) ,damage, remainingGameTime);
                        display.showMessage(player.getHealth(), 'H');
                        delayTimer.set(computeDeathDelay(msg));                                              /// check return type of computedelay
                        currentState = gameRunning_states_t::HIT;   
                    }
                }    
            }
            else if(evt == secondClock)
            {
                if( remainingGameTime > 0 )
                {
                    --remainingGameTime;
                    if( remainingGameTime%60 == 0 ){
                        display.showMessage(remainingGameTime/60, 'T');
                    }
                }
                else
                {
                    display.showMessage(0, 'T');
                    currentState = gameRunning_states_t::GAME_OVER;
                }
            }
            else
            {
                bnID = inputChannel.read();
                if(bnID == buttonid::eButton && bullets > 0)
                {
                    speaker.shootSound();
                    transmitter.sendMessage(shootCommand);
                    delayTimer.set(computeShootDelay()); ///<<----
                    bullets--;
                    display.showMessage(bullets, 'A');
                    currentState = gameRunning_states_t::WEAPON_COOLDOWN;
                }
                else if(bnID == buttonid::eButton){
                    display.showMessage("Out of ammo!", 'M');
                }
                else if(bnID == buttonid::fButton)
                {
                    display.showMessage("Reloading \nweapon...", 'M');
                    delayTimer.set(player.getWeapon(player.getCurrentWeapon()).weaponReloadTime);
                    currentState = gameRunning_states_t::WEAPON_RELOAD;
                }
            }
            break;
        }
        case gameRunning_states_t::WEAPON_RELOAD:{
            auto evt = wait(delayTimer + messageFlag + secondClock);
            if(evt == delayTimer){
                bullets = player.getWeapon(player.getCurrentWeapon()).bullets;
                display.showMessage(bullets, 'A');
                display.showMessage("Alive!", 'M');
                currentState = gameRunning_states_t::ALIVE;
            }else if( evt == secondClock){
                if( remainingGameTime > 0 )
                {
                    remainingGameTime--;
                    if( remainingGameTime%60 == 0 ){
                        display.showMessage(remainingGameTime/60, 'T');
                    }
                }
                else
                {
                    currentState = gameRunning_states_t::GAME_OVER;
                }
            }
            else{
                msg = messagepool.read();
                if(isHitMessage(msg))
                {
                    display.showMessage("hit", 'M');                
                    auto damage = computeHit(msg);
                    auto lives = player.getHealth() - damage;
                    if(lives <= 0){
                        player.setHealth(0); 
                        display.showMessage(player.getHealth(), 'H');
                        currentState = gameRunning_states_t::GAME_OVER;
                    }else{
                        player.setHealth(lives);
                        addHit(getEnemyID(msg) ,damage, remainingGameTime);
                        display.showMessage(player.getHealth(), 'H');
                        delayTimer.set(computeDeathDelay(msg));                                              /// check return type of computedelay
                        currentState = gameRunning_states_t::HIT;   
                    }
                }    
            }
            
            break;
        }
        case gameRunning_states_t::WEAPON_COOLDOWN:{
            auto evt = wait(delayTimer + messageFlag + secondClock);
            if(evt == messageFlag)
            {
                msg = messagepool.read();
                if(isHitMessage(msg))
                {
                    display.showMessage("hit", 'M');                
                    auto damage = computeHit(msg);
                    auto lives = player.getHealth() - damage;
                    if(lives <= 0){
                        player.setHealth(0); 
                        display.showMessage(player.getHealth(), 'H');
                        currentState = gameRunning_states_t::GAME_OVER;
                    }else{
                        player.setHealth(lives);
                        addHit(getEnemyID(msg) ,damage, remainingGameTime);
                        display.showMessage(player.getHealth(), 'H');
                        delayTimer.set(computeDeathDelay(msg));                                              /// check return type of computedelay
                        currentState = gameRunning_states_t::HIT;   
                    }
                }                    
            }
            else if(evt == secondClock)
            {
                if( remainingGameTime > 0 )
                {
                    remainingGameTime--;
                    if( remainingGameTime%60 == 0 ){
                        display.showMessage(remainingGameTime/60, 'T');
                    }
                }
                else
                {
                    currentState = gameRunning_states_t::GAME_OVER;
                }
            }
            else
            {
                inputChannel.clear();
                currentState = gameRunning_states_t::ALIVE;
            }
            break;
        }
        case gameRunning_states_t::HIT:{
            auto evt = wait(delayTimer + secondClock);
            if (evt == delayTimer)
            {
                display.showMessage("Alive", 'M');
                inputChannel.clear();
                messageFlag.clear();
                currentState = gameRunning_states_t::ALIVE;
            }
            else
            {
                if( remainingGameTime > 0 )
                {
                    remainingGameTime--;
                    if( remainingGameTime%60 == 0 ){
                        display.showMessage(remainingGameTime/60, 'T');
                    }
                }
                else
                {
                    currentState = gameRunning_states_t::GAME_OVER;
                }
            }
            break;
        }
        case gameRunning_states_t::GAME_OVER:{
            hwlib::cout << "dood" << hwlib::endl;
            display.showMessage("Game over", 'M');
            loop = false;
            break;
        }
        default:
            break;
        }   
    }
}

void runGameTaak::postGame(){
    postGame_states_t postGameState = postGame_states_t::IDLE;
    bool loop = 1;
    while(loop){
        switch (postGameState)
        {
        case postGame_states_t::IDLE:{
            bnID = inputChannel.read();
            if(bnID == buttonid::eButton){
                write_hits();
            }
            else if(bnID == buttonid::fButton){
                loop = false;
            }
            break;
        }
        default:
            break;
        }
    }
}

void runGameTaak::main()
{
    while(true){
        preGame();
        gameRunning();
        postGame();
    }
}


void runGameTaak::write_hits(){
                
    hwlib::cout << "PlayerID: " << player.getPlayerID() << "\n";
    if( hitAmount != 0 ){
        for( unsigned int i = hitAmount; i > 0; i-- ){
            hwlib::cout << "Enemy: " << hits[i-1].enemyID << ", Damage: " << hits[i-1].damage << ", Time: " << hits[i-1].time << "\n";
        }
    }else{
        hwlib::cout << "No hits registered\n";
    }    
}


void runGameTaak::addHit( int enemyID, int damage, int time ){
    hit newHit{ enemyID, damage, time };
    if( hitAmount < 100 ){
        hits[hitAmount] = newHit;
        hitAmount++;
    }
}

uint32_t runGameTaak::computeGameTime(uint32_t msg)
{
    msg <<= 23;
    msg >>= 28;
    return msg;
};

uint32_t runGameTaak::computeCountdown(uint32_t msg)
{
    msg <<= 23;
    msg >>= 28;
    return msg*2;
};

void runGameTaak::computeShootCommand(uint32_t & shootcommand){
    auto id     = player.getPlayerID();
    auto wepid  = player.getCurrentWeapon();
    
    shootcommand |= (id << 26);
    shootcommand |= (id << 10);
    shootcommand |= (wepid << 5);
    shootcommand |= (wepid << 21);

    shootcommand = calculateCheckSum(shootcommand);
}

uint32_t runGameTaak::computeStartCommand(uint32_t countdown, uint32_t command)
{
        /* 
        This function calculates the startcommand that can be send to players. 
        the structure is:

        1-00000-1nnnn-xxxxx

        Where nnnn is the remaining countdown devided by 2. So that means a countdown of 30 will result in 1-00000-11111
        the control bits are calculated afterwards
        */
      
        countdown/=2;
        command|= (countdown << 5 );
        command|= (countdown << 21);
        auto x = calculateCheckSum(command);

        return x;
};

int runGameTaak::waitForInput(char place)
{
    int tens;
    int ones;
    int returnval = -1;

    enum class waitForInputStates{AWAIT_FIRST_CHARACTER, AWAIT_SECOND_CHARACTER, END};
    waitForInputStates state = waitForInputStates::AWAIT_FIRST_CHARACTER;
    
    for(;;)
    {
        switch(state)
        {
            case waitForInputStates::AWAIT_FIRST_CHARACTER:{
                bnID = inputChannel.read();
                if(bnID >= buttonid::zeroButton && bnID <= buttonid::nineButton)
                {
                    tens = static_cast<int>(bnID);
                    display.showMessage(tens, place);
                    state = waitForInputStates::AWAIT_SECOND_CHARACTER;
                }
                else if(bnID == buttonid::starButton)
                {
                    state = waitForInputStates::END;
                }
                break;
            }
            case waitForInputStates::AWAIT_SECOND_CHARACTER:{
                bnID = inputChannel.read();
                if(bnID >= buttonid::zeroButton && bnID <= buttonid::nineButton)
                {
                    ones = static_cast<int>(bnID);
                    display.showMessage((tens*10) + ones , place);
                    returnval = (tens*10) + ones;
                    state = waitForInputStates::END;
                }
                else if(bnID == buttonid::starButton)
                {
                    returnval = tens;
                    state = waitForInputStates::END;
                }
                break;
            }
            case waitForInputStates::END:{
                return returnval;
                break;
            }
            default:
                break;
        }
    }
};

uint32_t runGameTaak::calculateCheckSum(uint32_t input)
{
    /*
    message layout:
    2 * 16 bits message.

    bit : 0           : 1 : 2 : 3 : 4 : 5 : 6 : 7 : 8 : 9 : 10 : 11: 12 : 13 : 14 : 15
    val : startBit    : player ID         : weapon ID          : XOR bits

    bit : 0           : 16 : 17 : 18 : 19 : 20 : 21 : 22 : 23 : 24 : 25 : 26: 27 : 28 : 29 : 30
    val : startBit    : player ID              : weapon ID              : XOR bits

    control bit 11 is the xor of bit 1 and 6;
    control bit 12 is the xor of bit 2 and 7;
    etc...
    */
    
    for(int i = 21; i < 26; i++)
    {
        bool x = input & (1 << i);
        bool y = input & (1 << (i+5));

        input = input | ((x ^ y) << (i-5));
        input = input | ((x ^ y) << (i-21));
    }

    return input;
};

bool runGameTaak::isHitMessage(uint32_t message)
{
    /* 
    This funciton calculates wether the player has been shot by an enemy.
    If the player shoots himself, the function will return false;
    */
    auto id = player.getPlayerID();
    message <<=17;
    message >>=27;
    // hwlib::cout << "Message: " << message << " compare: " << (message != id) << " ID: " << id << "\n";
    if(message!= id ){        // if player id from message does not equal own plater id, it means that the player is shot.
        return true;
    }
    return false;
};

int runGameTaak::computeHit(uint32_t message)
{
    /* 
    this function calculates the hit damage. 
    it calculates the weapon id the player has been hit by and
    returns the specific damage that is attached to that weapon.
    */
    message <<=22;
    message >>= 27; // now contains weapon id;
    
    return player.getWeapon(message).damage;
};

int runGameTaak::computeDeathDelay(uint32_t message)
{
    /* 
    this function calculates the deathdelay. 
    it calculates the weapon id the player has been hit by and
    returns the specific delay that is attached to that weapon.
    */
    message <<=22;
    message >>= 27; // now contains weapon id;

    return player.getWeapon(message).deathDelay;
};

int runGameTaak::computeShootDelay()
{
    /* 
    this function calculates the weaponcooldown. 
    it calculates the weapon id the player has shot with
    returns the specific delay that is attached to that weapon.
    */
    auto wepNr = player.getCurrentWeapon();

    return player.getWeapon(wepNr).weaponCooldown;
};

bool runGameTaak::isStartMessage(uint32_t message){
    return ((message >> 9) & 1) && ((message >> 25) & 1);
};

bool runGameTaak::isGameTimeMessage(uint32_t message){
    return (((message >> 9) & 1) == 0) && (((message >> 25) & 1) == 0 );
};

uint32_t runGameTaak::getEnemyID(uint32_t message){
    message <<= 17;
    message >>= 27;
    return message;
}


void runGameTaak::sendMessage(uint32_t m){
    messagepool.write(m); messageFlag.set();
}

void runGameTaak::inputMessage(buttonid id){
    inputChannel.write(id);
}