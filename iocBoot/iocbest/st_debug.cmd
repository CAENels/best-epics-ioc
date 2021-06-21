#!/home/best/Desktop/bibest/best-epics-ioc/bin/linux-x86_64/best

## You may have to change best to something else
## everywhere it appears in this file

# < /home/best/Desktop/bibest/best-epics-ioc/iocBoot/iocbest/envPaths

## Register all support components
dbLoadDatabase("/home/best/Desktop/bibest/best-epics-ioc/dbd/best.dbd",0,0)
best_registerRecordDeviceDriver(pdbbase) 

## Load record instances
dbLoadRecords("/home/best/Desktop/bibest/best-epics-ioc/db/best.db","P=best0")

iocInit()

## Start any sequence programs
#seq sncbest,"user=jan"
