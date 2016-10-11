
#include <string>

#include "best_io.h"

// default debug meassages on error and warn
uint32_t debug = DEBUG_WARN | DEBUG_ERROR;

int stripEpicsIocName(char *pvName, aoRecord *pao){
	if (pao->out.type != INST_IO){
		PDEBUG(DEBUG_ERROR, "Record %s: Output link must be of type INST_IO\n", pao->name);
		return -1;
	}

	strncpy(pvName, pao->out.value.instio.string, 32);

	return 0;
}

int stripEpicsIocName(char *pvName, waveformRecord *pwf){
	if (pwf->inp.type != INST_IO){
		PDEBUG(DEBUG_ERROR, "Record %s: Input link must be of type INST_IO\n", pwf->name);
		return -1;
	}

	strncpy(pvName, pwf->inp.value.instio.string, 32);

	return 0;
}

int stripEpicsIocName(char *pvName, aiRecord *pai){
	if (pai->inp.type != INST_IO){
		PDEBUG(DEBUG_ERROR, "Record %s: Input link must be of type INST_IO\n", pai->name);
	}

	strncpy(pvName, pai->inp.value.instio.string, 32);

	return 0;
}

int stripEpicsIocName(char *pvName, mbbiRecord *pmbbi){
	if (pmbbi->inp.type != INST_IO){
		PDEBUG(DEBUG_ERROR, "Record %s: Input link must be of type INST_IO\n", pmbbi->name);
	}

	strncpy(pvName, pmbbi->inp.value.instio.string, 32);

	return 0;
}

int stripEpicsIocName(char *pvName, boRecord *pbo){
	if (pbo->out.type != INST_IO){
		PDEBUG(DEBUG_ERROR, "Record %s: Output link must be of type INST_IO\n", pbo->name);
		return -1;
	}

	strncpy(pvName, pbo->out.value.instio.string, 32);

	return 0;
}

int stripEpicsIocName(char *pvName, mbboRecord *pmbbo){
	if (pmbbo->out.type != INST_IO){
		PDEBUG(DEBUG_ERROR, "Record %s: Output link must be of type INST_IO\n", pmbbo->name);
		return -1;
	}

	strncpy(pvName, pmbbo->out.value.instio.string, 32);

	return 0;
}

int stripEpicsIocName(char *pvName, stringoutRecord *psor){
	if (psor->out.type != INST_IO){
		PDEBUG(DEBUG_ERROR, "Record %s: Output link must be of type INST_IO\n", psor->name);
		return -1;
	}

	strncpy(pvName, psor->out.value.instio.string, 32);

	return 0;
}


int initBest(char *pvName, retType_t type, void *rec){
    double roiX, roiY, roiI0min, roiI0max, roc;
    PDEBUG(DEBUG_LOW_FUNC | DEBUG_REC_INIT, "func: %s(), pv: %s\n", __FUNCTION__, pvName);


    if( strcmp(pvName, "PID:SetpointX") == 0 ) {
        aoRecord *pao = (aoRecord*) rec;
		getROCandROI( &roiX, &roiY, &roiI0min, &roiI0max, &roc );

        pao->hopr = roiX;
        pao->lopr = roiX*-1.;

        return 0;
    }
    else if( strcmp(pvName, "PID:SetpointY") == 0) {
        aoRecord *pao = (aoRecord*) rec;
		getROCandROI( &roiX, &roiY, &roiI0min, &roiI0max, &roc );

        pao->hopr = roiY;
        pao->lopr = roiY*-1.;

        return 0;
    }
    else if( strcmp(pvName, "PID:SetpointI0") == 0) {
        aoRecord *pao = (aoRecord*) rec;
		getROCandROI( &roiX, &roiY, &roiI0min, &roiI0max, &roc );

        pao->hopr = roiI0max;
        pao->lopr = roiI0min;

        return 0;
    }
    else if( strcmp(pvName, "BPM0:PosX") == 0 ){
        waveformRecord *pwf = (waveformRecord*) rec;
        double scaleX, scaleY;
        getBPMscaling(&scaleX, &scaleY);

        pwf->hopr = scaleX;
        pwf->lopr = scaleX*-1.;

        return 0;
    }
    else if( strcmp(pvName, "BPM0:PosY") == 0 ){
        waveformRecord *pwf = (waveformRecord*) rec;
        double scaleX, scaleY;
        getBPMscaling(&scaleX, &scaleY);

        pwf->hopr = scaleY;
        pwf->lopr = scaleY*-1.;

        return 0;
    }

    return 0;
}

