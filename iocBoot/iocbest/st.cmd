#!../../bin/linux-x86_64/best

## You may have to change best to something else
## everywhere it appears in this file

#< envPaths

## Register all support components
dbLoadDatabase("../../dbd/best.dbd",0,0)
best_registerRecordDeviceDriver(pdbbase) 

## Load record instances
dbLoadRecords("../../db/best.db","P=best0")

iocInit()

## Start any sequence programs
#seq sncbest,"user=jan"
