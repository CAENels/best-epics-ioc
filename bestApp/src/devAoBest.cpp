#include <aoRecord.h>
#include <epicsExport.h>

#include <dbScan.h>
#include <dbDefs.h>
#include <dbAccess.h>
#include <devSup.h>

#include <stdlib.h>
#include <stdio.h>

#include "best.h"

static long init_record(void* precord){
    aoRecord *pao = (aoRecord*) precord;
    printf("%s:\n", pao->name);
    return 0;
}

static long write_ao(void* precord){
    aoRecord *pao = (aoRecord*) precord;
    char pvName[128];
    stripEpicsIocName(pvName, pao->name);
    printf("%s: %s\n", __FUNCTION__, pvName);

    writeBest(pvName, DOUBLE, (void*)&pao->val);
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
