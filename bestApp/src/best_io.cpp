
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
    char kappa_sel[16] = {0};
    char pos_sel[16] = {0};
    unsigned short status;
    std::string pv_name_str = std::string(pvName);

    //=========================================================================
    if( sscanf(pvName, "best_BPM%dscale%c", &bpm_sel, pos_sel) == 2){

        double scaleX, scaleY;
        getBPMscaling_sel(bpm_sel, &scaleX, &scaleY);

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

        getPosArray_sel(bpm_sel, posX, posY, I0, count);

        if      (pos_sel_str == "X")   memcpy(payload, posX, count*sizeof(double));
        else if (pos_sel_str == "Y")   memcpy(payload, posY, count*sizeof(double));
        else if (pos_sel_str == "Int") memcpy(payload, I0, count*sizeof(double));
        else                           {free(posX); free(posY); free(I0); return -1;}

        PDEBUG(DEBUG_RET_DATA, "pv: %s, data[0]: %lf\n", pvName, *(double*)payload);

        free(posX); free(posY); free(I0);
        return 0;
    }
    //=========================================================================
    else if( sscanf(pvName, "best_TetrAMM%dch%d", &bpm_sel, &ch) == 2){

        double (*current)[4];
        current = (double(*)[4])malloc(4*count*sizeof(double));

        getCurrentArray_sel(bpm_sel, current, count);

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
    else if (pv_name_str == "best_NumberTetrAMM"){
		uint16_t id;
		uint32_t timestamp;
		uint32_t fw_ver;
		uint32_t ser_nr;
		int nr_tetramms = 0;

		for (int selector = 0; selector < 4; selector++) {
			getSFPinfo(selector, &id, &timestamp, &fw_ver, &ser_nr);
			nr_tetramms += (id & 0xFF00) == 0x2100;
		}

        PDEBUG(DEBUG_RET_DATA, "pv: %s, nr_tetramms: %d\n", pvName, nr_tetramms);

        *(unsigned short *) payload = nr_tetramms;
    }
    //=========================================================================
    // TODO: add to library
    else if (pv_name_str == "best_rbv_PreDACoutmux") {
		int fd = open(FILE_MBOX, O_RDWR | O_SYNC);
		if(fd < 0){
			printf("Error opening FILE_MBOX\n");
			return -1;
		}

		struct mail comm;
		memset(comm.payload, 0, 28);
		comm.cmd = CMD_SIG_DEMUX << 8 | CMD_READ;

		// emit io control
		int ret = ioctl(fd, IOCTL_MAIL_COMM, &comm);
		if (ret < 0){
			printf("ioctl(IOCTL_MAIL_COMM) failed\n");
			return -1;
		}

		*(short int*)payload = (short int)comm.payload[0];

		close(fd);
		return 0;
    }
    //=========================================================================
	else if( (pv_name_str == "best_rbv_PidOffsetX") ||
			 (pv_name_str == "best_rbv_PidOffsetY") ||
			 (pv_name_str == "best_rbv_PidOffsetI0") ){
		
        enum best_pid_select pid_sel;
		if      (pv_name_str == "best_rbv_PidOffsetX")  pid_sel = BEST_PID_SELECT_X;
		else if (pv_name_str == "best_rbv_PidOffsetY")  pid_sel = BEST_PID_SELECT_Y;
		else if (pv_name_str == "best_rbv_PidOffsetI0") pid_sel = BEST_PID_SELECT_I0;
		else                                        return -1;

        getOffsetSingle(pid_sel, (double*)payload);

        PDEBUG(DEBUG_RET_DATA, "pv: %s, data[0]: %lf\n", pvName, *(double*)payload);

        return 0;
    }
    //=========================================================================
    else if( sscanf(pvName, "best_rbv_PidK%1s%2s", kappa_sel, pos_sel) == 2){
    
    	std::string K_sel_str = std::string(kappa_sel);
        int K_sel;
        if      (K_sel_str == "p")   K_sel = 4;
        else if (K_sel_str == "i")   K_sel = 12;
        else if (K_sel_str == "d")   K_sel = 20;
        else                         return -1;
        
        std::string pos_sel_str = std::string(pos_sel);
        enum best_pid_select pid_sel;
        if      (pos_sel_str == "X")   pid_sel = BEST_PID_SELECT_X;
        else if (pos_sel_str == "Y")   pid_sel = BEST_PID_SELECT_Y;
        else if (pos_sel_str == "I0")   pid_sel = BEST_PID_SELECT_I0;
        else                            return -1;
                
        getPIDparamSingle(K_sel, pid_sel, (int)pid_par_sel_K, (double*)payload);

        //PDEBUG(DEBUG_RET_DATA, "pv: %s, data[0]: %lf\n", pvName, *(double*)payload);
        //if ((K_sel == 4) & (pid_sel == 1))
        //    printf("pv: %s, data[0]: %lf\n", pvName, *(double*)payload);

        return 0;
    }
    //=========================================================================
    else if( sscanf(pvName, "best_rbv_PidO%3s%2s", kappa_sel, pos_sel) == 2){
    
        
    	std::string K_sel_str = std::string(kappa_sel);
        int K_sel;
        if      (K_sel_str == "min")   K_sel = 4;
        else if (K_sel_str == "max")   K_sel = 12;
        else if (K_sel_str == "gai")   K_sel = 20;
        else                           return -1;
        
        std::string pos_sel_str = std::string(pos_sel);
        enum best_pid_select pid_sel;
        if      (pos_sel_str == "X")   pid_sel = BEST_PID_SELECT_X;
        else if (pos_sel_str == "Y")   pid_sel = BEST_PID_SELECT_Y;
        else if (pos_sel_str == "I0")   pid_sel = BEST_PID_SELECT_I0;
        else                            return -1;
          
        getPIDparamSingle(K_sel, pid_sel, (int)pid_par_sel_out, (double*)payload);

        PDEBUG(DEBUG_RET_DATA, "pv: %s, data[0]: %lf\n", pvName, *(double*)payload);

        return 0;
    }
    //=========================================================================
    else if( sscanf(pvName, "best_rbv_PidImax%2s", pos_sel) == 1){
    
        int K_sel = 12;  
        std::string pos_sel_str = std::string(pos_sel);
        enum best_pid_select pid_sel;
        if      (pos_sel_str == "X")   pid_sel = BEST_PID_SELECT_X;
        else if (pos_sel_str == "Y")   pid_sel = BEST_PID_SELECT_Y;
        else if (pos_sel_str == "I0")   pid_sel = BEST_PID_SELECT_I0;
        else                            return -1;
          
        getPIDparamSingle(K_sel, pid_sel, (int)pid_par_sel_lim, (double*)payload);

        PDEBUG(DEBUG_RET_DATA, "pv: %s, data[0]: %lf\n", pvName, *(double*)payload);

        return 0;
    }
    //=========================================================================
    else if( sscanf(pvName, "best_rbv_Pidemin%2s", pos_sel) == 1){
    
        int K_sel = 4;  
        std::string pos_sel_str = std::string(pos_sel);
        enum best_pid_select pid_sel;
        if      (pos_sel_str == "X")   pid_sel = BEST_PID_SELECT_X;
        else if (pos_sel_str == "Y")   pid_sel = BEST_PID_SELECT_Y;
        else if (pos_sel_str == "I0")   pid_sel = BEST_PID_SELECT_I0;
        else                            return -1;
          
        getPIDparamSingle(K_sel, pid_sel, (int)pid_par_sel_lim, (double*)payload);

        PDEBUG(DEBUG_RET_DATA, "pv: %s, data[0]: %lf\n", pvName, *(double*)payload);

        return 0;
    }
    //=========================================================================
    else if( sscanf(pvName, "best_rbv_PreDAC_Ch%d", &ch) == 1){

        double (*voltage)[4];
        voltage = (double(*)[4])malloc(4*count*sizeof(double));

        getVoltageArray(voltage, count);

        int i = 0;
        double *d_ptr = (double*)payload;
        for(i = 0; i < count; i++){
            *d_ptr++ = voltage[i][ch-1];
        }

        PDEBUG(DEBUG_RET_DATA, "pv: %s, data[0]: %lf\n", pvName, *(double*)payload);

        free(voltage);
        return 0;
    }
    //=========================================================================
    else {
        PDEBUG(DEBUG_ERROR, " %s: unknown name\n", pvName);
    }

    return -1; // unknown variable
}


