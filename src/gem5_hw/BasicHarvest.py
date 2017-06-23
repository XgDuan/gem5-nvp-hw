from m5.SimObject import SimObject
from BaseHarvest import BaseHarvest
from m5.params import *
from m5.proxy import *

class BasicHarvest(BaseHarvest):
    type = 'BasicHarvest'
    cxx_header = "gem5_hw/basic_harvest.hh"
    capacity = Param.Float(50000, "capacity of energy system, default 50000")