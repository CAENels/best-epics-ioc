#include <stringinRecord.h>
#include <epicsExport.h>

#include <dbScan.h>
#include <dbDefs.h>
#include <dbAccess.h>
#include <devSup.h>

#include <stdlib.h>
#include <stdio.h>

#include "best_io.h"

static long init_record(void* precord){
    stringinRecord *psir = (stringinRecord*) precord;
    PDEBUG(DEBUG_REC_INIT, "record name: %s\n", psir->name);

    return 0;
}

static long read_stringin(void* precord){
    char stripName[32];
    char value[40];
    stringinRecord *psir = (stringinRecord*) precord;
    PDEBUG(DEBUG_REC_PROC, "record name: %s\n", psir->name);

    stripEpicsIocName(stripName, psir);
    readBest(stripName, STRING, &value , 0);
    strcpy(psir->val, value);
    psir->udf = FALSE;
    return 2;


}


struct { /* stringin input dset */
    long		number;
    DEVSUPFUN	dev_report;
    DEVSUPFUN	init;
    DEVSUPFUN	init_record; /*returns: (-1,0)=>(failure,success)*/
    DEVSUPFUN	get_ioint_info;
    DEVSUPFUN	read_stringin;/*(-1,0)=>(failure,success)*/
} devStringinBest = {
    6,
    NULL,
    NULL,
    (long int (*)(void*))init_record,
    NULL,
    (long int (*)(void*))read_stringin
};


epicsExportAddress(dset,devStringinBest);