int writeBest(char *pvName, retType_t type, void* payload){
    unsigned short value;
    int predac_ch;
    int bpm_sel;
    static double predac_out[4];
    char kappa_sel[16] = {0};
    char pos_sel[16] = {0};

    PDEBUG(DEBUG_LOW_FUNC, "func: %s(), pv: %s\n", __FUNCTION__, pvName);

    std::string pv_name_str = std::string(pvName);

    //=========================================================================
    if(sscanf(pvName, "best_TetrAMM%drange", &bpm_sel) == 1) {
        value = *(unsigned short*)payload;
	PDEBUG(DEBUG_SET_DATA,
               "pv: %s, tetrammSetRange(%d, %d)\n",
               pvName, value, bpm_sel);

        tetrammSetRange(value, bpm_sel);

        return 0;
    }
    //=========================================================================
    else if( (pv_name_str == "best_PidSetpointX") ||
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
    else if( sscanf(pvName, "best_PidK%1s%2s", kappa_sel, pos_sel) == 2){
    
    	std::string K_sel_str = std::string(kappa_sel);
        int K_sel;
        if      (K_sel_str == "p")   K_sel = 4;
        else if (K_sel_str == "i")   K_sel = 12;
        else if (K_sel_str == "d")   K_sel = 20;
        else                         return -1;
        //printf("%2s\n",pos_sel);
        std::string pos_sel_str = std::string(pos_sel);
        enum best_pid_select pid_sel;
        if      (pos_sel_str == "X")    pid_sel = BEST_PID_SELECT_X;
        else if (pos_sel_str == "Y")    pid_sel = BEST_PID_SELECT_Y;
        else if (pos_sel_str == "I0")   pid_sel = BEST_PID_SELECT_I0;
        else                            return -1;
                
        PDEBUG(DEBUG_SET_DATA, "pv: %s, setPIDparamSingle(%d, %d, %d, %lf)\n",
               pvName, K_sel, (int)pid_sel, (int)pid_par_sel_K, *(double*)payload);

        setPIDparamSingle(K_sel, pid_sel, (int)pid_par_sel_K, *(double*)payload);

        return 0;
    }
    //=========================================================================
    else if( sscanf(pvName, "best_PidO%3s%2s", kappa_sel, pos_sel) == 2){
    
    	std::string K_sel_str = std::string(kappa_sel);
        int K_sel;
        if      (K_sel_str == "min")   K_sel = 4;
        else if (K_sel_str == "max")   K_sel = 12;
        else if (K_sel_str == "gai")   K_sel = 20;
        else                           return -1;
        
        std::string pos_sel_str = std::string(pos_sel);
        enum best_pid_select pid_sel;
        if      (pos_sel_str == "X")    pid_sel = BEST_PID_SELECT_X;
        else if (pos_sel_str == "Y")    pid_sel = BEST_PID_SELECT_Y;
        else if (pos_sel_str == "I0")   pid_sel = BEST_PID_SELECT_I0;
        else                            return -1;
                
        PDEBUG(DEBUG_SET_DATA, "pv: %s, setPIDparamSingle(%d, %d, %d, %lf)\n",
               pvName, K_sel, (int)pid_sel, (int)pid_par_sel_out, *(double*)payload);

        setPIDparamSingle(K_sel, pid_sel, (int)pid_par_sel_out, *(double*)payload);

        return 0;
    }
    //=========================================================================
    else if( sscanf(pvName, "best_PidImax%2s", pos_sel) == 1){
    
        int K_sel = 12;
        std::string pos_sel_str = std::string(pos_sel);
        enum best_pid_select pid_sel;
        if      (pos_sel_str == "X")    pid_sel = BEST_PID_SELECT_X;
        else if (pos_sel_str == "Y")    pid_sel = BEST_PID_SELECT_Y;
        else if (pos_sel_str == "I0")   pid_sel = BEST_PID_SELECT_I0;
        else                            return -1;
                
        PDEBUG(DEBUG_SET_DATA, "pv: %s, setPIDparamSingle(%d, %d, %d, %lf)\n",
               pvName, K_sel, (int)pid_sel, (int)pid_par_sel_lim, *(double*)payload);

        setPIDparamSingle(K_sel, pid_sel, (int)pid_par_sel_lim, *(double*)payload);

        return 0;
    }
    //=========================================================================
    else if( sscanf(pvName, "best_Pidemin%2s", pos_sel) == 1){
    
        int K_sel = 4;
        std::string pos_sel_str = std::string(pos_sel);
        enum best_pid_select pid_sel;
        if      (pos_sel_str == "X")    pid_sel = BEST_PID_SELECT_X;
        else if (pos_sel_str == "Y")    pid_sel = BEST_PID_SELECT_Y;
        else if (pos_sel_str == "I0")   pid_sel = BEST_PID_SELECT_I0;
        else                            return -1;
                
        PDEBUG(DEBUG_SET_DATA, "pv: %s, setPIDparamSingle(%d, %d, %d, %lf)\n",
               pvName, K_sel, (int)pid_sel, (int)pid_par_sel_lim, *(double*)payload);

        setPIDparamSingle(K_sel, pid_sel, (int)pid_par_sel_lim, *(double*)payload);

        return 0;
    }
    //=========================================================================
    else if (pv_name_str == "best_PIDenable") {
        value = *(unsigned short*)payload;

        PDEBUG(DEBUG_SET_DATA, "pv: %s, setFBenable(%d)\n", pvName, value);

        return setFBenable(value);
    }
    //=========================================================================
    else if (pv_name_str == "best_PIDreset") {
        value = *(unsigned short*)payload;

        PDEBUG(DEBUG_SET_DATA, "pv: %s, setCtrlReset()\n", pvName);

        return setCtrlReset();
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
    // TODO: add to library
    else if (pv_name_str == "best_PreDACoutmux") {
		int fd = open(FILE_MBOX, O_RDWR | O_SYNC);
		if(fd < 0){
			printf("Error opening FILE_MBOX\n");
			return -1;
		}
		struct mail comm;
		comm.cmd = CMD_SIG_DEMUX << 8 | CMD_WRITE;
		memset(comm.payload, 0, 28);
		comm.payload[0] = *(short int*)payload;

		// emit io control
		int ret = ioctl(fd, IOCTL_MAIL_COMM, &comm);
		if (ret < 0){
			printf("ioctl(IOCTL_MAIL_COMM) failed\n");
			return -1;
		}

		close(fd);

		return 0;
    }
    //=========================================================================
    else if( sscanf(pvName, "best_PreDACoutCh%d", &predac_ch) == 1){
		int rc = 0;
		int fd = open(FILE_PREDAC, O_RDWR);

		if(fd < 0){
			printf("Error opening FILE_PREDAC\n");
			return -1;
		}

		predac_ch -= 1; // to get from the user numbers (1..4) to array idxs

		if ((predac_ch < 0) ||
				(predac_ch >= (int)(sizeof(predac_out)/sizeof(*predac_out)))){
			return -1;
		}

		predac_out[predac_ch] = *(double*)payload;

		struct dac_data data;
		data.control = 0x1; // enable PreDAC output
		data.ch0 = predac_out[0];
		data.ch1 = predac_out[1];
		data.ch2 = predac_out[2];
		data.ch3 = predac_out[3];

		rc = ioctl(fd, IOCTL_WRITE_DAC, &data);
		if (rc < 0){
			printf("ioctl(IOCTL_WRITE_DAC) failed\n");
			return -1;
		}

		close(fd);
		return 0;
    }
    //=========================================================================
    else {
        PDEBUG(DEBUG_ERROR, " %s: unknown name\n", pvName);
    }

    return -1;
}
