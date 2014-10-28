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

    int ch; unsigned short status;

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
    else if( (strcmp(pvName, "BPM0:PosX") == 0) ||
            (strcmp(pvName, "BPM0:PosY") == 0) ||
            (strcmp(pvName, "BPM0:Int")  == 0) ) {


        double *posX = (double*)malloc(count*sizeof(double));
        double *posY = (double*)malloc(count*sizeof(double));
        double *I0 = (double*)malloc(count*sizeof(double));

        getPosArray(posX, posY, I0, count);

        if(pvName[8] == 'X')      memcpy(payload, posX, count*sizeof(double));
        else if(pvName[8] == 'Y') memcpy(payload, posY, count*sizeof(double));
        else                      memcpy(payload, I0,   count*sizeof(double));

        PDEBUG(DEBUG_RET_DATA, "pv: %s, data[0]: %lf\n", pvName, *(double*)payload);

        free(posX); free(posY); free(I0);
        return 0;
    }
    //=========================================================================
    else if( sscanf(pvName, "TetrAMM0:Ch%d", &ch) == 1){

        double (*current)[4];
        current = (double(*)[4])malloc(4*count*sizeof(double));

        getCurrentArray(current, count);

        int i = 0;
        double *d_ptr = (double*)payload;
        for(i = 0; i < count; i++){
            *d_ptr++ = current[i][ch-1];
        }

        PDEBUG(DEBUG_RET_DATA, "pv: %s, data[0]: %lf\n", pvName, *(double*)payload);

        return 0;
    }
    //=========================================================================
    else if( strcmp(pvName, "PID:Status") == 0 ){

        status = (unsigned short)getPIDstatus();
        PDEBUG(DEBUG_RET_DATA, "pv: %s, status: %d\n", pvName, status);

        *(unsigned short *) payload = status;

        return 0;
    }
    //=========================================================================
    else if ( strcmp(pvName, "Login:Level") == 0){

        status = (unsigned short)getLockStatus();
        PDEBUG(DEBUG_RET_DATA, "pv: %s, status: %d\n", pvName, status);

        *(unsigned short *) payload = status;
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
    else if( strcmp(pvName, "PID:Enable") == 0 ) {
        unsigned short value;
        value = *(unsigned short*)payload;

        PDEBUG(DEBUG_SET_DATA, "pv: %s, setFBenable(%d)\n", pvName, value);

        int rc = setFBenable(value);
        printf("rc: %d\n", rc); //XXX TODO

        return 0;
    }
    //=========================================================================
    else if ( strcmp(pvName, "Login:UserPass") == 0) {
        printf("XXXXXXXXXXXXXXXXXXXXXX\n\n");

        char user[40]; char pass[40];
        if(sscanf((char*)payload, "%[^:]:%s", user, pass) != 2){
            PDEBUG(DEBUG_WARN, "User and password are not separated by ':'\n")
            return -1;
        }

        int level = getLock(user, pass);
        PDEBUG(DEBUG_ACC_SEC,
               "pv: %s, user: %s, pass: %s, level: %d\n",
               pvName, user, pass, level);

        printf("XXXXXXXXXXXXXXXXXXXXXX\n\n");
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
    else {
        puts("unknown variable");
    }*/
}
