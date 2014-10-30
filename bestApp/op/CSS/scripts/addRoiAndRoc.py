from org.csstudio.opibuilder.scriptUtil import PVUtil
from org.csstudio.swt.xygraph.figures import XYGraph
from org.csstudio.swt.xygraph.figures import Axis
#from org.eclipse.swt.widgets import Display
import org.csstudio.swt.xygraph
#from org.csstudio.swt.xygraph.editparts import XYGraph


posX      = pvs[0]
posY      = pvs[1]
posX_HOPR = pvs[2]
posX_LOPR = pvs[3] 
posY_HOPR = pvs[4]
posY_LOPR = pvs[5]

xyGraph = widget

xyGraph.getFigure().getXYGraph().primaryXAxis.setRange(posX_LOPR.getValue().getValue(), posX_HOPR.getValue().getValue())
xyGraph.getFigure().getXYGraph().primaryYAxis.setRange(posY_LOPR.getValue().getValue(), posY_HOPR.getValue().getValue())
