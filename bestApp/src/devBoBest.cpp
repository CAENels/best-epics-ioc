#include <boRecord.h>
#include <epicsExport.h>

#include <dbScan.h>
#include <dbDefs.h>
#include <dbAccess.h>
#include <devSup.h>

#include <stdlib.h>
#include <stdio.h>

#include "best_io.h"

static long init_record(void* precord){
    boRecord *pbo = (boRecord*) precord;
    PDEBUG(DEBUG_REC_INIT, "record name: %s\n", pbo->name);

    return 0;
}

static long write_bo(void* precord){
    char stripdName[32];
    boRecord *pbo = (boRecord*) precord;
    PDEBUG(DEBUG_REC_PROC, "record name: %s\n", pbo->name);

    stripEpicsIocName(stripdName, pbo->name);
    writeBest(stripdName, BOOL, (void*)&pbo->val);

    return 0;
}


struct { /* binary output dset */
    long		number;
    DEVSUPFUN	dev_report;
    DEVSUPFUN	init;
    DEVSUPFUN	init_record;  /*returns:(0,2)=>(success,success no convert*/
    DEVSUPFUN	get_ioint_info;
    DEVSUPFUN	write_bo;/*returns: (-1,0)=>(failure,success)*/
} devBoBest = {
    6,
    NULL,
    NULL,
    (long int (*)(void*))init_record,
    NULL,
    (long int (*)(void*))write_bo
};


epicsExportAddress(dset,devBoBest);
