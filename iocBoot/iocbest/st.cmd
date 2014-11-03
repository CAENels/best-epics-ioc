#!/opt/CAENels/BEST/best_epics_ioc/bin/linux-x86_64/best

## You may have to change best to something else
## everywhere it appears in this file

# < /opt/CAENels/BEST/best_epics_ioc/iocBoot/iocbest/envPaths

## Register all support components
dbLoadDatabase("/opt/CAENels/BEST/best_epics_ioc/dbd/best.dbd",0,0)
best_registerRecordDeviceDriver(pdbbase) 

## Load record instances
dbLoadRecords("/opt/CAENels/BEST/best_epics_ioc/db/best.db","P=best0")

iocInit()

## Start any sequence programs
#seq sncbest,"user=jan"
