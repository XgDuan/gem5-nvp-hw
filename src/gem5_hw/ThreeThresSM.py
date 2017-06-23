from m5.SimObject import SimObject
from BaseEnergySM import BaseEnergySM
from m5.params import *
from m5.proxy import *

class ThreeThresSM(BaseEnergySM):
    type = 'ThreeThresSM'
    cxx_header = "gem5_hw/three_thres.hh"
    thres_high = Param.Float(20000, "high threshold of energy state machine")
    thres_low = Param.Float(10000, "low threshold of energy state machine")
