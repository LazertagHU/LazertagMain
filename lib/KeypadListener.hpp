#ifndef _KEYPADLISTENER_HPP
#define _KEYPADLISTENER_HPP



class keypadListener{
private:

    
public:
    /// \brief
    /// Purely virtual and abstract class used for the listener patern.
    virtual void keyPressed(char key) = 0;

};

#endif


