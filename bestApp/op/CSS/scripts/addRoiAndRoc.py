from org.csstudio.opibuilder.scriptUtil import PVUtil

posX      = pvs[0]
posY      = pvs[1]
posX_HOPR = pvs[2]
posX_LOPR = pvs[3] 
posY_HOPR = pvs[4]
posY_LOPR = pvs[5]

xyGraph = widget
xAxis = xyGraph.axisList[0]
yAxis = xyGraph.axisList[1]

# set
xAxis.setRange(posX_LOPR.getValue().getValue(), posX_HOPR.getValue().getValue()); 
yAxis.setRange(posY_LOPR.getValue().getValue(), posY_HOPR.getValue().getValue()); 


