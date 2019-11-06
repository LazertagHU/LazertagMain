#ifndef RUN_GAME_TAAK_HPP
#define RUN_GAME_TAAK_HPP

#include "DisplayTaak.hpp"
#include "hwlib.hpp"
#include "../../hwlib/library/hwlib.hpp"
#include "rtos.hpp"
#include "../../rtos/rtos.hpp"
#include "weapon.hpp"
#include "PlayerInfo.hpp"
#include "SendTask.hpp"
#include "pause_detector.hpp"
#include "msg_decoder.hpp"
//#include "TransferHitsControlTaak.hpp"
#include "InputControlTaak.hpp"
#include "SpeakerTaak.hpp"
#include "hit.hpp"

class runGameTaak : public rtos::task<>, public msg_listener, public inputListener
{
private:

    enum class preGame_states_t 
    {
        IDLE, WAIT_FOR_PLAYER_NUMBER, WAIT_FOR_WEAPON_NUMBER, 
        ENTER_TIME_REMAINING, SEND_COMMAND_STATE,
        START_GAME_TRANSMISSION_STATE, AFTELLEN
    };

    enum class gameRunning_states_t
    {
        ALIVE, HIT, WEAPON_RELOAD, WEAPON_COOLDOWN, GAME_OVER
    };

    enum class postGame_states_t
    {
        IDLE
    };

    displayTaak&                display;
    sendTask&                   transmitter;
    //TransferHitsControlTaak&    transfer;
    speakerTaak&		        speaker;
    inputControlTaak            inputControl;
    rtos::channel<buttonid, 10> inputChannel;
    rtos::flag                  messageFlag;
    rtos::pool<uint32_t>        messagepool;
    rtos::pool<playerInfo>&     playerpool;
    rtos::clock                 secondClock;
    rtos::timer                 delayTimer;
    buttonid                    bnID;


    hit                         hits[20];
    unsigned int                hitAmount;
    uint32_t                    command             = 0b1'00000'10000'00000'1'00000'10000'00000;
    uint32_t                    startCommand        = 0b1'00000'10000'00000'1'00000'10000'00000;
    uint32_t                    setTimeCommand      = 0b1'00000'00000'00000'1'00000'00000'00000;
    uint32_t                    shootCommand        = 0b1'00000'00000'00000'1'00000'00000'00000;
    uint32_t                    msg;
    bool                        gameLeader          = false;
    int                         countdown           = 0;
    int                         remainingGameTime   = 0;
    int                         bullets             = 0;

    /// \brief
    /// This Function handles the pregame functionality
    /// \details
    /// With this function, you can set the player id ( unless you are the game leader. In this case the player id will be #0), 
    /// weapon type you want to play with, and the game leader can set the game duration and start command.
    /// The slaves can receive the game time and start command and will start at the same time as the other salves due to
    /// a modulated game start message in which the countdown is located.
    void preGame();

    /// \brief
    /// Main function for the lasergame
    /// \details
    /// The player can either be alive, hit, reloading and on a shoot cooldown. This function handles all this and keeps track of the score 
    /// and gametime. When either the player's HP is reduced to 0 or the game time is zero, this function will end.
    void gameRunning();

    /// \brief
    /// Post game function for the lasergame
    /// \details
    /// For now, this function handles only the transferring of the hits to a terminal. More functionality can be added later.
    /// If the reloadbutton is pressed, the game will be restarted.
    void postGame();

    /// \brief
    /// The main() of the RunGame task.
    /// \details
    /// This task controls the game setup and the game whilst it's running
    /// It's code is based on the runGameTaak std and can be found in the
    /// google drive map.

    void main() override;

    /// \brief
    /// Returns the decoded game time.
    /// \details
    /// The game leader can send a specific command which states the length
    /// of the upcoming game.
    /// This function decodes that command and returns the gametime. 
    uint32_t computeGameTime(uint32_t msg);

