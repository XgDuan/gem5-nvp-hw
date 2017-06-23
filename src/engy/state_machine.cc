//
// Created by lf-z on 3/13/17.
//

#include "engy/state_machine.hh"

BaseEnergySM::BaseEnergySM(const Params *p)
    : SimObject(p), mgmt(NULL)
{

}

void BaseEnergySM::broadcastMsg(const EnergyMsg &msg)
{
    mgmt->broadcastMsgAsEvent(msg);
}

SimpleEnergySM::SimpleEnergySM(const Params *p)
        : BaseEnergySM(p), state(SimpleEnergySM::State::STATE_INIT)
{

}

void SimpleEnergySM::init()
{
    state = State::STATE_POWERON;
}

void SimpleEnergySM::update(double _energy)
{
    EnergyMsg msg;
    msg.val = 0;

    if (state == STATE_INIT) {
        state = STATE_POWERON;
    } else if (state == STATE_POWERON && _energy < 0) {
        state = STATE_POWEROFF;
        msg.type = MsgType ::POWEROFF;
        broadcastMsg(msg);
    } else if (state == STATE_POWEROFF && _energy > 0) {
        state = STATE_POWERON;
        msg.type = MsgType::POWERON;
        broadcastMsg(msg);
    }
}

BaseEnergySM *
BaseEnergySMParams::create()
{
    return new BaseEnergySM(this);
}

SimpleEnergySM *
SimpleEnergySMParams::create()
{
    return new SimpleEnergySM(this);
}
