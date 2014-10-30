from org.csstudio.opibuilder.scriptUtil import PVUtil

userPass = pvs[0]
user     = pvs[1]
passwd   = pvs[2]

if user.getValue() and passwd.getValue():
    userPass.setValue(user.getValue().getValue() +":" + passwd.getValue().getValue())
