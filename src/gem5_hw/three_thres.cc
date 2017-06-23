//
// Created by lf-z on 3/16/17.
//

#include "gem5_hw/three_thres.hh"
#include "debug/EnergyMgmt.hh"

ThreeThresSM::ThreeThresSM(const Params *p)
    : BaseEnergySM(p), state(ThreeThresSM::State::STATE_INIT),
      thres_high(p->thres_high), thres_low(p->thres_low)
{

}

void ThreeThresSM::init()
{
    state = ThreeThresSM::State::STATE_POWEROFF;
    EnergyMsg msg;
    msg.val = 0;
    msg.type = MsgType::POWEROFF;
    broadcastMsg(msg);
}

void ThreeThresSM::update(double _energy)
{
    EnergyMsg msg;
    msg.val = 0;

    if (state == STATE_INIT) {
        state = STATE_POWEROFF;
    } else if (state != STATE_HIGHFREQ && _energy > thres_high) {
        DPRINTF(EnergyMgmt, "State change: ->high freq state=%d, _energy=%lf, thres_low=%lf\n", state, _energy, thres_low);
        state = STATE_HIGHFREQ;
        msg.type = MsgType::HIGHFREQ;
        broadcastMsg(msg);
    } else if (state == STATE_HIGHFREQ && _energy < thres_high) {
        DPRINTF(EnergyMgmt, "State change: high freq->low freq state=%d, _energy=%lf, thres_high=%lf\n", state, _energy, thres_high);
        state = STATE_LOWFREQ;
        msg.type = MsgType::LOWFREQ;
        broadcastMsg(msg);
    } else if (state != STATE_POWEROFF && _energy < thres_low){
        DPRINTF(EnergyMgmt, "State change: ->off state=%d, _energy=%lf, thres_high=%lf\n", state, _energy, thres_high);
        state = STATE_POWEROFF;
        msg.type = MsgType::POWEROFF;
        broadcastMsg(msg);
    }

}

ThreeThresSM *
ThreeThresSMParams::create()
{
    return new ThreeThresSM(this);
}
