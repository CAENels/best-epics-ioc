#include <mbbiRecord.h>
#include <epicsExport.h>

#include <dbScan.h>
#include <dbDefs.h>
#include <dbAccess.h>
#include <devSup.h>

#include <stdlib.h>
#include <stdio.h>

#include "best_io.h"

static long init_record(void* precord){
    mbbiRecord *pmbbi = (mbbiRecord*) precord;
    PDEBUG(DEBUG_REC_INIT, "record name: %s\n", pmbbi->name);

    return 0;
}

static long read_mbbi(void* precord){
    char stripdName[32];
    unsigned short value;
    mbbiRecord *pmbbi = (mbbiRecord*) precord;
    PDEBUG(DEBUG_REC_PROC, "record name: %s\n", pmbbi->name);

    stripEpicsIocName(stripdName, pmbbi);
    readBest(stripdName, USHORT, &value, 0);

    pmbbi->val = value;
    pmbbi->udf = FALSE;

    return 2;
}


struct {
    long		number;
    DEVSUPFUN	dev_report;
    DEVSUPFUN	init;
    DEVSUPFUN	init_record; /*returns: (-1,0)=>(failure,success)*/
    DEVSUPFUN	get_ioint_info;
    DEVSUPFUN	read_mbbi;/*(0,2)=>(success, success no convert)*/
} devMbbiBest = {
    5,
    NULL,
    NULL,
    (long int (*)(void*))init_record,
    NULL,
    (long int (*)(void*))read_mbbi,
};


epicsExportAddress(dset, devMbbiBest);
