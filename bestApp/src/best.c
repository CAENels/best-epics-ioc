#include "best.h"

int stripEpicsIocName(char *pvName, const char *name){
    strcpy(pvName, name);
    while(sscanf(pvName, "%*[^:]:%s", pvName) > 0);
    return 0;
}

int readBest(char *pvName, retType_t type, void* payload, int count){

    //=========================================================================
    if( strcmp(pvName, "PosX") == 0 ) {
        printf("%s: PosX\n", __FUNCTION__);
        double *buffer = (double*)malloc(count*sizeof(double));
        int fd = open(FILE_DISP, O_RDONLY | O_SYNC);
        printf("fd: %d\n", fd);
        int rv = read(fd, buffer, count*sizeof(double));
        printf("rv: %d\n", rv);

        int i = 0;
        for(i = 0; i < 32; i++){
            printf("%d: %ld\n", i, buffer[i]);
        }

        close(fd);
        free(buffer);
    }
    //=========================================================================
    else {
        puts("unknown variable");
    }
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
        int fd = open(FILE_MBOX, O_RDWR | O_SYNC);
        printf("fd: %d\n", fd);
        struct mail comm;
        comm.cmd = CMD_SIG_DEMUX << 8 | CMD_WRITE;
        memset(comm.payload, 0, 28);
        comm.payload[0] = *(short int*)payload;

        // emit io control
        int ret = ioctl(fd, IOCTL_MAIL_COMM, &comm);
        printf("ioctl(): %d\n", ret);
        close(fd);
    }
    //=========================================================================
    else if( strcmp(pvName, "Out") == 0 ){
        double* d_ptr = (double*)payload;
        printf("Out %lf %lf %lf %lf\n", *d_ptr, *(d_ptr+1), *(d_ptr+2), *(d_ptr+3));
        int fd = open(FILE_PREDAC, O_RDWR | O_SYNC);
        printf("fd: %d\n", fd);
        struct dac_data dac = {
            .ch0 = *d_ptr,
            .ch1 = *(d_ptr+1),
            .ch2 = *(d_ptr+2),
            .ch3 = *(d_ptr+3),
            .control = 0x1
        };

        int ret = ioctl(fd, IOCTL_WRITE_DAC, &dac);
        printf("ioctl(): %d\n", ret);

        close(fd);
    }
    //=========================================================================
    else {
        puts("unknown variable");
    }
}