    /// \brief
    /// Returns the decoded countdown.
    /// \details
    /// The game leader can send a specific command which tells the player 
    /// in how many seconds the game is going to start. This way, every
    /// player starts at about the same time.
    /// 
    /// This function decodes that message and translates it in to the proper
    /// countdown.
    uint32_t computeCountdown(uint32_t msg);

    /// \brief
    /// Modifies startCommand to the correct start countdown
    /// \details
    /// The game leader can send a specific command which tells the player 
    /// in how many seconds the game is going to start. This way, every
    /// player starts at about the same time.
    /// 
    /// This function calculates the specific command that needs to be send 
    /// by the game leader at a specific countdown timer.
    uint32_t computeStartCommand(uint32_t countdown, uint32_t command);

    /// \brief
    /// Returns pressed chars on keypad.
    /// \details
    /// This function adds two inputs of the keypad together.
    /// The user has the option to return when no numbers are entered
    /// or when one number is entered. this way the user can return
    /// 1 - 9 and then return out of the function.
    int waitForInput(char place);
   
    /// \brief
    /// Returns XOR checksum
    /// \details
    /// This function calculates the checksum of a specific command.
    /// This is done by XOR-ing specific bits
    uint32_t calculateCheckSum(uint32_t input);

    /// \brief
    /// This function is used to decide wether te player is hit
    bool isHitMessage(uint32_t message);
    
    /// \brief
    /// Returns damage taken.
    /// \details
    /// This function is used to calculate the hit damage when a
    /// player is hit
    int computeHit(uint32_t message);

    /// \brief
    /// Returns after hit timeout.
    /// \details
    /// This function is used to calculate the delay that a player
    /// can't shoot or be shot after he has been shot by another player
    int computeDeathDelay(uint32_t message);


    /// \brief
    /// This function computes the shootcommand according to the weapon 
    /// and player id that the player has been chosen.
    void computeShootCommand(uint32_t & shootcommand);


    /// \brief
    /// Returns after shot timeout.
    /// \details
    /// This function is used to calculate the delay that a player
    /// can't shoot after he has shot
    int computeShootDelay();

    /// \brief
    /// This function checks if the message received is a start message;
    bool isStartMessage(uint32_t message);

    /// \brief
    /// This function checks if the message received contains the game time;
    bool isGameTimeMessage(uint32_t message);

    /// \brief
    /// Extracts and returns the enemyID from message.
    uint32_t getEnemyID(uint32_t message);

    /// \brief
    /// Funtion that writes hits
    /// \details
    /// Writes all the hits to the terminal with enemy id damage and time
    /// if no hits were registered writes no hits registered
    void write_hits();

    /// \brief
    /// Function to add a hit to the hit array
    /// \details
    /// The hit array is used to keep track of whom the player has been hit by. With this function a hit can be added to the array
    void addHit( int enemyID, int damage, int time );
    

public:
    /// \brief
    /// The default constructor of RunGameTaak
    /// \details
    /// Names its task, binds all given paramaters, inits own objects and starts its 1s clock.
    runGameTaak(int prio,
        const char * name,
        displayTaak & display, 
        sendTask& transmitter,
        //TransferHitsControlTaak& transfer,
        rtos::pool<playerInfo> & playerpool,
        speakerTaak & speaker
    ):
        task(prio, name ),
        display(display),
        transmitter(transmitter),
        //transfer(transfer),
	    speaker(speaker),
        inputControl(7, this, "InputControlTaak"),
        inputChannel(this, "inputChannel"),
        messageFlag(this, "messageFlag"),
        messagepool("messagepool"),
        playerpool(playerpool),
        secondClock(this, 1'000'000, "secondClock"),
        delayTimer(this, "delayTimer")
    {
        hitAmount = 0;
    }

    /// \brief
    /// Interface for writing recieved messages
    /// \details    
    /// Public function to write the recieved message to. This function internally uses a pool and a flag to save this incoming data.  
    void sendMessage(uint32_t m ) override;

    /// \brief
    /// Interface for writing recieved commands
    /// \details    
    /// Public function to write commands to. This function internally uses a channel as waitable to save this incoming data.  
    void inputMessage(buttonid id)override;
};



#endif
