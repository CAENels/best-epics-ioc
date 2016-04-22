#include <aiRecord.h>
#include <epicsExport.h>

#include <dbScan.h>
#include <dbDefs.h>
#include <dbAccess.h>
#include <devSup.h>

#include <stdlib.h>
#include <stdio.h>

#include "best_io.h"

static long init_record_ai(void* precord){
    aiRecord *pai = (aiRecord*) precord;
    PDEBUG(DEBUG_REC_INIT, "record name: %s\n", pai->name);

    return 0;
}


static long read_ai(void* precord){
    double value;
    char stripdName[32];
    aiRecord *pai = (aiRecord*) precord;
    PDEBUG(DEBUG_REC_PROC, "record name: %s\n", pai->name);

    stripEpicsIocName(stripdName, pai->name);
    readBest(stripdName, DOUBLE, &value, 0);
    pai->val = value;
    pai->udf = FALSE;

    return 2;
}


struct {
    long num;
    DEVSUPFUN  report;
    DEVSUPFUN  init;
    DEVSUPFUN  init_record;
    DEVSUPFUN  get_ioint_info;
    DEVSUPFUN  read_ai;
    DEVSUPFUN  special_linconv;
} devAiBest = {
    6,
    NULL,
    NULL,
    (long int (*)(void*))init_record_ai,
    NULL,
    (long int (*)(void*))read_ai,
    NULL
};


epicsExportAddress(dset,devAiBest);
