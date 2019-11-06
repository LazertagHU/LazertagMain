#ifndef _INPUTLISTENER_HPP
#define _INPUTLISTENER_HPP

/// \brief
/// enum class with all the button id's
enum class buttonid
{
    zeroButton, oneButton, twoButton, threeButton,
    fourButton, fiveButton, sixButton, sevenButton,
    eightButton, nineButton, aButton, bButton,
    cButton, dButton, eButton, fButton, starButton,
    hastagButton
};

class inputListener{ 
public:
    /// \brief
    /// Purely virtual and abstract class used for the listener patern.
    virtual void inputMessage(buttonid id) = 0;

};

#endif


