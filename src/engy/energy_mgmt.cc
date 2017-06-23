//
// Created by lf-z on 12/28/16.
//
#include <fstream>
#include "engy/energy_mgmt.hh"
#include "engy/state_machine.hh"
#include "engy/harvest.hh"
#include "debug/EnergyMgmt.hh"
#include "sim/eventq.hh"

EnergyMgmt::EnergyMgmt(const Params *p)
        : SimObject(p),
          time_unit(p->energy_time_unit),
          energy_remained(0),
          event_msg(this, false, Event::Energy_Pri),
          event_energy_harvest(this, false, Event::Energy_Pri),
          state_machine(p->state_machine),
          harvest_module(p->harvest_module),
          _path_energy_profile(p->path_energy_profile)
{
    msg_togo.resize(0);
}

EnergyMgmt::~EnergyMgmt()
{

}

void EnergyMgmt::init()
{
    /* Read energy profile. */
    energy_harvest_data = readEnergyProfile();
    /* Reset energy remained to 0. */
    energy_remained = 0;
    /* Set mgmt pointer in state machine. */
    if (state_machine) {
        state_machine->mgmt = this;
    }

    DPRINTF(EnergyMgmt, "Energy Management module initialized!\n");
    DPRINTF(EnergyMgmt, "Energy profile: %s (Time unit: %d ticks)\n",
            _path_energy_profile.c_str(), time_unit);

    /* Trigger first energy harvest event here */
    energyHarvest();
}

int EnergyMgmt::consumeEnergy(double val)
{
    /* Todo: Pass the module which consumed the energy to this function. (Or DPRINTF in the module which consumes energy) */
    /* Consume energy if val > 0, and harvest energy if val < 0 */
    if (val > 0) {
        energy_remained -= val;
        DPRINTF(EnergyMgmt, "Energy %lf is consumed by xxx. Energy remained: %lf\n", val, energy_remained);
    } else {
        energy_remained = harvest_module->energy_harvest(-val, energy_remained);
        DPRINTF(EnergyMgmt, "Energy %lf is harvested. Energy remained: %lf\n", -val, energy_remained);
    }

    state_machine->update(energy_remained);

    return 1;
}

void EnergyMgmt::broadcastMsg()
{
    /* Broadcast the first message in the msg queue. */
    _meport.broadcastMsg(msg_togo[0]);
    /* Delete the message we broadcast. */
    msg_togo.erase(msg_togo.begin());
    /* If there are still messages, schedule a new event. */
    if (!msg_togo.empty())
        schedule(event_msg, curTick());
}

int EnergyMgmt::broadcastMsgAsEvent(const EnergyMsg &msg)
{
    msg_togo.push_back(msg);
    /* Trigger first msg in the current tick. */
    if (msg_togo.size() == 1)
        schedule(event_msg, curTick());
    return 1;
}

int EnergyMgmt::handleMsg(const EnergyMsg &msg)
{
    /* msg type should be 0 here, for 0 represents energy consuming, */
    /* and EnergyMgmt module can only handle energy consuming msg*/
    if (msg.type)
        return 0;

    return consumeEnergy(msg.val);
}

std::vector<double> EnergyMgmt::readEnergyProfile()
{
    std::vector<double> data;
    data.resize(0);
    /* Return empty vector is no energy profile is provided. */
    if (_path_energy_profile == "")
        return data;

    double temp;
    std::ifstream fin;
    fin.open(_path_energy_profile.c_str());
    /* Read energy profile and store the data into vector. */
    while (fin>>temp) {
        data.push_back(temp);
    }
    /* Reverse the energy harvest queue so that the first energy unit pops first */
    reverse(data.begin(), data.end());
    fin.close();
    return data;
}

void EnergyMgmt::energyHarvest()
{
    /* Add harvested energy into capacity. */
    if (energy_harvest_data.empty())
        return;

    double energy_val = energy_harvest_data.back();
    consumeEnergy(-energy_val);
    energy_harvest_data.pop_back();

    /* Trigger the next harvest function. */
    schedule(event_energy_harvest, curTick() + time_unit);

    // DPRINTF(EnergyMgmt, "Energy %lf harvested.\n", energy_val);
}

EnergyMgmt *
EnergyMgmtParams::create()
{
    return new EnergyMgmt(this);
}
