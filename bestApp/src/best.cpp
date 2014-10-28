#include "best.h"

uint32_t debug;

int stripEpicsIocName(char *pvName, const char *name){
	const char* tmp = name;
	int nrColons = 0;
	while(*tmp != 0){
		if(*tmp == ':') nrColons++; 
		*tmp++;
	}

	tmp = name;
	while(*tmp != 0){
		if(*tmp == ':')  nrColons--; 
		*tmp++;
		if(nrColons == 1) break;
	}

	strcpy(pvName, tmp);	

	return 0;
}

int readBest(char *pvName, retType_t type, void* payload, int count){

    PDEBUG(DEBUG_LOW_FUNC, "func: %s(), pv: %s\n", __FUNCTION__, pvName);

    //=========================================================================
    if( (strcmp(pvName, "BPM0:ScaleX") == 0) ||
            (strcmp(pvName, "BPM0:ScaleY") == 0) ) {

        double scaleX, scaleY;
        getBPMscaling(&scaleX, &scaleY);

        if(pvName[10] == 'X')
            *(double*)payload = scaleX;
        else
            *(double*)payload = scaleY;


        PDEBUG(DEBUG_RET_DATA, "pv: %s, data: %lf\n", pvName, *(double*)payload);

        return 0;
    }
    //=========================================================================
    if( (strcmp(pvName, "PosX") == 0) ||
            (strcmp(pvName, "PosY") == 0) ||
            (strcmp(pvName, "Int")  == 0) ) {


        double *buffer = (double*)malloc(count*sizeof(double)*DISP_NR_CH);
        double *retBuf = (double*)payload;

        int fd = open(FILE_DISP, O_RDONLY | O_SYNC);
        int rv = read(fd, buffer, count*sizeof(double)*DISP_NR_CH);

        int offset = 0;
        if( strcmp(pvName, "PosX") == 0 ) offset = DISP_POSX;
        if( strcmp(pvName, "PosY") == 0 ) offset = DISP_POSY;
        if( strcmp(pvName, "Int") == 0 ) offset = DISP_INT0;

        int i;
        for(i = 0; i < count; i++){
            //printf("%d: %lf\n", i, buffer[offset + i*DISP_NR_CH]);
            retBuf[i] = buffer[offset + i*DISP_NR_CH];
        }

        close(fd);
        free(buffer);
    }
    //=========================================================================
    else {
        puts("unknown variable");
    }

    return -1; // unknown variable
}


int writeBest(char *pvName, retType_t type, void* payload){

    PDEBUG(DEBUG_LOW_FUNC, "func: %s(), pv: %s\n", __FUNCTION__, pvName);


    //=========================================================================
    if( (strcmp(pvName, "PID:SetpointX") == 0) ||
            (strcmp(pvName, "PID:SetpointY") == 0) ||
            (strcmp(pvName, "PID:SetpointI0") == 0) ){
        int sel;
        switch(pvName[12]){
        case 'X': sel = 0; break;
        case 'Y': sel = 1; break;
        case 'I': sel = 2; break;
        }

        PDEBUG(DEBUG_SET_DATA,
               "pv: %s, setSetpoint(%d, %lf)\n",
               pvName, sel, *(double*)payload);

        setSetpoint(sel, *(double*)payload);

    }
    //=========================================================================
    else {
        puts("unknown variable");
    }

    return -1;
    /*
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
    }*/
}
