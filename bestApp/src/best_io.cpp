
#include <string>

#include "best_io.h"

// default debug meassages on error and warn
uint32_t debug = DEBUG_WARN | DEBUG_ERROR;
uint32_t PID_MAX_FREQ = 100000.0;

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

int stripEpicsIocName(char *pvName, biRecord *pbi){
	if (pbi->inp.type != INST_IO){
		PDEBUG(DEBUG_ERROR, "Record %s: Input link must be of type INST_IO\n", pbi->name);
	}

	strncpy(pvName, pbi->inp.value.instio.string, 32);

	return 0;
}

int stripEpicsIocName(char *pvName, stringinRecord *psir){
	if (psir->inp.type != INST_IO){
		PDEBUG(DEBUG_ERROR, "Record %s: Input link must be of type INST_IO\n", psir->name);
	}

	strncpy(pvName, psir->inp.value.instio.string, 32);

	return 0;
}

// bool init_config_success = 0;

int initBest(char *pvName, retType_t type, void *rec){
	// double roiX, roiY, roiI0min, roiI0max, roc;
	PDEBUG(DEBUG_LOW_FUNC | DEBUG_REC_INIT, "func: %s(), pv: %s\n", __FUNCTION__, pvName);

	/*
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
	*/
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
	if ( pv_name_str == "best_rbv_SelectBpm" ) {
		int posX, posY, I0;
		getBPMselector(&posX, &posY, &I0);
		short int value = (posX << 2) | (posY << 1) | (I0);
		*(unsigned short *)payload = value;
		PDEBUG(DEBUG_RET_DATA, "pv: %s, data: %lf\n", pvName, *(double *)payload);
		return 0;
	}
	//=========================================================================
	else if( ( sscanf(pvName, "best_rbv_BPM%1dscale%1s", &bpm_sel, pos_sel) == 2 ) && 
		( strlen(pvName) == 19 ) ) {

		double scaleX, scaleY;
		getBPMscale(bpm_sel, &scaleX, &scaleY);

		std::string pos_sel_str = std::string(pos_sel);

		if	  (pos_sel_str == "X") *(double*)payload = scaleX;
		else if (pos_sel_str == "Y") *(double*)payload = scaleY;
		else						 return -1;

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

		if	  (pos_sel_str == "X")   memcpy(payload, posX, count*sizeof(double));
		else if (pos_sel_str == "Y")   memcpy(payload, posY, count*sizeof(double));
		else if (pos_sel_str == "Int") memcpy(payload, I0, count*sizeof(double));
		else						   {free(posX); free(posY); free(I0); return -1;}

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
	else if ( pv_name_str == "best_rbv_PidConfigOUT" ) {
		uint8_t posX, posY, I0;
		getPIDoutSel(&posX, &posY, &I0);
		*(double*)payload = (posX+1)*100 + (posY+1)*10 + (I0+1);
		PDEBUG(DEBUG_RET_DATA, "pv: %s, data: %lf\n", pvName, *(double*)payload);
		return 0;
	}

	//=========================================================================
	else if ( pv_name_str == "best_rbv_PidConfigIN" ) {
		char conf;
		getPIDconf(&conf);
		*(unsigned short *)payload = conf;
		PDEBUG(DEBUG_RET_DATA, "pv: %s, data: %lf\n", pvName, *(double*)payload);
		return 0;
	}

	//=========================================================================
	// else if( sscanf(pvName, "best_PIDro%8s", pos_sel) == 1){
	// 	double roiX, roiY, roiI0min, roiI0max, roc;
	// 	getROCandROI( &roiX, &roiY, &roiI0min, &roiI0max, &roc );
	// 
	// 	std::string sel_str = std::string(pos_sel);
	// 
	// 	if	  (sel_str == "c")	   *(double *) payload = roc;
	// 	else if (sel_str == "iX")	  *(double *) payload = roiX;
	// 	else if (sel_str == "iY")	  *(double *) payload = roiY;
	// 	else if (sel_str == "iIntMax") *(double *) payload = roiI0max;
	// 	else if (sel_str == "iIntMin") *(double *) payload = roiI0min;
	// 	else						   return -1;
	// 
	// 	PDEBUG(DEBUG_RET_DATA, "pv: %s, value: %lf\n", pvName, *(double*)payload);
	// 
	// 	return 0;
	// }
	//=========================================================================
	else if (pv_name_str == "best_LoginLevel"){

		status = (unsigned short)getLockStatus();
		PDEBUG(DEBUG_RET_DATA, "pv: %s, status: %d\n", pvName, status);

		*(unsigned short *) payload = status;
	}
	//=========================================================================
	else if (pv_name_str == "best_NumberTetrAMM"){

		status = getTetrammsNumber();

		PDEBUG(DEBUG_RET_DATA, "pv: %s, nr_tetramms: %d\n", pvName, status);

		*(unsigned short *) payload = status;
	}
	//=========================================================================
	// TODO: add to library
	else if (pv_name_str == "best_rbv_PreDACoutmux") {
		/*
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
		*/
		uint8_t sel;
		getOutputMux(&sel);
		*(short int*)payload = sel;
		return 0;
	}
	//=========================================================================
	else if( (pv_name_str == "best_rbv_PidOffsetX") ||
			 (pv_name_str == "best_rbv_PidOffsetY") ||
			 (pv_name_str == "best_rbv_PidOffsetI0") ){
		
		enum best_pid_select pid_sel;
		if	  (pv_name_str == "best_rbv_PidOffsetX")  pid_sel = BEST_PID_SELECT_X;
		else if (pv_name_str == "best_rbv_PidOffsetY")  pid_sel = BEST_PID_SELECT_Y;
		else if (pv_name_str == "best_rbv_PidOffsetI0") pid_sel = BEST_PID_SELECT_I0;
		else										return -1;

		getOffsetSingle(pid_sel, (double*)payload);

		PDEBUG(DEBUG_RET_DATA, "pv: %s, data[0]: %lf\n", pvName, *(double*)payload);

		return 0;
	}
	//=========================================================================
	else if( sscanf(pvName, "best_rbv_PidK%1s%2s", kappa_sel, pos_sel) == 2){
	
		std::string par_sel_str = std::string(kappa_sel);
		int pid_par;
		if	  (par_sel_str == "p")   pid_par = 0;
		else if (par_sel_str == "i")   pid_par = 1;
		else if (par_sel_str == "d")   pid_par = 2;
		else						 return -1;
		
		std::string pos_sel_str = std::string(pos_sel);
		enum best_pid_select pid_sel;
		if	  (pos_sel_str == "X")   pid_sel = BEST_PID_SELECT_X;
		else if (pos_sel_str == "Y")   pid_sel = BEST_PID_SELECT_Y;
		else if (pos_sel_str == "I0")   pid_sel = BEST_PID_SELECT_I0;
		else							return -1;
				
		getPIDparamSingle(pid_sel, pid_par, (double*)payload);

		PDEBUG(DEBUG_RET_DATA, "pv: %s, data[0]: %lf\n", pvName, *(double*)payload);

		return 0;
	}
	//=========================================================================
	else if( sscanf(pvName, "best_rbv_PidO%3s%2s", kappa_sel, pos_sel) == 2){
	
		
		std::string par_sel_str = std::string(kappa_sel);
		int pid_par;
		if	  (par_sel_str == "min")   pid_par = 5;
		else if (par_sel_str == "max")   pid_par = 6;
		else if (par_sel_str == "gai")   pid_par = 7;
		else						   return -1;
		
		std::string pos_sel_str = std::string(pos_sel);
		enum best_pid_select pid_sel;
		if	  (pos_sel_str == "X")   pid_sel = BEST_PID_SELECT_X;
		else if (pos_sel_str == "Y")   pid_sel = BEST_PID_SELECT_Y;
		else if (pos_sel_str == "I0")   pid_sel = BEST_PID_SELECT_I0;
		else							return -1;
		  
		getPIDparamSingle(pid_sel, pid_par, (double*)payload);

		PDEBUG(DEBUG_RET_DATA, "pv: %s, data[0]: %lf\n", pvName, *(double*)payload);

		return 0;
	}

	//=========================================================================
	else if ( ( sscanf(pvName, "best_rbv_Bpm%1dEnaroc%1s", &bpm_sel, pos_sel) == 2 ) &&
			  ( strlen(pvName) == 20 ) ) {
		bool rocX, rocY, rocI0, rocX2, rocY2, rocI02;
		getROCenable(&rocX, &rocY, &rocI0, &rocX2, &rocY2, &rocI02);
		std::string pos_sel_str = std::string(pos_sel);
		if	  ( (bpm_sel == 0) && (pos_sel_str == "X")  )  *(unsigned short *)payload = rocX;
		else if ( (bpm_sel == 0) && (pos_sel_str == "Y")  )  *(unsigned short *)payload = rocY;
		else if ( (bpm_sel == 1) && (pos_sel_str == "X")  )  *(unsigned short *)payload = rocX2;
		else if ( (bpm_sel == 1) && (pos_sel_str == "Y")  )  *(unsigned short *)payload = rocY2;
		else						   return -1;
		PDEBUG(DEBUG_RET_DATA, "pv: %s, data: %lf\n", pvName, *(double*)payload);
		return 0;
	}
	else if ( ( sscanf(pvName, "best_rbv_Bpm%1dEnaroc%2s", &bpm_sel, pos_sel) == 2 ) &&
			  ( strlen(pvName) == 21 ) ) {
		bool rocX, rocY, rocI0, rocX2, rocY2, rocI02;
		getROCenable(&rocX, &rocY, &rocI0, &rocX2, &rocY2, &rocI02);
		std::string pos_sel_str = std::string(pos_sel);
		if	  ( (bpm_sel == 0) && (pos_sel_str == "I0") )  *(unsigned short *)payload = rocI0;
		else if ( (bpm_sel == 1) && (pos_sel_str == "I0") )  *(unsigned short *)payload = rocI02;
		else						   return -1;
		PDEBUG(DEBUG_RET_DATA, "pv: %s, data: %lf\n", pvName, *(double*)payload);
		return 0;
	}

	//=========================================================================
	else if ( sscanf(pvName, "best_rbv_Bpm%1droc%2s", &bpm_sel, pos_sel) == 2 ) {
		
		char roc_selX, roc_selY, roc_selI;
		double min, max;
		std::string pos_sel_str = std::string(pos_sel);

		if (bpm_sel == 0) 
		{ 
			roc_selX = 0x0; 
			roc_selY = 0x1; 
			roc_selI = 0x2; 
		}
		else if (bpm_sel == 1) 
		{ 
			roc_selX = 0x3; 
			roc_selY = 0x4; 
			roc_selI = 0x5; 
		}
		else 
		{
			return -1;
		}

		if (pos_sel_str == "X")  // RocX
			getROClimits(roc_selX, &min, &max);	
		else if (pos_sel_str == "Y") // RocY
			getROClimits(roc_selY, &min, &max);
		else if (pos_sel_str == "I0") // RocI0 = BeamOffTh
			getROClimits(roc_selI, &min, &max);
		else 
			return -1;

		*(double *)payload = max;
		
		PDEBUG(DEBUG_RET_DATA, "pv: %s, data[0]: %lf\n", pvName, *(double*)payload);
		return 0;
	}
	//=========================================================================
	else if ( ( sscanf(pvName, "best_rbv_Bpm%1doffset%1s", &bpm_sel, pos_sel) == 2 ) &&
			  ( strlen(pvName) == 20 ) ) {
		double offsetX = 0.0;
		double offsetY = 0.0;
		getBPMoffset(&offsetX, &offsetY, bpm_sel);
		std::string pos_sel_str = std::string(pos_sel);
		if	  (pos_sel_str == "X") *(double*)payload = offsetX;
		else if (pos_sel_str == "Y") *(double*)payload = offsetY;
		else						 return -1;
		PDEBUG(DEBUG_RET_DATA, "pv: %s, data: %lf\n", pvName, *(double*)payload);
		return 0;
	}
	//=========================================================================
	else if ( ( sscanf(pvName, "best_rbv_Pid%1s_update_frequency", pos_sel) == 1) &&
			  ( strlen(pvName) == 30 ) ) {
	 
		std::string pos_sel_str = std::string(pos_sel);
		enum best_pid_select pid_sel;
		uint32_t nr_samp = 0;
		double freq;
		if	  (pos_sel_str == "X")   pid_sel = BEST_PID_SELECT_X;
		else if (pos_sel_str == "Y")   pid_sel = BEST_PID_SELECT_Y;
		else if (pos_sel_str == "I")   pid_sel = BEST_PID_SELECT_I0;
		else							return -1;
		
		// get nr_samp from FPGA
		getWindAvg(pid_sel, &nr_samp);
		// convert nr_samp to Frequency
		freq = PID_MAX_FREQ / nr_samp;
		if ((freq > PID_MAX_FREQ) || (freq < 1))
			return -1;
		else
			*(double*)payload = freq;	 

		PDEBUG(DEBUG_RET_DATA, "pv: %s, data[0]: %lf\n", pvName, *(double*)payload);

		return 0;
	}
	//=========================================================================
	else if( sscanf(pvName, "best_rbv_PidImax%2s", pos_sel) == 1){
	
		int pid_par = 4;  
		std::string pos_sel_str = std::string(pos_sel);
		enum best_pid_select pid_sel;
		if	  (pos_sel_str == "X")   pid_sel = BEST_PID_SELECT_X;
		else if (pos_sel_str == "Y")   pid_sel = BEST_PID_SELECT_Y;
		else if (pos_sel_str == "I0")   pid_sel = BEST_PID_SELECT_I0;
		else							return -1;
		  
		getPIDparamSingle(pid_sel, pid_par, (double*)payload);

		PDEBUG(DEBUG_RET_DATA, "pv: %s, data[0]: %lf\n", pvName, *(double*)payload);

		return 0;
	}
	//=========================================================================
	else if( sscanf(pvName, "best_rbv_Pidemin%2s", pos_sel) == 1){
	
		int pid_par = 3;  
		std::string pos_sel_str = std::string(pos_sel);
		enum best_pid_select pid_sel;
		if	  (pos_sel_str == "X")   pid_sel = BEST_PID_SELECT_X;
		else if (pos_sel_str == "Y")   pid_sel = BEST_PID_SELECT_Y;
		else if (pos_sel_str == "I0")   pid_sel = BEST_PID_SELECT_I0;
		else							return -1;
		  
		getPIDparamSingle(pid_sel, pid_par, (double*)payload);

		PDEBUG(DEBUG_RET_DATA, "pv: %s, data[0]: %lf\n", pvName, *(double*)payload);

		return 0;
	}

	//=========================================================================
	else if ( ( sscanf(pvName, "best_rbv_Bpm%dorient", &bpm_sel) == 1 ) &&
			  ( strlen(pvName) == 19 ) ) {
		char sel;
		if	   (bpm_sel == 0) sel = 0x0;
		else if  (bpm_sel == 1) sel = 0x1;
		else				   return -1;
		uint8_t is90deg;
		getBPMorient(sel, &is90deg);
		*(uint8_t *)payload = is90deg;
		PDEBUG(DEBUG_RET_DATA, "pv: %s, data: %lf\n", pvName, *(double*)payload);
		return 0;
	}

	//=========================================================================
	else if( sscanf(pvName, "best_rbv_PreDAC_Ch%d", &ch) == 1)
	{
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
	else if ( ( sscanf(pvName, "best_rbv_Bpm%1dcrossbar", &bpm_sel) == 1 ) &&
			  ( strlen(pvName) == 21 ) ) {
		uint8_t sel, ch0, ch1, ch2, ch3;
		if	  (bpm_sel == 0) sel = 0;
		else if (bpm_sel == 1) sel = 1;
		else				  return -1;
		getCrossBar(sel, &ch0, &ch1, &ch2, &ch3);
		*(double*)payload = (ch0+1)*1000 + (ch1+1)*100 + (ch2+1)*10 + (ch3+1);
		PDEBUG(DEBUG_RET_DATA, "pv: %s, data: %lf\n", pvName, *(double*)payload);
		return 0;
	}

	//=========================================================================
	else if ( sscanf(pvName, "best_rbv_PidSetpoint%s", pos_sel) == 1 ) {
		std::string pos_sel_str = std::string(pos_sel);
		int pid_sel;
		if	  (pos_sel_str == "X")	pid_sel = 0;
		else if (pos_sel_str == "Y")	pid_sel = 1;
		else if (pos_sel_str == "I0")   pid_sel = 2;
		else							return -1;

		getSetpoint2(pid_sel, (double*)payload);
		PDEBUG(DEBUG_RET_DATA, "pv: %s, data[0]: %lf\n", pvName, *(double*)payload);
		return 0;
	}

	//=========================================================================
	else if ( (pv_name_str == "best_rbv_SfpA") || (pv_name_str == "best_rbv_SfpB")) {
		int pos_A, pos_B;
		bool value = 0;
		getTetramms(&pos_A, &pos_B);
		if      ((pv_name_str == "best_rbv_SfpA") & pos_A) value = 1;
		else if ((pv_name_str == "best_rbv_SfpB") & pos_B) value = 1;
               *(unsigned short *)payload = value;
		PDEBUG(DEBUG_RET_DATA, "pv: %s, data[0]: %lf\n", pvName, *(double*)payload);
		return 0;
	}

	//=========================================================================
	else 
	{
		PDEBUG(DEBUG_ERROR, " %s: unknown name (readBest)\n", pvName);
	}

	return -1; // unknown variable
}

int writeBest(char *pvName, retType_t type, void* payload) {

	PDEBUG(DEBUG_LOW_FUNC, "func: %s(), pv: %s\n", __FUNCTION__, pvName);

	unsigned short value;
	int predac_ch;
	int bpm_sel;
	static double predac_out[4];
	char kappa_sel[16] = {0};
	char pos_sel[16] = {0};
	mINI::INIFile file(CONFIG_PATH);
	mINI::INIStructure ini;
	std::string section;
	std::string key;
	file.read(ini);
	std::string pv_name_str = std::string(pvName);
	int errLoginLevel = -1;

	//=========================================================================
	if ( ( sscanf(pvName, "best_BPM%1dscale%1s", &bpm_sel, pos_sel) == 2) && 
			  ( strlen(pvName) == 15 ) ) {
		double scaleX, scaleY;
		getBPMscale(bpm_sel, &scaleX, &scaleY);
		std::string pos_sel_str = std::string(pos_sel);
		if	  (pos_sel_str == "X") { scaleX = *(double*)payload; key = "Scale_X"; }
		else if (pos_sel_str == "Y") { scaleY = *(double*)payload; key = "Scale_Y"; }
		else						 return -1;
		errLoginLevel = setBPMscale(scaleX, scaleY, bpm_sel);
		if	  (bpm_sel == 0) section = "BPM1";
		else if (bpm_sel == 1) section = "BPM2";
		else				  return -1;
		std::ostringstream ss;
		ss << *(double *)payload;
		std::string payload_formatted_str = ss.str();
		if (!errLoginLevel) { ini[section][key] = payload_formatted_str; file.write(ini); }
		PDEBUG(DEBUG_RET_DATA, "pv: %s, data: %lf\n", pvName, *(double*)payload);
		PDEBUG(DEBUG_SET_DATA, "pv: %s, best_Bpm%1dscale%1s %lf)\n", pvName, bpm_sel, pos_sel, *(double*)payload);
		return 0;
	}
	
	//=========================================================================
	else if( (pv_name_str == "best_PidSetpointX") ||
			(pv_name_str == "best_PidSetpointY") ||
			(pv_name_str == "best_PidSetpointI0") ){
		int sel;
		if	  (pv_name_str == "best_PidSetpointX")  sel = 0;
		else if (pv_name_str == "best_PidSetpointY")  sel = 1;
		else if (pv_name_str == "best_PidSetpointI0") sel = 2;
		else										  return -1;

		PDEBUG(DEBUG_SET_DATA,
			   "pv: %s, setSetpoint(%d, %lf)\n",
			   pvName, sel, *(double*)payload);

		setSetpoint2(sel, *(double*)payload);

		return 0;
	}
	//=========================================================================
	else if( (pv_name_str == "best_PidOffsetX") ||
			(pv_name_str == "best_PidOffsetY") ||
			(pv_name_str == "best_PidOffsetI0") ){
		key = "Offset";
		std::ostringstream ss;
		ss << *(double *)payload;
		std::string payload_formatted_str = ss.str();
		enum best_pid_select pid_sel;

		if	  (pv_name_str == "best_PidOffsetX")  { pid_sel = BEST_PID_SELECT_X; section = "PID_X"; }
		else if (pv_name_str == "best_PidOffsetY")  { pid_sel = BEST_PID_SELECT_Y; section = "PID_Y"; }
		else if (pv_name_str == "best_PidOffsetI0") { pid_sel = BEST_PID_SELECT_I0; section = "PID_I0"; }
		else										return -1;

		PDEBUG(DEBUG_SET_DATA, "pv: %s, setOffsetSingle(%d, %lf)\n", pvName, (int)pid_sel, *(double*)payload);

		errLoginLevel = setOffsetSingle(pid_sel, *(double*)payload);
		if (!errLoginLevel) { ini[section][key] = payload_formatted_str; file.write(ini); }
		PDEBUG(DEBUG_RET_DATA, "pv: %s, data: %lf\n", pvName, *(double*)payload);

		return 0;
	}
	//=========================================================================
	else if( sscanf(pvName, "best_PidK%1s%2s", kappa_sel, pos_sel) == 2){
	
		std::string par_sel_str = std::string(kappa_sel);
		int pid_par;
		if	  (par_sel_str == "p") { pid_par = 0;  key = "Kp"; }
		else if (par_sel_str == "i") { pid_par = 1; key = "Ki"; }
		else if (par_sel_str == "d") { pid_par = 2; key = "Kd"; }
		else					   return -1;
		std::string pos_sel_str = std::string(pos_sel);
		enum best_pid_select pid_sel;
		std::ostringstream ss;
		ss << *(double *)payload;
		std::string payload_formatted_str = ss.str();
		if      (pos_sel_str == "X")  { pid_sel = BEST_PID_SELECT_X; section = "PID_X";}
		else if (pos_sel_str == "Y")  { pid_sel = BEST_PID_SELECT_Y; section = "PID_Y";}
		else if (pos_sel_str == "I0") { pid_sel = BEST_PID_SELECT_I0; section = "PID_I0";}
		else                          return -1;
				
		PDEBUG(DEBUG_SET_DATA, "pv: %s, setPIDparamSingle(%d, %d, %lf)\n",
			   pvName, (int)pid_sel, pid_par, *(double*)payload);

		errLoginLevel = setPIDparamSingle(pid_sel, pid_par, *(double*)payload);
		if (!errLoginLevel) { ini[section][key] = payload_formatted_str; file.write(ini); }
		PDEBUG(DEBUG_RET_DATA, "pv: %s, data: %lf\n", pvName, *(double*)payload);

		return 0;
	}
	//=========================================================================
	else if( sscanf(pvName, "best_PidO%3s%2s", kappa_sel, pos_sel) == 2){
	
		std::string par_sel_str = std::string(kappa_sel);
		int pid_par;
		if	  (par_sel_str == "min") {  pid_par = 5; key = "Omin"; }
		else if (par_sel_str == "max") {  pid_par = 6; key = "Omax"; }
		else if (par_sel_str == "gai") {  pid_par = 7; key = "Ogain"; }
		else						   return -1;
		std::ostringstream ss;
		ss << *(double *)payload;
		std::string payload_formatted_str = ss.str();
		std::string pos_sel_str = std::string(pos_sel);
		enum best_pid_select pid_sel;
		if      (pos_sel_str == "X")  { pid_sel = BEST_PID_SELECT_X; section = "PID_X";}
		else if (pos_sel_str == "Y")  { pid_sel = BEST_PID_SELECT_Y; section = "PID_Y";}
		else if (pos_sel_str == "I0") { pid_sel = BEST_PID_SELECT_I0; section = "PID_I0"; }
		else                          return -1;
				
		PDEBUG(DEBUG_SET_DATA, "pv: %s, setPIDparamSingle(%d, %d, %lf)\n",
			   pvName, (int)pid_sel, pid_par, *(double*)payload);

		errLoginLevel = setPIDparamSingle(pid_sel, pid_par, *(double*)payload);
		if (!errLoginLevel) { ini[section][key] = payload_formatted_str; file.write(ini); }
		PDEBUG(DEBUG_RET_DATA, "pv: %s, data: %lf\n", pvName, *(double*)payload);
		return 0;
	}

	//=========================================================================
	else if ( ( sscanf(pvName, "best_Bpm%1dEnaroc%1s", &bpm_sel, pos_sel) == 2 ) &&
			  ( strlen(pvName) == 16 ) ) {
		bool rocX, rocY, rocI0, rocX2, rocY2, rocI02;
		getROCenable(&rocX, &rocY, &rocI0, &rocX2, &rocY2, &rocI02);
		std::string pos_sel_str = std::string(pos_sel);
		std::string roc;
		if	    ((bpm_sel == 0) && (pos_sel_str == "X"))  { rocX  = *(double*)payload; section = "ROCandROI"; key = "RocXenable"; roc = rocX  ? "true" : "false"; }
		else if ((bpm_sel == 0) && (pos_sel_str == "Y"))  { rocY  = *(double*)payload; section = "ROCandROI"; key = "RocYenable"; roc = rocY  ? "true" : "false"; }
		else if ((bpm_sel == 1) && (pos_sel_str == "X"))  { rocX2 = *(double*)payload; section = "ROCandROI2";key = "RocXenable"; roc = rocX2 ? "true" : "false"; }
		else if ((bpm_sel == 1) && (pos_sel_str == "Y"))  { rocY2 = *(double*)payload; section = "ROCandROI2";key = "RocYenable"; roc = rocY2 ? "true" : "false"; }
		else						  return -1;
		errLoginLevel = setROCenable(rocX, rocY, rocI0, rocX2, rocY2, rocI02);
		if (!errLoginLevel) { ini[section][key] = roc; file.write(ini); }
		PDEBUG(DEBUG_SET_DATA, "pv: %s, best_Bpm%1dEnaroc%2s %lf)\n", pvName, bpm_sel, pos_sel, *(double*)payload);
		return 0;
	}
	else if ( ( sscanf(pvName, "best_Bpm%1dEnaroc%2s", &bpm_sel, pos_sel) == 2 ) &&
			  ( strlen(pvName) == 17 ) ) {
		bool rocX, rocY, rocI0, rocX2, rocY2, rocI02;
		getROCenable(&rocX, &rocY, &rocI0, &rocX2, &rocY2, &rocI02);
		std::string pos_sel_str = std::string(pos_sel);
		std::string roc;
		if	    ((bpm_sel == 0) && (pos_sel_str == "I0")) { rocI0  = *(double*)payload; section = "ROCandROI" ; key = "BeamOffEnable"; roc = rocI0  ? "true" : "false";}
		else if ((bpm_sel == 1) && (pos_sel_str == "I0")) { rocI02 = *(double*)payload; section = "ROCandROI2"; key = "BeamOffEnable"; roc = rocI02 ? "true" : "false";}
		else						  return -1;
		errLoginLevel = setROCenable(rocX, rocY, rocI0, rocX2, rocY2, rocI02);
		if (!errLoginLevel) { ini[section][key] = roc; file.write(ini); }
		PDEBUG(DEBUG_RET_DATA, "pv: %s, data: %lf\n", pvName, *(double*)payload);
		PDEBUG(DEBUG_SET_DATA, "pv: %s, best_Bpm%1dEnaroc%2s %lf)\n", pvName, bpm_sel, pos_sel, *(double*)payload);
		return 0;
	}

	//=========================================================================
	else if ( sscanf(pvName, "best_Bpm%1droc%2s", &bpm_sel, pos_sel) == 2 )
	{
		//double scaleX, scaleY;
		char roc_selX, roc_selY, roc_selI;
		//getBPMscale(bpm_sel, &scaleX, &scaleY);

		if (bpm_sel == 0) 
		{ 
			roc_selX = 0x0; 
			roc_selY = 0x1; 
			roc_selI = 0x2;
			section = "ROCandROI";
		}
		else if (bpm_sel == 1) 
		{ 
			roc_selX = 0x3; 
			roc_selY = 0x4; 
			roc_selI = 0x5; 
			section = "ROCandROI2";
		}
		else 
		{
			return -1;
		}
		std::ostringstream ss;
		ss << *(double *)payload;
		std::string payload_formatted_str = ss.str();
		std::string pos_sel_str = std::string(pos_sel);
		if (pos_sel_str == "X") { // RocX
			errLoginLevel = setROClimits(roc_selX, 0, *(double *)payload); // / scaleX
			key = "RocX";
		}
		else if (pos_sel_str == "Y") { // RocY
			errLoginLevel = setROClimits(roc_selY, 0, *(double *)payload); // / scaleY
			key = "RocY";
		}
		else if (pos_sel_str == "I0") { // RocI0 = BeamOffTh
			errLoginLevel = setROClimits(roc_selI, 0, *(double *)payload);
			key = "BeamOffTh";
		}
		else 
			return -1;
		if (!errLoginLevel) { ini[section][key] = payload_formatted_str; file.write(ini); }
		PDEBUG(DEBUG_RET_DATA, "pv: %s, data: %lf\n", pvName, *(double*)payload);
		PDEBUG(DEBUG_SET_DATA, "pv: %s, best_Bpm%1droc%s %lf)\n", pvName, bpm_sel, pos_sel, *(double*)payload);
		return 0;
	}

	//=========================================================================
	else if ( ( sscanf(pvName, "best_Bpm%1doffset%1s", &bpm_sel, pos_sel) == 2) &&
			  ( strlen(pvName) == 16) ) {
		std::ostringstream ss;
		ss << *(double *)payload;
		std::string payload_formatted_str = ss.str();
		std::string pos_sel_str = std::string(pos_sel);
		if	  (bpm_sel == 0) section = "BPM1";
		else if (bpm_sel == 1) section = "BPM2"; 
		double offsetX, offsetY;
		getBPMoffset(&offsetX, &offsetY, bpm_sel);
		if	  (pos_sel_str == "X") { offsetX = *(double *)payload; key = "Offset_X"; }
		else if (pos_sel_str == "Y") { offsetY = *(double *)payload; key = "Offset_Y"; }
		else						 return -1;
		errLoginLevel = setBPMoffset(offsetX, offsetY, bpm_sel);
		if (!errLoginLevel) { ini[section][key] = payload_formatted_str; file.write(ini); }
		PDEBUG(DEBUG_RET_DATA, "pv: %s, data: %lf\n", pvName, *(double*)payload);
		PDEBUG(DEBUG_SET_DATA, "pv: %s, best_Bpm%1doffset%1s %lf)\n", pvName, bpm_sel, pos_sel, *(double*)payload);
		return 0;
	}

	//=========================================================================
	else if( sscanf(pvName, "best_PidImax%2s", pos_sel) == 1){
		key = "Imax";
		std::ostringstream ss;
		ss << *(double *)payload;
		std::string payload_formatted_str = ss.str();
		int pid_par = 4;
		std::string pos_sel_str = std::string(pos_sel);
		enum best_pid_select pid_sel;
		if      (pos_sel_str == "X")  { pid_sel = BEST_PID_SELECT_X; section = "PID_X";}
		else if (pos_sel_str == "Y")  { pid_sel = BEST_PID_SELECT_Y; section = "PID_Y";}
		else if (pos_sel_str == "I0") { pid_sel = BEST_PID_SELECT_I0; section = "PID_I0"; }
		else                          return -1;
				
		PDEBUG(DEBUG_SET_DATA, "pv: %s, setPIDparamSingle(%d, %d, %lf)\n",
			   pvName, (int)pid_sel, pid_par, *(double*)payload);

		errLoginLevel = setPIDparamSingle(pid_sel, pid_par, *(double*)payload);
		if (!errLoginLevel) { ini[section][key] = payload_formatted_str; file.write(ini); }
		PDEBUG(DEBUG_SET_DATA, "pv: %s, setPIDparamSingle(%d, %d, %lf)\n",pvName, (int)pid_sel, pid_par, *(double*)payload);
		return 0;
	}

	//=========================================================================
	else if ( ( sscanf(pvName, "best_TetrAMM%1drange", &bpm_sel) == 1 ) &&
			  ( strlen(pvName) == 18 ) ) {
		value = *(unsigned short*)payload;
		PDEBUG(DEBUG_SET_DATA,
			   "pv: %s, tetrammSetRange(%d, %d)\n",
			   pvName, value, bpm_sel);
		int pos_A, pos_B;
		getTetramms(&pos_A, &pos_B);
		if ((!bpm_sel && !pos_A) || (bpm_sel && !pos_B)) return -1;
		tetrammSetRange(value, bpm_sel);

		return 0;
	}

	//=========================================================================
	else if( sscanf(pvName, "best_Pidemin%2s", pos_sel) == 1){
		key = "emin";
		int pid_par = 3;
		std::ostringstream ss;
		ss << *(double *)payload;
		std::string payload_formatted_str = ss.str();
		std::string pos_sel_str = std::string(pos_sel);
		enum best_pid_select pid_sel;
		if      (pos_sel_str == "X")  { pid_sel = BEST_PID_SELECT_X; section = "PID_X"; }
		else if (pos_sel_str == "Y")  { pid_sel = BEST_PID_SELECT_Y; section = "PID_Y"; }
		else if (pos_sel_str == "I0") { pid_sel = BEST_PID_SELECT_I0; section = "PID_I0"; }
		else                          return -1;
				
		PDEBUG(DEBUG_SET_DATA, "pv: %s, setPIDparamSingle(%d, %d, %lf)\n",
			   pvName, (int)pid_sel, pid_par, *(double*)payload);

		errLoginLevel = setPIDparamSingle(pid_sel, pid_par, *(double*)payload);
		if (!errLoginLevel) { ini[section][key] = payload_formatted_str; file.write(ini); }
		PDEBUG(DEBUG_SET_DATA, "pv: %s, setPIDparamSingle(%d, %d, %lf)\n", pvName, (int)pid_sel, pid_par, *(double*)payload);
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
	else if (pv_name_str == "best_PidConfigIN") {
		value = *(unsigned short*)payload;
		errLoginLevel = setPIDconf(value);
		if (!errLoginLevel) {
			if      (value == 0)  { ini["PID_X"]["enable"]="1"; ini["PID_Y"]["enable"]="0"; ini["PID_Ylf"]["enable"]="0"; }
			else if (value == 1)  { ini["PID_X"]["enable"]="1"; ini["PID_Y"]["enable"]="1"; ini["PID_Ylf"]["enable"]="0"; }
			else if (value == 2)  { ini["PID_X"]["enable"]="1"; ini["PID_Y"]["enable"]="0"; ini["PID_Ylf"]["enable"]="1"; }
			else if (value == 3)  { ini["PID_X"]["enable"]="0"; ini["PID_Y"]["enable"]="1"; ini["PID_Ylf"]["enable"]="0"; }
			else if (value == 4)  { ini["PID_X"]["enable"]="0"; ini["PID_Y"]["enable"]="1"; ini["PID_Ylf"]["enable"]="1"; }
			else if (value == 5)  { ini["PID_X"]["enable"]="1"; ini["PID_Y"]["enable"]="1"; ini["PID_Ylf"]["enable"]="1"; }
			else if (value == 6)  { ini["PID_X"]["enable"]="0"; ini["PID_Y"]["enable"]="0"; ini["PID_Ylf"]["enable"]="1"; }
			else {
				printf("%s", "error setPIDconf()"); return -1;
			}
			file.write(ini);
		}
		return 0;
	}
	//=========================================================================
	else if ( pv_name_str == "best_PidConfigOUT" ) {
		int conf = (int)*(double *)payload;
		switch (conf) {
			case 123:
				errLoginLevel = setPIDoutSel((uint8_t)0,(uint8_t)1,(uint8_t)2);
				if (!errLoginLevel) { ini["Beamline"].set({ {"DAC_posX","0"},{"DAC_posY","1"},{"DAC_I0","2"} }); file.write(ini); }; break;
			case 132:
				errLoginLevel = setPIDoutSel((uint8_t)0,(uint8_t)2,(uint8_t)1);
				if (!errLoginLevel) { ini["Beamline"].set({ {"DAC_posX","0"},{"DAC_posY","2"},{"DAC_I0","1"} }); file.write(ini); }; break;
			case 213:
				errLoginLevel = setPIDoutSel((uint8_t)1,(uint8_t)0,(uint8_t)2);
				if (!errLoginLevel) { ini["Beamline"].set({ {"DAC_posX","1"},{"DAC_posY","0"},{"DAC_I0","2"} }); file.write(ini); }; break;
			case 231:
				errLoginLevel = setPIDoutSel((uint8_t)1,(uint8_t)2,(uint8_t)0);
				if (!errLoginLevel) { ini["Beamline"].set({ {"DAC_posX","1"},{"DAC_posY","2"},{"DAC_I0","0"} }); file.write(ini); }; break;
			case 312:
				errLoginLevel = setPIDoutSel((uint8_t)2,(uint8_t)0,(uint8_t)1);
				if (!errLoginLevel) { ini["Beamline"].set({ {"DAC_posX","2"},{"DAC_posY","0"},{"DAC_I0","1"} }); file.write(ini); }; break;
			case 321:
				errLoginLevel = setPIDoutSel((uint8_t)2,(uint8_t)1,(uint8_t)0);
				if (!errLoginLevel) { ini["Beamline"].set({ {"DAC_posX","2"},{"DAC_posY","1"},{"DAC_I0","0"} }); file.write(ini); }; break;
			default: printf("%s", "error setPIDoutSel()"); return -1; break;
		}
		PDEBUG(DEBUG_RET_DATA, "pv: %s, data: %lf\n", pvName, *(double*)payload);
		PDEBUG(DEBUG_SET_DATA, "pv: %s, best_PidConfigOUT %lf)\n", pvName, *(double*)payload);
		return 0;
	}
	//=========================================================================
	else if ( ( sscanf(pvName, "best_Bpm%1dorient", &bpm_sel) == 1 ) &&
			  ( strlen(pvName) == 15 ) ) {
		key = "Orient";
		std::string payload_str = std::to_string(*(bool*)payload);
		uint8_t sel;
		if	  (bpm_sel == 1) { sel = 1; section = "BPM2"; }
		else if (bpm_sel == 0) { sel = 0; section = "BPM1"; }
		else				  return -1;
		uint8_t orient;
		*(bool*)payload ? orient = 1 : orient = 0;
		errLoginLevel = setBPMorient(sel, orient);
		if (!errLoginLevel) { ini[section][key] = payload_str; file.write(ini); }
		PDEBUG(DEBUG_RET_DATA, "pv: %s, data: %lf\n", pvName, *(double*)payload);
		PDEBUG(DEBUG_SET_DATA, "pv: %s, best_Bpm%1dorient %lf)\n", pvName, bpm_sel, *(double*)payload);
		return 0;
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
		/*
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
		*/
		setOutputMux(*(short int*)payload);
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
	else if ( ( sscanf(pvName, "best_Pid%1s_update_frequency", pos_sel) == 1) &&
			  ( strlen(pvName) == 26 ) ) {
		
		int rc = 0;
		std::string key = "Freq";
		double freq = *(double *)payload;
		uint32_t nr_samp = lround(PID_MAX_FREQ / freq);
		std::ostringstream ss;
		ss << *(double *)payload;
		std::string payload_formatted_str = ss.str();
		std::string pos_sel_str = std::string(pos_sel);
		enum best_pid_select pid_sel;
		if	  (pos_sel_str == "X")   { pid_sel = BEST_PID_SELECT_X; section = "PID_X";}
		else if (pos_sel_str == "Y")   { pid_sel = BEST_PID_SELECT_Y; section = "PID_Y";}
		else if (pos_sel_str == "I")   { pid_sel = BEST_PID_SELECT_I0; section = "PID_I0";}
		else							return -1;
				
		PDEBUG(DEBUG_SET_DATA, "pv: %s, setWindAvg(%d, %d)\n",pvName, (int)pid_sel, nr_samp);

		if ((freq > PID_MAX_FREQ) || (freq < 1.0))
		{
			rc = -1;
		}
		else
		{
			rc = setWindAvg(pid_sel, nr_samp);
			if (!rc) { ini[section][key] = payload_formatted_str; file.write(ini); }
		}

		return rc;
	}

	//=========================================================================
	else if (pv_name_str == "best_SelectBpm")
	{
		value = *(short int *)payload;
		errLoginLevel = setBPMselector((value & 0x4) >> 2, (value & 0x2) >> 1, value & 0x1);
		section = "Beamline";
		if (errLoginLevel) {
			if ((value & 0x4) >> 2) ini[section]["BPM_posX"] = "1"; else ini[section]["BPM_posX"] = "0";
			if ((value & 0x2) >> 1) ini[section]["BPM_posY"] = "1"; else ini[section]["BPM_posY"] = "0";
			if (value & 0x1)        ini[section]["BPM_I0"]   = "1"; else ini[section]["BPM_I0"]   = "0";
			file.write(ini);
		}
		PDEBUG(DEBUG_SET_DATA, "pv: %s, best_SelectBpm %lf)\n", pvName, *(double *)payload);
		return 0;
	}

	//=========================================================================
	else if ( ( sscanf(pvName, "best_Bpm%1dcrossbar", &bpm_sel) == 1 ) &&
			  ( strlen(pvName) == 17 ) ) {
		uint8_t sel;
		if	  (bpm_sel == 0) { sel = 0; section = "BPM1"; }
		else if (bpm_sel == 1) { sel = 1; section = "BPM2"; }
		else				  return -1;
		int conf = (int) *(double *)payload;
		switch (conf) {
			case 1234:
				errLoginLevel = setCrossBar(sel, 0, 1, 2, 3);
				if (!errLoginLevel) { ini[section].set({ {"Top","0"},{"Right","1"},{"Bottom","2"},{"Left","3"} }); file.write(ini); }; break;
			case 1243:
				errLoginLevel = setCrossBar(sel, 0, 1, 3, 2);
				if (!errLoginLevel) { ini[section].set({ {"Top","0"},{"Right","1"},{"Bottom","3"},{"Left","2"} }); file.write(ini); }; break;
			case 1324:
				errLoginLevel = setCrossBar(sel, 0, 2, 1, 3);
				if (!errLoginLevel) { ini[section].set({ {"Top","0"},{"Right","2"},{"Bottom","1"},{"Left","3"} }); file.write(ini); }; break;
			case 1342:
				errLoginLevel = setCrossBar(sel, 0, 2, 3, 1);
				if (!errLoginLevel) { ini[section].set({ {"Top","0"},{"Right","2"},{"Bottom","3"},{"Left","1"} }); file.write(ini); }; break;
			case 1423:
				errLoginLevel = setCrossBar(sel, 0, 3, 1, 2);
				if (!errLoginLevel) { ini[section].set({ {"Top","0"},{"Right","3"},{"Bottom","1"},{"Left","2"} }); file.write(ini); }; break;
			case 1432:
				errLoginLevel = setCrossBar(sel, 0, 3, 2, 1);
				if (!errLoginLevel) { ini[section].set({ {"Top","0"},{"Right","3"},{"Bottom","2"},{"Left","1"} }); file.write(ini); }; break;
			case 2134:
				errLoginLevel = setCrossBar(sel, 1, 0, 2, 3);
				if (!errLoginLevel) { ini[section].set({ {"Top","1"},{"Right","0"},{"Bottom","2"},{"Left","3"} }); file.write(ini); }; break;
			case 2143:
				errLoginLevel = setCrossBar(sel, 1, 0, 3, 2);
				if (!errLoginLevel) { ini[section].set({ {"Top","1"},{"Right","0"},{"Bottom","3"},{"Left","2"} }); file.write(ini); }; break;
			case 2314:
				errLoginLevel = setCrossBar(sel, 1, 2, 0, 3);
				if (!errLoginLevel) { ini[section].set({ {"Top","1"},{"Right","2"},{"Bottom","0"},{"Left","3"} }); file.write(ini); }; break;
			case 2341:
				errLoginLevel = setCrossBar(sel, 1, 2, 3, 0);
				if (!errLoginLevel) { ini[section].set({ {"Top","1"},{"Right","2"},{"Bottom","3"},{"Left","0"} }); file.write(ini); }; break;
			case 2413:
				errLoginLevel = setCrossBar(sel, 1, 3, 0, 2);
				if (!errLoginLevel) { ini[section].set({ {"Top","1"},{"Right","3"},{"Bottom","0"},{"Left","2"} }); file.write(ini); }; break;
			case 2431:
				errLoginLevel = setCrossBar(sel, 1, 3, 2, 0);
				if (!errLoginLevel) { ini[section].set({ {"Top","1"},{"Right","3"},{"Bottom","2"},{"Left","0"} }); file.write(ini); }; break;
			case 3124:
				errLoginLevel = setCrossBar(sel, 2, 0, 1, 3);
				if (!errLoginLevel) { ini[section].set({ {"Top","2"},{"Right","0"},{"Bottom","1"},{"Left","3"} }); file.write(ini); }; break;
			case 3142:
				errLoginLevel = setCrossBar(sel, 2, 0, 3, 1);
				if (!errLoginLevel) { ini[section].set({ {"Top","2"},{"Right","0"},{"Bottom","3"},{"Left","1"} }); file.write(ini); }; break;
			case 3214:
				errLoginLevel = setCrossBar(sel, 2, 1, 0, 3);
				if (!errLoginLevel) { ini[section].set({ {"Top","2"},{"Right","1"},{"Bottom","0"},{"Left","3"} }); file.write(ini); }; break;
			case 3241:
				errLoginLevel = setCrossBar(sel, 2, 1, 3, 0);
				if (!errLoginLevel) { ini[section].set({ {"Top","2"},{"Right","1"},{"Bottom","3"},{"Left","0"} }); file.write(ini); }; break;
			case 3412:
				errLoginLevel = setCrossBar(sel, 2, 3, 0, 1);
				if (!errLoginLevel) { ini[section].set({ {"Top","2"},{"Right","3"},{"Bottom","0"},{"Left","1"} }); file.write(ini); }; break;
			case 3421:
				errLoginLevel = setCrossBar(sel, 2, 3, 1, 0);
				if (!errLoginLevel) { ini[section].set({ {"Top","2"},{"Right","3"},{"Bottom","1"},{"Left","0"} }); file.write(ini); }; break;
			case 4123:
				errLoginLevel = setCrossBar(sel, 3, 0, 1, 2);
				if (!errLoginLevel) { ini[section].set({ {"Top","3"},{"Right","0"},{"Bottom","1"},{"Left","2"} }); file.write(ini); }; break;
			case 4132:
				errLoginLevel = setCrossBar(sel, 3, 0, 2, 1);
				if (!errLoginLevel) { ini[section].set({ {"Top","3"},{"Right","0"},{"Bottom","2"},{"Left","1"} }); file.write(ini); }; break;
			case 4213:
				errLoginLevel = setCrossBar(sel, 3, 1, 0, 2);
				if (!errLoginLevel) { ini[section].set({ {"Top","3"},{"Right","1"},{"Bottom","0"},{"Left","2"} }); file.write(ini); }; break;
			case 4231:
				errLoginLevel = setCrossBar(sel, 3, 1, 2, 0);
				if (!errLoginLevel) { ini[section].set({ {"Top","3"},{"Right","1"},{"Bottom","2"},{"Left","0"} }); file.write(ini); }; break;
			case 4312:
				errLoginLevel = setCrossBar(sel, 3, 2, 0, 1);
				if (!errLoginLevel) { ini[section].set({ {"Top","3"},{"Right","2"},{"Bottom","0"},{"Left","1"} }); file.write(ini); }; break;
			case 4321:
				errLoginLevel = setCrossBar(sel, 3, 2, 1, 0);
				if (!errLoginLevel) { ini[section].set({ {"Top","3"},{"Right","2"},{"Bottom","1"},{"Left","0"} }); file.write(ini); }; break;
			default: printf("%s", "error setCrossbar()"); return -1; break;
		}
		PDEBUG(DEBUG_SET_DATA, "pv: %s, best_Bpm%1dcrossbar %lf)\n", pvName, bpm_sel, *(double*)payload);
		return 0;
	}
	
	//=========================================================================
	else if ( pv_name_str == "best_Init" ) {
		int init_config_success = 0;
		
		std::string value1, value2, value3, value4, value5, value6;
		section = "BPM1";
		value1 = ini.get(section).get("Top");
		value2 = ini.get(section).get("Right");
		value3 = ini.get(section).get("Bottom");
		value4 = ini.get(section).get("Left");
		init_config_success = setCrossBar(0, std::stoi(value1), std::stoi(value2), std::stoi(value3), std::stoi(value4)) ? 1 : init_config_success;
		value1 = ini.get(section).get("Offset_X");
		value2 = ini.get(section).get("Offset_Y");
		init_config_success = setBPMoffset(std::stod(value1), std::stod(value2), 0) ? 1 : init_config_success;
		value1 = ini.get(section).get("Scale_X");
		value2 = ini.get(section).get("Scale_Y");
		init_config_success = setBPMscale(std::stod(value1), std::stod(value2), 0) ? 1 : init_config_success;

		section = "BPM2";
		value1 = ini.get(section).get("Top");
		value2 = ini.get(section).get("Right");
		value3 = ini.get(section).get("Bottom");
		value4 = ini.get(section).get("Left");
		init_config_success = setCrossBar(1, std::stoi(value1), std::stoi(value2), std::stoi(value3), std::stoi(value4)) ? 1 : init_config_success;
		value1 = ini.get(section).get("Offset_X");
		value2 = ini.get(section).get("Offset_Y");
		init_config_success = setBPMoffset(std::stod(value1), std::stod(value2), 1) ? 1 : init_config_success;
		value1 = ini.get(section).get("Scale_X");
		value2 = ini.get(section).get("Scale_Y");
		init_config_success = setBPMscale(std::stod(value1), std::stod(value2), 1) ? 1 : init_config_success;

		section = "Beamline";
		value1 = ini.get(section).get("DAC_posX");
		value2 = ini.get(section).get("DAC_posY");
		value3 = ini.get(section).get("DAC_I0");
		init_config_success = setPIDoutSel((uint8_t)std::stoi(value1), (uint8_t)std::stoi(value2), (uint8_t)std::stoi(value3)) ? 1 : init_config_success;
		value1 = ini.get(section).get("BPM_posX");
		value2 = ini.get(section).get("BPM_posY");
		value3 = ini.get(section).get("BPM_I0");
		// setBPMselector: 0 = failure, 1 = success
		init_config_success = setBPMselector(std::stoi(value1), std::stoi(value2), std::stoi(value3)) ? init_config_success : 1;

		section = "PID_I0";
		value1 = ini.get(section).get("Freq");
		init_config_success = setWindAvg(BEST_PID_SELECT_I0, lround(PID_MAX_FREQ / std::stod(value1))) ? 1 : init_config_success;
		value1 = ini.get(section).get("Offset");
		init_config_success = setOffsetSingle(BEST_PID_SELECT_I0, std::stod(value1)) ? 1 : init_config_success;
		value1 = ini.get(section).get("Kd");
		init_config_success = setPIDparamSingle(BEST_PID_SELECT_I0, 2, std::stod(value1)) ? 1 : init_config_success;
		value1 = ini.get(section).get("Ki");
		init_config_success = setPIDparamSingle(BEST_PID_SELECT_I0, 1, std::stod(value1)) ? 1 : init_config_success;
		value1 = ini.get(section).get("Kp");
		init_config_success = setPIDparamSingle(BEST_PID_SELECT_I0, 0, std::stod(value1)) ? 1 : init_config_success;
		value1 = ini.get(section).get("Ogain");
		init_config_success = setPIDparamSingle(BEST_PID_SELECT_I0, 7, std::stod(value1)) ? 1 : init_config_success;
		value1 = ini.get(section).get("Omax");
		init_config_success = setPIDparamSingle(BEST_PID_SELECT_I0, 6, std::stod(value1)) ? 1 : init_config_success;
		value1 = ini.get(section).get("Omin");
		init_config_success = setPIDparamSingle(BEST_PID_SELECT_I0, 5, std::stod(value1)) ? 1 : init_config_success;
		value1 = ini.get(section).get("emin");
		init_config_success = setPIDparamSingle(BEST_PID_SELECT_I0, 3, std::stod(value1)) ? 1 : init_config_success;
		value1 = ini.get(section).get("Imax");
		init_config_success = setPIDparamSingle(BEST_PID_SELECT_I0, 4, std::stod(value1)) ? 1 : init_config_success;
		
		section = "PID_X";
		value1 = ini.get(section).get("Freq");
		init_config_success = setWindAvg(BEST_PID_SELECT_X, lround(PID_MAX_FREQ / std::stod(value1))) ? 1 : init_config_success;
		value1 = ini.get(section).get("Offset");
		init_config_success = setOffsetSingle(BEST_PID_SELECT_X, std::stod(value1)) ? 1 : init_config_success;
		value1 = ini.get(section).get("Kd");
		init_config_success = setPIDparamSingle(BEST_PID_SELECT_X, 2, std::stod(value1)) ? 1 : init_config_success;
		value1 = ini.get(section).get("Ki");
		init_config_success = setPIDparamSingle(BEST_PID_SELECT_X, 1, std::stod(value1)) ? 1 : init_config_success;
		value1 = ini.get(section).get("Kp");
		init_config_success = setPIDparamSingle(BEST_PID_SELECT_X, 0, std::stod(value1)) ? 1 : init_config_success;
		value1 = ini.get(section).get("Ogain");
		init_config_success = setPIDparamSingle(BEST_PID_SELECT_X, 7, std::stod(value1)) ? 1 : init_config_success;
		value1 = ini.get(section).get("Omax");
		init_config_success = setPIDparamSingle(BEST_PID_SELECT_X, 6, std::stod(value1)) ? 1 : init_config_success;
		value1 = ini.get(section).get("Omin");
		init_config_success = setPIDparamSingle(BEST_PID_SELECT_X, 5, std::stod(value1)) ? 1 : init_config_success;
		value1 = ini.get(section).get("emin");
		init_config_success = setPIDparamSingle(BEST_PID_SELECT_X, 3, std::stod(value1)) ? 1 : init_config_success;
		value1 = ini.get(section).get("Imax");
		init_config_success = setPIDparamSingle(BEST_PID_SELECT_X, 4, std::stod(value1)) ? 1 : init_config_success;

		section = "PID_Y";
		value1 = ini.get(section).get("Freq");
		init_config_success = setWindAvg(BEST_PID_SELECT_Y, lround(PID_MAX_FREQ / std::stod(value1))) ? 1 : init_config_success;
		value1 = ini.get(section).get("Offset");
		init_config_success = setOffsetSingle(BEST_PID_SELECT_Y, std::stod(value1)) ? 1 : init_config_success;
		value1 = ini.get(section).get("Kd");
		init_config_success = setPIDparamSingle(BEST_PID_SELECT_Y, 2, std::stod(value1)) ? 1 : init_config_success;
		value1 = ini.get(section).get("Ki");
		init_config_success = setPIDparamSingle(BEST_PID_SELECT_Y, 1, std::stod(value1)) ? 1 : init_config_success;
		value1 = ini.get(section).get("Kp");
		init_config_success = setPIDparamSingle(BEST_PID_SELECT_Y, 0, std::stod(value1)) ? 1 : init_config_success;
		value1 = ini.get(section).get("Ogain");
		init_config_success = setPIDparamSingle(BEST_PID_SELECT_Y, 7, std::stod(value1)) ? 1 : init_config_success;
		value1 = ini.get(section).get("Omax");
		init_config_success = setPIDparamSingle(BEST_PID_SELECT_Y, 6, std::stod(value1)) ? 1 : init_config_success;
		value1 = ini.get(section).get("Omin");
		init_config_success = setPIDparamSingle(BEST_PID_SELECT_Y, 5, std::stod(value1)) ? 1 : init_config_success;
		value1 = ini.get(section).get("emin");
		init_config_success = setPIDparamSingle(BEST_PID_SELECT_Y, 3, std::stod(value1)) ? 1 : init_config_success;
		value1 = ini.get(section).get("Imax");
		init_config_success = setPIDparamSingle(BEST_PID_SELECT_Y, 4, std::stod(value1)) ? 1 : init_config_success;

		value1 = ini.get("PID_X").get("enable");
		value2 = ini.get("PID_Y").get("enable");
		value3 = ini.get("PID_I0").get("enable");
		if      ( (value1 == "1") && (value2 == "0") && (value3 == "0") ) init_config_success = setPIDconf(0) ? 1 : init_config_success;
		else if ( (value1 == "1") && (value2 == "1") && (value3 == "0") ) init_config_success = setPIDconf(1) ? 1 : init_config_success;
		else if ( (value1 == "1") && (value2 == "0") && (value3 == "1") ) init_config_success = setPIDconf(2) ? 1 : init_config_success;
		else if ( (value1 == "0") && (value2 == "1") && (value3 == "0") ) init_config_success = setPIDconf(3) ? 1 : init_config_success;
		else if ( (value1 == "0") && (value2 == "1") && (value3 == "1") ) init_config_success = setPIDconf(4) ? 1 : init_config_success;
		else if ( (value1 == "1") && (value2 == "1") && (value3 == "1") ) init_config_success = setPIDconf(5) ? 1 : init_config_success;
		else if ( (value1 == "0") && (value2 == "0") && (value3 == "1") ) init_config_success = setPIDconf(6) ? 1 : init_config_success;

		// ROC
		section = "ROCandROI";
		value1 = ini.get(section).get("BeamOffTh");
		init_config_success = setROClimits(0x2, 0, std::stod(value1)) ? 1 : init_config_success;
		value1 = ini.get(section).get("RocX");
		init_config_success = setROClimits(0x0, 0, std::stod(value1)) ? 1 : init_config_success;
		value1 = ini.get(section).get("RocY");
		init_config_success = setROClimits(0x1, 0, std::stod(value1)) ? 1 : init_config_success;
		section = "ROCandROI2";
		value1 = ini.get(section).get("BeamOffTh");
		init_config_success = setROClimits(0x5, 0, std::stod(value1)) ? 1 : init_config_success;
		value1 = ini.get(section).get("RocX");
		init_config_success = setROClimits(0x3, 0, std::stod(value1)) ? 1 : init_config_success;
		value1 = ini.get(section).get("RocY");
		init_config_success = setROClimits(0x4, 0, std::stod(value1)) ? 1 : init_config_success;

		// ROC enable
		section = "ROCandROI";
		value1 = ini.get(section).get("BeamOffEnable");
		value2 = ini.get(section).get("RocXenable");
		value3 = ini.get(section).get("RocYenable");
		section = "ROCandROI2";
		value4 = ini.get(section).get("BeamOffEnable");
		value5 = ini.get(section).get("RocXenable");
		value6 = ini.get(section).get("RocYenable");
		bool rocX, rocY, rocI, rocX2, rocY2, rocI2;
		if (value2 == "true") rocX = 1;  else if (value2 == "false") rocX = 0;  else return -1;
		if (value3 == "true") rocY = 1;  else if (value3 == "false") rocY = 0;  else return -1;
		if (value1 == "true") rocI = 1;  else if (value1 == "false") rocI = 0;  else return -1;
		if (value5 == "true") rocX2 = 1; else if (value5 == "false") rocX2 = 0; else return -1;
		if (value6 == "true") rocY2 = 1; else if (value6 == "false") rocY2 = 0; else return -1;
		if (value4 == "true") rocI2 = 1; else if (value4 == "false") rocI2 = 0; else return -1;
		init_config_success = setROCenable(rocX, rocY, rocI, rocX2, rocY2, rocI2) ? 1 : init_config_success;

		// Ignored
		// RoiI0max	// ROC
		// RoiI0min	// ROC
		// RoiX	// ROC
		// RoiY	// ROC
		
		(init_config_success) ? *(unsigned short *)payload = 1 : *(unsigned short *)payload = 0 ;
		PDEBUG(DEBUG_SET_DATA, "pv: %s, best_Init %d)\n", pvName, init_config_success);
		return 0;
	}

	//=========================================================================
	else if ( ( sscanf(pvName, "best_TetrAMM%1dHVenable", &bpm_sel) == 1 ) && 
			  ( strlen(pvName) == 21 ) ) {
		uint8_t sel;
		if      (bpm_sel == 0) sel = 0;
		else if (bpm_sel == 1) sel = 1;
		else                   return -1;
		value = *(unsigned short*)payload;
		int pos_A, pos_B;
		getTetramms(&pos_A, &pos_B);
		if ((!bpm_sel && !pos_A) || (bpm_sel && !pos_B)) return -1;
		tetrammHVenable(value, sel);
		PDEBUG(DEBUG_SET_DATA, "pv: %s, best_TetrAMM%1dHVenable %d\n", pvName, bpm_sel, value);
		return 0;
	}

	//=========================================================================
	else if  ( sscanf(pvName, "best_TetrAMM%1dHVvoltage", &bpm_sel) == 1 ) {
		float voltage = (float) *(double *)payload;
		int pos_A, pos_B;
		getTetramms(&pos_A, &pos_B);
		if ((!bpm_sel && !pos_A) || (bpm_sel && !pos_B)) return -1;
		tetrammHVSetVoltage(voltage, bpm_sel);
		PDEBUG(DEBUG_SET_DATA, "pv: %s, best_TetrAMM%1dHVvoltage %lf\n", pvName, bpm_sel, *(double*)payload);
		return 0;
	}

	//=========================================================================
	else {
		PDEBUG(DEBUG_ERROR, " %s: unknown name (writeBest)\n", pvName);
	}

	return -1;
}
