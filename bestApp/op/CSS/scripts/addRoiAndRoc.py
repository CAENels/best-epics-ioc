###############################################################################
##              ____      _      _____   _   _          _                    ##
##             / ___|    / \    | ____| | \ | |   ___  | |  ___              ##
##            | |       / _ \   |  _|   |  \| |  / _ \ | | / __|             ##
##            | |___   / ___ \  | |___  | |\  | |  __/ | | \__ \             ##
##             \____| /_/   \_\ |_____| |_| \_|  \___| |_| |___/             ##
##                                                                           ##
###############################################################################

# (c) 2014, CAEN ELS d.o.o.

from org.csstudio.opibuilder.scriptUtil import PVUtil
from org.csstudio.swt.xygraph.figures import XYGraph
from org.csstudio.swt.xygraph.figures import Axis
from org.csstudio.opibuilder.scriptUtil import DataUtil


posX      = pvs[0]
posY      = pvs[1]
posX_HOPR = pvs[2]
posX_LOPR = pvs[3] 
posY_HOPR = pvs[4]
posY_LOPR = pvs[5]
roiXgraph = pvs[6]
roiYgraph = pvs[7]
roiXpv    = pvs[8]
roiYpv    = pvs[9]
rocXgraph = pvs[10]
rocYgraph = pvs[11]
rocPV     = pvs[12]

xyGraph = widget

# axes limits from PV
xyGraph.getFigure().getXYGraph().primaryXAxis.setRange(posX_LOPR.getValue().getValue(), posX_HOPR.getValue().getValue())
xyGraph.getFigure().getXYGraph().primaryYAxis.setRange(posY_LOPR.getValue().getValue(), posY_HOPR.getValue().getValue())


xArray = DataUtil.createDoubleArray(4);
yArray = DataUtil.createDoubleArray(4);

# draw roi
xArray[0] = -roiXpv.getValue().getValue()
xArray[1] =  roiXpv.getValue().getValue()
xArray[2] =  roiXpv.getValue().getValue()
xArray[3] = -roiXpv.getValue().getValue()

yArray[0] =  roiYpv.getValue().getValue()
yArray[1] =  roiYpv.getValue().getValue()
yArray[2] = -roiYpv.getValue().getValue()
yArray[3] = -roiYpv.getValue().getValue()
roiXgraph.setValue(xArray)
roiYgraph.setValue(yArray)


#draw roc
rocDouble = rocPV.getValue().getValue() / 100.0
rocXArray = DataUtil.createDoubleArray(4);
rocYArray = DataUtil.createDoubleArray(4);

for i in range(4):
    rocXArray[i] = xArray[i]*rocDouble
    rocYArray[i] = yArray[i]*rocDouble
    
rocXgraph.setValue(rocXArray)
rocYgraph.setValue(rocYArray)