#include <stringoutRecord.h>
#include <epicsExport.h>

#include <dbScan.h>
#include <dbDefs.h>
#include <dbAccess.h>
#include <devSup.h>

#include <stdlib.h>
#include <stdio.h>

#include "best.h"

static long init_record(void* precord){
    stringoutRecord *psor = (stringoutRecord*) precord;
    PDEBUG(DEBUG_REC_INIT, "record name: %s\n", psor->name);

    return 0;
}

static long write_stringout(void* precord){
    char stripdName[32];
    stringoutRecord *psor = (stringoutRecord*) precord;
    PDEBUG(DEBUG_REC_PROC, "record name: %s\n", psor->name);

    stripEpicsIocName(stripdName, psor->name);
    writeBest(stripdName, STRING, (void*)&psor->val);

    return 0;
}


struct { /* stringout input dset */
    long		number;
    DEVSUPFUN	dev_report;
    DEVSUPFUN	init;
    DEVSUPFUN	init_record; /*returns: (-1,0)=>(failure,success)*/
    DEVSUPFUN	get_ioint_info;
    DEVSUPFUN	write_stringout;/*(-1,0)=>(failure,success)*/
} devStringoutBest = {
    6,
    NULL,
    NULL,
    (long int (*)(void*))init_record,
    NULL,
    (long int (*)(void*))write_stringout
};


epicsExportAddress(dset,devStringoutBest);
