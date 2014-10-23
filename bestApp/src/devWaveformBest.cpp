#include <waveformRecord.h>
#include <epicsExport.h>

#include <dbScan.h>
#include <dbDefs.h>
#include <dbAccess.h>
#include <devSup.h>

#include <stdlib.h>
#include <stdio.h>
#include "cantProceed.h"

#include "best.h"

double buffer[4];

static long init_record(void* precord){
    waveformRecord *pwf = (waveformRecord*) precord;
    printf("%s:\n", pwf->name);

    pwf->bptr = callocMustSucceed(pwf->nelm, dbValueSize(pwf->ftvl), "first buf");

    return 0;
}


static long read_wf(void* precord){
    waveformRecord *pwf = (waveformRecord*) precord;
    char pvName[128];
    stripEpicsIocName(pvName, pwf->name);
    printf("%s: %s\n", __FUNCTION__, pvName);
/*
    printf("%s: BPTR: %p\n", __FUNCTION__, pwf->bptr);
    printf("%s: nelm: %d\n", __FUNCTION__, pwf->nelm);
    printf("%s: @bptr: %lf\n", __FUNCTION__, *(double*)pwf->bptr);
    printf("%s: @(bptr+8): %lf\n", __FUNCTION__, *(double*)(pwf->bptr+40));
    printf("%s: val: %lf\n", __FUNCTION__, pwf->val);

    int i = 0;
    unsigned char* tmp = (unsigned char*)pwf->bptr;
    for(i = 0; i < 64; i++){
        if(i % 8 == 0) printf("\n");
        printf("%02x ", *tmp++);
    }
*/
    pwf->nord = 10;

    //writeBest(pvName, DOUBLE, (void*)pwf->bptr);
    return 0;
}


static long write_wf(void* precord){
    waveformRecord *pwf = (waveformRecord*) precord;
    char pvName[128];
    stripEpicsIocName(pvName, pwf->name);
    printf("%s: %s\n", __FUNCTION__, pvName);
/*
    printf("%s: BPTR: %p\n", __FUNCTION__, pwf->bptr);
    printf("%s: nelm: %d\n", __FUNCTION__, pwf->nelm);
    printf("%s: @bptr: %lf\n", __FUNCTION__, *(double*)pwf->bptr);
    printf("%s: @(bptr+8): %lf\n", __FUNCTION__, *(double*)(pwf->bptr+40));
    printf("%s: val: %lf\n", __FUNCTION__, pwf->val);

    int i = 0;
    unsigned char* tmp = (unsigned char*)pwf->bptr;
    for(i = 0; i < 64; i++){
        if(i % 8 == 0) printf("\n");
        printf("%02x ", *tmp++);
    }

    pwf->nord = 10;
*/
    //writeBest(pvName, DOUBLE, (void*)pwf->bptr);
    return 0;
}




struct {
    long num;
    DEVSUPFUN  report;
    DEVSUPFUN  init;
    DEVSUPFUN  init_record;
    DEVSUPFUN  get_ioint_info;
    DEVSUPFUN  read_wf;
    DEVSUPFUN  write_wf;
    DEVSUPFUN  special_linconv;
} devWaveformBest = {
    6,
    NULL,
    NULL,
    (long int (*)(void*))init_record,
    NULL,
    (long int (*)(void*))read_wf,
    (long int (*)(void*))write_wf,
    NULL
};


epicsExportAddress(dset,devWaveformBest);