int readBest(char *pvName, retType_t type, void* payload, int count){

    PDEBUG(DEBUG_LOW_FUNC, "func: %s(), pv: %s\n", __FUNCTION__, pvName);

    int ch;
    int bpm_sel;
    char pos_sel[16] = {0};
    unsigned short status;
    std::string pv_name_str = std::string(pvName);

    //=========================================================================
    if( sscanf(pvName, "best_BPM%dscale%c", &bpm_sel, pos_sel) == 2){

        double scaleX, scaleY;
        getBPMscaling(&scaleX, &scaleY);

    	std::string pos_sel_str = std::string(pos_sel);

    	if      (pos_sel_str == "X") *(double*)payload = scaleX;
    	else if (pos_sel_str == "Y") *(double*)payload = scaleY;
    	else                         return -1;

        PDEBUG(DEBUG_RET_DATA, "pv: %s, data: %lf\n", pvName, *(double*)payload);

        return 0;
    }
    //=========================================================================
    else if( sscanf(pvName, "best_BPM%dwfm%3s", &bpm_sel, pos_sel) == 2){

    	std::string pos_sel_str = std::string(pos_sel);

        double *posX = (double*)malloc(count*sizeof(double));
        double *posY = (double*)malloc(count*sizeof(double));
        double *I0 = (double*)malloc(count*sizeof(double));

        getPosArray(posX, posY, I0, count);

        if      (pos_sel_str == "X")   memcpy(payload, posX, count*sizeof(double));
        else if (pos_sel_str == "Y")   memcpy(payload, posY, count*sizeof(double));
        else if (pos_sel_str == "Int") memcpy(payload, I0, count*sizeof(double));
        else                           {free(posX); free(posY); free(I0); return -1;}

        PDEBUG(DEBUG_RET_DATA, "pv: %s, data[0]: %lf\n", pvName, *(double*)payload);

        free(posX); free(posY); free(I0);
        return 0;
    }
    //=========================================================================
    else if( sscanf(pvName, "best_TetrAMMch%d", &ch) == 1){

        double (*current)[4];
        current = (double(*)[4])malloc(4*count*sizeof(double));

        getCurrentArray(current, count);

        int i = 0;
        double *d_ptr = (double*)payload;
        for(i = 0; i < count; i++){
            *d_ptr++ = current[i][ch-1];
        }

        PDEBUG(DEBUG_RET_DATA, "pv: %s, data[0]: %lf\n", pvName, *(double*)payload);

        free(current);
        return 0;
    }
    //=========================================================================
    else if (pv_name_str == "best_PIDstatus"){

        status = (unsigned short)getPIDstatus();
        PDEBUG(DEBUG_RET_DATA, "pv: %s, status: %d\n", pvName, status);

        *(unsigned short *) payload = status;

        return 0;
    }
    //=========================================================================
    else if( sscanf(pvName, "best_PIDro%8s", pos_sel) == 1){
        double roiX, roiY, roiI0min, roiI0max, roc;
        getROCandROI( &roiX, &roiY, &roiI0min, &roiI0max, &roc );

    	std::string sel_str = std::string(pos_sel);

    	if      (sel_str == "c")       *(double *) payload = roc;
    	else if (sel_str == "iX")      *(double *) payload = roiX;
    	else if (sel_str == "iY")      *(double *) payload = roiY;
    	else if (sel_str == "iIntMax") *(double *) payload = roiI0max;
    	else if (sel_str == "iIntMin") *(double *) payload = roiI0min;
    	else                           return -1;

        PDEBUG(DEBUG_RET_DATA, "pv: %s, value: %lf\n", pvName, *(double*)payload);

        return 0;
    }
    //=========================================================================
    else if (pv_name_str == "best_LoginLevel"){

        status = (unsigned short)getLockStatus();
        PDEBUG(DEBUG_RET_DATA, "pv: %s, status: %d\n", pvName, status);

        *(unsigned short *) payload = status;
    }
    //=========================================================================
    else {
        PDEBUG(DEBUG_ERROR, " %s: unknown name\n", pvName);
    }

    return -1; // unknown variable
}


