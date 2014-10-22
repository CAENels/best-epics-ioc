#include <aiRecord.h>
#include <epicsExport.h>

#include <dbScan.h>
#include <dbDefs.h>
#include <dbAccess.h>
#include <devSup.h>

#include <stdlib.h>
#include <stdio.h>

static long init_record_ai(void* precord){
    aiRecord *pao = (aiRecord*) precord;
    printf("%s:\n", pao->name);
    pao->val = 1.0;
    return 0;
}

static long read_ai(void* precord){
    aiRecord *pao = (aiRecord*) precord;
    printf("%s:\n", __FUNCTION__);
    pao->rval = 2.0;
    return 0;
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
