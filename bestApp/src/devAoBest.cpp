#include <aoRecord.h>
#include <epicsExport.h>

#include <dbScan.h>
#include <dbDefs.h>
#include <dbAccess.h>
#include <devSup.h>

#include <stdlib.h>
#include <stdio.h>

#include "best_io.h"

static long init_record(void* precord){
    char stripdName[32];
    aoRecord *pao = (aoRecord*) precord;
    PDEBUG(DEBUG_REC_INIT, "record name: %s\n", pao->name);

    stripEpicsIocName(stripdName, pao->name);
    initBest(stripdName, DOUBLE, precord);

    return 0;
}

static long write_ao(void* precord){
    char stripdName[32];
    aoRecord *pao = (aoRecord*) precord;
    PDEBUG(DEBUG_REC_PROC, "record name: %s\n", pao->name);

    stripEpicsIocName(stripdName, pao->name);
    writeBest(stripdName, DOUBLE, (void*)&pao->val);

    return 0;
}



struct {
    long num;
    DEVSUPFUN  report;
    DEVSUPFUN  init;
    DEVSUPFUN  init_record;
    DEVSUPFUN  get_ioint_info;
    DEVSUPFUN  write_ao;
    DEVSUPFUN  special_linconv;
} devAoBest = {
    6,
    NULL,
    NULL,
    (long int (*)(void*))init_record,
    NULL,
    (long int (*)(void*))write_ao,
    NULL
};




epicsExportAddress(dset,devAoBest);
