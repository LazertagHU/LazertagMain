#ifndef _TRANSFERHITSCONTROLTAAK_HPP
#define _TRANSFERHITSCONTROLTAAK_HPP

#include "PlayerInfo.hpp"
#include "rtos.hpp"

class TransferHitsControlTaak : public rtos::task<> {
private:
    rtos::flag TransferEnableFlag;
    rtos::pool< PlayerInfo > & entity_pool;
    void main() override;
    enum class states { IDLE };
    states state;
public:
    TransferHitsControlTaak( rtos::pool< PlayerInfo > & entity_pool ):
        task( "TransferHitsControlTaak" ),
	entity_pool( entity_pool ),
        TransferEnableFlag( this, "TransferEnableFlag" )
    {}

    void write();
};

#endif
