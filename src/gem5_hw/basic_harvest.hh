//
// @author: Duan Xg
// 
//

#ifndef GEM5_BASIC_HARVEST_HH
#define GEM5_BASIC_HARVEST_HH

#include "sim/sim_object.hh"
#include "params/BasicHarvest.hh"
#include "params/SimpleHarvest.hh"

#include "engy/harvest.hh"

class BasicHarvest : public BaseHarvest
{
public:
    typedef BasicHarvestParams Params;
    const Params *params() const
    {
        return reinterpret_cast<const Params *>(_params);
    }
    BasicHarvest(const Params *p) : BaseHarvest(p),_capacity(p->capacity) {}
    virtual ~BasicHarvest() {}
    virtual void init();
public:
    double energy_harvest(double energy_harvested, double energy_remained);
private:
    double _capacity;
};

#endif //GEM5_HARVEST_HH