int writeBest(char *pvName, retType_t type, void* payload){
    unsigned short value;

    PDEBUG(DEBUG_LOW_FUNC, "func: %s(), pv: %s\n", __FUNCTION__, pvName);

    std::string pv_name_str = std::string(pvName);


    //=========================================================================
    if( (pv_name_str == "best_PidSetpointX") ||
			(pv_name_str == "best_PidSetpointY") ||
			(pv_name_str == "best_PidSetpointI0") ){
	    int sel;
		if      (pv_name_str == "best_PidSetpointX")  sel = 0;
		else if (pv_name_str == "best_PidSetpointY")  sel = 1;
		else if (pv_name_str == "best_PidSetpointI0") sel = 2;
		else                                          return -1;

        PDEBUG(DEBUG_SET_DATA,
               "pv: %s, setSetpoint(%d, %lf)\n",
               pvName, sel, *(double*)payload);

        setSetpoint(sel, *(double*)payload);

        return 0;
    }
    //=========================================================================
	else if( (pv_name_str == "best_PidOffsetX") ||
			(pv_name_str == "best_PidOffsetY") ||
			(pv_name_str == "best_PidOffsetI0") ){
		enum best_pid_select pid_sel;

		if      (pv_name_str == "best_PidOffsetX")  pid_sel = BEST_PID_SELECT_X;
		else if (pv_name_str == "best_PidOffsetY")  pid_sel = BEST_PID_SELECT_Y;
		else if (pv_name_str == "best_PidOffsetI0") pid_sel = BEST_PID_SELECT_I0;
		else                                        return -1;

        PDEBUG(DEBUG_SET_DATA,
               "pv: %s, setOffsetSingle(%d, %lf)\n",
               pvName, (int)pid_sel, *(double*)payload);

        setOffsetSingle(pid_sel, *(double*)payload);

        return 0;
    }
    //=========================================================================
    else if (pv_name_str == "best_PIDenable") {
        value = *(unsigned short*)payload;

        PDEBUG(DEBUG_SET_DATA, "pv: %s, setFBenable(%d)\n", pvName, value);

        return setFBenable(value);
    }
    //=========================================================================
    else if (pv_name_str == "best_PIDconfig") {
        value = *(unsigned short*)payload;
        return setPIDconf(value);
    }
    //=========================================================================
    else if (pv_name_str == "best_LoginUserPass") {
        char user[40]; char pass[40];
        if(sscanf((char*)payload, "%[^:]:%s", user, pass) != 2){
            PDEBUG(DEBUG_WARN, "User and password are not separated by ':'\n")
            return -1;
        }

        int level = getLock(user, pass);
        PDEBUG(DEBUG_ACC_SEC,
               "pv: %s, user: %s, pass: %s, level: %d\n",
               pvName, user, pass, level);
    }
    //=========================================================================
    /* TODO: add to library
        else if (pv_name_str == "best_PreDACoutmux"){
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
    } */
    //=========================================================================
    else {
        PDEBUG(DEBUG_ERROR, " %s: unknown name\n", pvName);
    }

    return -1;
}
