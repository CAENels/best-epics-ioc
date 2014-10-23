#include "best.h"

int stripEpicsIocName(char *pvName, const char *name){
    strcpy(pvName, name);
    while(sscanf(pvName, "%*[^:]:%s", pvName)>1);
    return 0;
}



int writeBest(char *pvName, retType_t type, void* payload){
    int dacCh;

    //=========================================================================
    if( sscanf(pvName, "DACoutCh%d", &dacCh) == 1 ){
        printf("DAC Out Channel: %d, value: %lf\n", dacCh, *(double*)payload);
    }
    //=========================================================================
    else {
        puts("unknown variable");
    }
}
