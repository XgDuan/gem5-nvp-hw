//
// @author: Duan Xg
//

#ifndef GEM5_THREE_THRES_HH
#define GEM5_THREE_THRES_HH

#include "engy/state_machine.hh"
#include "params/ThreeThresSM.hh"


class ThreeThresSM : public BaseEnergySM
{

public:
    typedef ThreeThresSMParams Params;
    const Params *params() const
    {
        return reinterpret_cast<const Params *>(_params);
    }
    ThreeThresSM(const Params *p);
    ~ThreeThresSM() {}
    virtual void init();
    virtual void update(double _energy);

    enum State {
        STATE_INIT = 0,
        STATE_POWEROFF = 1,
        STATE_HIGHFREQ = 2,
        STATE_LOWFREQ = 3,
    };

    enum MsgType {
        CONSUMEENERGY = 0,
        POWEROFF = 1,
        HIGHFREQ = 3,
        LOWFREQ = 4
    };

protected:
    State state;
    double thres_high;
    double thres_low;

};
#endif //GEM5_THREE_THRES_HH
