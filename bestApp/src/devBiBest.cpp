#include <biRecord.h>
#include <epicsExport.h>

#include <dbScan.h>
#include <dbDefs.h>
#include <dbAccess.h>
#include <devSup.h>

#include <stdlib.h>
#include <stdio.h>

#include "best_io.h"

static long init_record_bi(void* precord){
    char stripdName[32];
    biRecord *pbi = (biRecord*) precord;
    PDEBUG(DEBUG_REC_INIT, "record name: %s\n", pbi->name);

    stripEpicsIocName(stripdName, pbi);
    initBest(stripdName, USHORT, precord);

    return 0;
}


static long read_bi(void* precord){
    char stripdName[32];
    unsigned short value;
    biRecord *pbi = (biRecord*) precord;
    PDEBUG(DEBUG_REC_PROC, "record name: %s\n", pbi->name);

    stripEpicsIocName(stripdName, pbi);
    readBest(stripdName, USHORT, &value, 0);
    pbi->val = value;
    pbi->udf = FALSE;

    return 2;
}


struct {
    long num;
    DEVSUPFUN  report;
    DEVSUPFUN  init;
    DEVSUPFUN  init_record;
    DEVSUPFUN  get_ioint_info;
    DEVSUPFUN  read_bi;
    DEVSUPFUN  special_linconv;
} devBiBest = {
    6,
    NULL,
    NULL,
    (long int (*)(void*))init_record_bi,
    NULL,
    (long int (*)(void*))read_bi,
    NULL
};


epicsExportAddress(dset,devBiBest);
