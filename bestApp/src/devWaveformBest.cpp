#include <waveformRecord.h>
#include <epicsExport.h>

#include <dbScan.h>
#include <dbDefs.h>
#include <dbAccess.h>
#include <devSup.h>

#include <stdlib.h>
#include <stdio.h>
#include "cantProceed.h"

#include "best_io.h"


static long init_record(void* precord){
    char stripdName[32];
    waveformRecord *pwf = (waveformRecord*) precord;
    PDEBUG(DEBUG_REC_INIT, "record name: %s\n", pwf->name);

    pwf->dpvt = callocMustSucceed(pwf->nelm, dbValueSize(pwf->ftvl), "first buf");
    PDEBUG(DEBUG_REC_INIT, "record: %s, alloc at: %p\n", pwf->name, pwf->dpvt);

    stripEpicsIocName(stripdName, pwf);
    initBest(stripdName, DOUBLE, precord);

    return 0;
}


static long read_wf(void* precord){
    waveformRecord *pwf = (waveformRecord*) precord;
    char stripdName[32];
    PDEBUG(DEBUG_REC_PROC, "record name: %s\n", pwf->name);

    stripEpicsIocName(stripdName, pwf);

    pwf->nord = pwf->nelm;
    if ( strcmp(stripdName, "PreDAC0:Out") == 0){
        writeBest(stripdName, DOUBLE, (void*)pwf->bptr);
    } else {
        readBest(stripdName, DOUBLE, (void*)pwf->dpvt, pwf->nelm);
        pwf->bptr = pwf->dpvt;
    }


    return 0;
}



struct wfdset { /* waveform dset */
        long            number;
        DEVSUPFUN       dev_report;
        DEVSUPFUN       init;
        DEVSUPFUN       init_record; /*returns: (-1,0)=>(failure,success)*/
        DEVSUPFUN       get_ioint_info;
        DEVSUPFUN       read_wf; /*returns: (-1,0)=>(failure,success)*/
};

struct wfdset devWaveformBest = {
    5,
    NULL,
    NULL,
    (long int (*)(void*))init_record,
    NULL,
    (long int (*)(void*))read_wf
};


epicsExportAddress(dset,devWaveformBest);
