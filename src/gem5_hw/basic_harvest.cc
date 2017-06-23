//
// @author: Duan Xg
// 
//

#include "basic_harvest.hh"
#include "debug/EnergyMgmt.hh"

double BasicHarvest::energy_harvest(double energy_harvested, double energy_remained)
{
	if(energy_remained + energy_harvested > _capacity)
	{
		DPRINTF(EnergyMgmt,"energy full: %f+%f->%f\n",energy_remained,energy_harvested,_capacity);
		return _capacity;
	}
    return (energy_remained + energy_harvested);
}

void BasicHarvest::init()
{
    DPRINTF(EnergyMgmt, "basic energy harvest module is used in this system.\n");
}


BasicHarvest *
BasicHarvestParams::create()
{
    return new BasicHarvest(this);
}
