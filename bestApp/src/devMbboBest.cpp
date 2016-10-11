#include <mbboRecord.h>
#include <epicsExport.h>

#include <dbScan.h>
#include <dbDefs.h>
#include <dbAccess.h>
#include <devSup.h>

#include <stdlib.h>
#include <stdio.h>

#include "best_io.h"

static long init_record(void* precord){
    mbboRecord *pmbbo = (mbboRecord*) precord;
    PDEBUG(DEBUG_REC_INIT, "record name: %s\n", pmbbo->name);

    return 0;
}

static long write_mbbo(void* precord){
    char stripdName[32];
    mbboRecord *pmbbo = (mbboRecord*) precord;
    PDEBUG(DEBUG_REC_PROC, "record name: %s\n", pmbbo->name);

    stripEpicsIocName(stripdName, pmbbo);
    return writeBest(stripdName, USHORT, (void*)&pmbbo->val);
}


struct { /* multi bit binary output dset */
    long		number;
    DEVSUPFUN	dev_report;
    DEVSUPFUN	init;
    DEVSUPFUN	init_record;  /*returns: (0,2)=>(success,success no convert)*/
    DEVSUPFUN	get_ioint_info;
    DEVSUPFUN	write_mbbo; /*returns: (0,2)=>(success,success no convert)*/
} devMbboBest = {
    5,
    NULL,
    NULL,
    (long int (*)(void*))init_record,
    NULL,
    (long int (*)(void*))write_mbbo,
};


epicsExportAddress(dset,devMbboBest);
