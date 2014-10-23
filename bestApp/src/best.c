#include "best.h"

int stripEpicsIocName(char *pvName, const char *name){
    strcpy(pvName, name);
    while(sscanf(pvName, "%*[^:]:%s", pvName) > 0);
    return 0;
}



int writeBest(char *pvName, retType_t type, void* payload){
    int dacCh;

    //=========================================================================
    if( sscanf(pvName, "DACoutCh%d", &dacCh) == 1 ){
        printf("DAC Out Channel: %d, value: %lf\n", dacCh, *(double*)payload);
    }
    //=========================================================================
    else if( strcmp(pvName, "OutMux") == 0 ){
        printf("Out Mux %d\n", *(short int*)payload);
    }
    //=========================================================================
    else if( strcmp(pvName, "Out") == 0 ){
        double* d_ptr = (double*)payload;
        printf("Out %lf %lf %lf %lf\n", *d_ptr, *(d_ptr+1), *(d_ptr+2), *(d_ptr+3));
    }
    //=========================================================================
    else {
        puts("unknown variable");
    }
}
