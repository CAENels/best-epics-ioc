
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
    //double roiX, roiY, roiI0min, roiI0max, roc;
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
    int ch_sel; // 0 1
    char kappa_sel[16] = {0}; // p i d max min gai
    char pos_sel[16] = {0}; // X Y I Xlf Xhf Ylf Yhf X2 Y2 I2
    char filt_sel[16] = {0}; // L B
    char param_sel[16] = {0}; // A B
    int coef_sel;
    unsigned short status;
    std::string pv_name_str = std::string(pvName);

    //=========================================================================
    if ( sscanf(pvName, "best_BPM%dscale%s", &ch_sel, pos_sel) == 2 ) {
        double scaleX, scaleY;
        getBPMscale(ch_sel, &scaleX, &scaleY);
        std::string pos_sel_str = std::string(pos_sel);
        if      (pos_sel_str == "X") *(double*)payload = scaleX;
        else if (pos_sel_str == "Y") *(double*)payload = scaleY;
        else                         return -1;
        PDEBUG(DEBUG_RET_DATA, "pv: %s, data: %lf\n", pvName, *(double*)payload);
        return 0;
    }

    //=========================================================================
    else if ( sscanf(pvName, "best_BPM%dwfm%1s", &ch_sel, pos_sel) == 2 ) {
        std::string pos_sel_str = std::string(pos_sel);
        double *posX = (double*)malloc(count*sizeof(double));
        double *posY = (double*)malloc(count*sizeof(double));
        double *I0 = (double*)malloc(count*sizeof(double));
        getPosArray_sel(ch_sel, posX, posY, I0, count);
        if      (pos_sel_str == "X")   memcpy(payload, posX, count*sizeof(double));
        else if (pos_sel_str == "Y")   memcpy(payload, posY, count*sizeof(double));
        else if (pos_sel_str == "I") memcpy(payload, I0, count*sizeof(double));
        else                           {free(posX); free(posY); free(I0); return -1;}
        PDEBUG(DEBUG_RET_DATA, "pv: %s, data[0]: %lf\n", pvName, *(double*)payload);
        free(posX); free(posY); free(I0);
        return 0;
    }

    //=========================================================================
    else if( sscanf(pvName, "best_TetrAMM%dch%d", &ch_sel, &ch) == 2 ) {
        double (*current)[4];
        current = (double(*)[4])malloc(4*count*sizeof(double));
        getCurrentArray_sel(ch_sel, current, count);
        int i = 0;
        double *d_ptr = (double*)payload;
        for(i = 0; i < count; i++){
            *d_ptr++ = current[i][ch-1];
        }
        PDEBUG(DEBUG_RET_DATA, "pv: %s, data: %lf\n", pvName, *(double*)payload);
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
    else if (pv_name_str == "best_rbv_PreDACoutmux") {
        /*
        int fd = open(FILE_MBOX, O_RDWR | O_SYNC);
        if(fd < 0) {
            printf("Error opening FILE_MBOX\n");
            return -1; }
        struct mail comm;
        memset(comm.payload, 0, 28);
        comm.cmd = CMD_SIG_DEMUX << 8 | CMD_READ;
        // emit io control
        int ret = ioctl(fd, IOCTL_MAIL_COMM, &comm);
        if (ret < 0){
            printf("ioctl(IOCTL_MAIL_COMM) failed\n");
            return -1; }
        *(short int*)payload = (short int)comm.payload[0];
        close(fd);
        return 0;
        */
        uint8_t sel;
        getOutputMux(&sel);
        *(short int*)payload = sel;
        return 0;
    }

    //=========================================================================
    else if ( ( pv_name_str == "best_rbv_PidOffsetXlf") ||
              ( pv_name_str == "best_rbv_PidOffsetXhf") ||
              ( pv_name_str == "best_rbv_PidOffsetYlf") ||
              ( pv_name_str == "best_rbv_PidOffsetYhf") ) {
        enum best_pid_select pid_sel;
        if      (pv_name_str == "best_rbv_PidOffsetXlf") pid_sel = BEST_PID_SELECT_0;
        else if (pv_name_str == "best_rbv_PidOffsetXhf") pid_sel = BEST_PID_SELECT_1;
        else if (pv_name_str == "best_rbv_PidOffsetYlf") pid_sel = BEST_PID_SELECT_2;
        else if (pv_name_str == "best_rbv_PidOffsetYhf") pid_sel = BEST_PID_SELECT_3;
        else                                             return -1;
        getOffsetSingle(pid_sel, (double*)payload);
        PDEBUG(DEBUG_RET_DATA, "pv: %s, data: %lf\n", pvName, *(double*)payload);
        return 0;
    }

    //=========================================================================
    else if ( sscanf(pvName, "best_rbv_PidK%1s%3s", kappa_sel, pos_sel) == 2 ) {
        std::string K_sel_str = std::string(kappa_sel);
        int K_sel;
        if      (K_sel_str == "p") K_sel = 4;
        else if (K_sel_str == "i") K_sel = 12;
        else if (K_sel_str == "d") K_sel = 20;
        else                       return -1;
        std::string pos_sel_str = std::string(pos_sel);
        enum best_pid_select pid_sel;
        if      (pos_sel_str == "Xlf") pid_sel = BEST_PID_SELECT_0;
        else if (pos_sel_str == "Xhf") pid_sel = BEST_PID_SELECT_1;
        else if (pos_sel_str == "Ylf") pid_sel = BEST_PID_SELECT_2;
        else if (pos_sel_str == "Yhf") pid_sel = BEST_PID_SELECT_3;
        else                           return -1;
        getPIDparamSingle(K_sel, pid_sel, (int)pid_par_sel_K, (double*)payload);
        PDEBUG(DEBUG_RET_DATA, "pv: %s, data: %lf\n", pvName, *(double*)payload);
        return 0;
    }

    //=========================================================================
    else if ( sscanf(pvName, "best_rbv_PidO%3s%3s", kappa_sel, pos_sel) == 2 ) {
        std::string K_sel_str = std::string(kappa_sel);
        int K_sel;
        if      (K_sel_str == "min") K_sel = 4;
        else if (K_sel_str == "max") K_sel = 12;
        else if (K_sel_str == "gai") K_sel = 20;
        else                         return -1;
        std::string pos_sel_str = std::string(pos_sel);
        enum best_pid_select pid_sel;
        if      (pos_sel_str == "Xlf") pid_sel = BEST_PID_SELECT_0;
        else if (pos_sel_str == "Xhf") pid_sel = BEST_PID_SELECT_1;
        else if (pos_sel_str == "Ylf") pid_sel = BEST_PID_SELECT_2;
        else if (pos_sel_str == "Yhf") pid_sel = BEST_PID_SELECT_3;
        else                           return -1;
        getPIDparamSingle(K_sel, pid_sel, (int)pid_par_sel_out, (double*)payload);
        PDEBUG(DEBUG_RET_DATA, "pv: %s, data: %lf\n", pvName, *(double*)payload);
        return 0;
    }

    //=========================================================================
    else if ( sscanf(pvName, "best_rbv_PidImax%3s", pos_sel) == 1 ) {
        int K_sel = 12;  
        std::string pos_sel_str = std::string(pos_sel);
        enum best_pid_select pid_sel;
        if      (pos_sel_str == "Xlf") pid_sel = BEST_PID_SELECT_0;
        else if (pos_sel_str == "Xhf") pid_sel = BEST_PID_SELECT_1;
        else if (pos_sel_str == "Ylf") pid_sel = BEST_PID_SELECT_2;
        else if (pos_sel_str == "Yhf") pid_sel = BEST_PID_SELECT_3;
        else                           return -1;
        getPIDparamSingle(K_sel, pid_sel, (int)pid_par_sel_lim, (double*)payload);
        PDEBUG(DEBUG_RET_DATA, "pv: %s, data: %lf\n", pvName, *(double*)payload);
        return 0;
    }

    //=========================================================================
    else if ( sscanf(pvName, "best_rbv_Pidemin%3s", pos_sel) == 1 ) {
        int K_sel = 4;
        std::string pos_sel_str = std::string(pos_sel);
        enum best_pid_select pid_sel;
        if      (pos_sel_str == "Xlf") pid_sel = BEST_PID_SELECT_0;
        else if (pos_sel_str == "Xhf") pid_sel = BEST_PID_SELECT_1;
        else if (pos_sel_str == "Ylf") pid_sel = BEST_PID_SELECT_2;
        else if (pos_sel_str == "Yhf") pid_sel = BEST_PID_SELECT_3;
        else                           return -1;
        getPIDparamSingle(K_sel, pid_sel, (int)pid_par_sel_lim, (double*)payload);
        PDEBUG(DEBUG_RET_DATA, "pv: %s, data: %lf\n", pvName, *(double*)payload);
        return 0;
    }

    //=========================================================================
    else if ( sscanf(pvName, "best_rbv_PreDAC_Ch%d", &ch) == 1 ) {

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
    else if ( sscanf(pvName, "best_rbv_Pidroc%1d", &ch_sel) == 1 ) {
        /*
        double roiX, roiY, roiI0min, roiI0max, roc;
        getROCandROI_sel(ch_sel, &roiX, &roiY, &roiI0min, &roiI0max, &roc);
        double scaleX, scaleY;
        getBPMscale(ch_sel, &scaleX, &scaleY);
        char roc_sel;
        if      (ch_sel == 0 ) roc_sel = 0;
        else if (ch_sel == 1 ) roc_sel = 3;
        else                   return -1;
        double min, max;
        getROClimits(roc_sel, &min, &max);
        *(double *)payload = max * 100 * scaleX / roiX;
        */
        mINI::INIFile file(CONFIG_PATH);
        mINI::INIStructure ini;
        std::string section;
        file.read(ini);
        if      (ch_sel == 0 ) section = "ROCandROI";
        else if (ch_sel == 1 ) section = "ROCandROI2";
        else                   return -1;
        std::string roc_str = ini[section]["roc"];
        *(double*) payload = std::stod(roc_str);
        PDEBUG(DEBUG_RET_DATA, "pv: %s, data[0]: %lf\n", pvName, *(double*)payload);
        return 0;
    }

    //=========================================================================
    else if ( sscanf(pvName, "best_rbv_PidrocI%1d", &ch_sel) == 1 ) {
        double roiX, roiY, roiI0min, roiI0max, roc;
        getROCandROI_sel(ch_sel, &roiX, &roiY, &roiI0min, &roiI0max, &roc);
        char roc_sel;
        if      (ch_sel == 0) roc_sel = 0x2;
        else if (ch_sel == 1) roc_sel = 0x5;
        else                  return -1;
        double min, max;
        getROClimits(roc_sel, &min, &max);
        *(double *)payload = max * 1000000;
        PDEBUG(DEBUG_RET_DATA, "pv: %s, data[0]: %lf\n", pvName, *(double*)payload);
        return 0;
    }

    //=========================================================================
    else if ( sscanf(pvName, "best_rbv_Enaroc%2s", pos_sel) == 1 ) {
        bool rocX, rocY, rocI0, rocX2, rocY2, rocI02;
        getROCenable(&rocX, &rocY, &rocI0, &rocX2, &rocY2, &rocI02);
        std::string pos_sel_str = std::string(pos_sel);
        if      (pos_sel_str == "X")   *(double*)payload = rocX;
        else if (pos_sel_str == "Y")   *(double*)payload = rocY;
        else if (pos_sel_str == "I")   *(double*)payload = rocI0;
        else if (pos_sel_str == "X2")  *(double*)payload = rocX2;
        else if (pos_sel_str == "Y2")  *(double*)payload = rocY2;
        else if (pos_sel_str == "I2")  *(double*)payload = rocI02;
        else                           return -1;
        PDEBUG(DEBUG_RET_DATA, "pv: %s, data: %lf\n", pvName, *(double*)payload);
        return 0;
    }

    //=========================================================================
    else if ( pv_name_str == "best_rbv_PidConfigOUT" ) {
        uint8_t posXlf, posXhf, posYlf, posYhf;
        getPIDoutSel(&posXlf, &posXhf, &posYlf, &posYhf);
        *(double*)payload = (posXlf+1)*1000 + (posXhf+1)*100 + (posYlf+1)*10 + (posYhf+1);
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
    else if ( sscanf(pvName, "best_rbv_PidFilter%1sP%1s", filt_sel, pos_sel) == 2 ) {
        std::string filt_sel_str = std::string(filt_sel);
        char filt;
        if      (filt_sel_str == "L") filt = 0;
        else if (filt_sel_str == "B") filt = 1;
        else                          return -1; 
        std::string pos_sel_str = std::string(pos_sel);
        char pos;
        if      (pos_sel_str == "X") pos = 0;
        else if (pos_sel_str == "Y") pos = 1;
        else                         return -1;
        char enable;
        getFiltControl(filt, pos, &enable);
        *(unsigned short *)payload = enable;
        PDEBUG(DEBUG_RET_DATA, "pv: %s, data: %lf\n", pvName, *(double*)payload);
        return 0;
    }

    //=========================================================================
    else if ( sscanf(pvName, "best_rbv_PidFilterCoef%1sP%1s%1s%d", filt_sel, pos_sel, param_sel, &coef_sel) == 4 ) {
        std::string filt_sel_str = std::string(filt_sel);
        char filt;
        if      (filt_sel_str == "L") filt = 0;
        else if (filt_sel_str == "B") filt = 1;
        else                           return -1;
        std::string pos_sel_str = std::string(pos_sel);
        char pos;
        if      (pos_sel_str == "X") pos = 0;
        else if (pos_sel_str == "Y") pos = 1;
        else                         return -1;
        std::string param_sel_str = std::string(param_sel);
        char param;
        if      (param_sel_str == "A") param = 1;
        else if (param_sel_str == "B") param = 0;
        else                           return -1;
        char coef;
        if      (coef_sel == 0) coef = 0;
        else if (coef_sel == 1) coef = 1;
        else if (coef_sel == 2) coef = 2;
        else if (coef_sel == 3) coef = 3;
        else if (coef_sel == 4) coef = 4;
        else                    return -1;
        getFiltCoef(filt, pos, param, coef, (double*)payload);
        PDEBUG(DEBUG_RET_DATA, "pv: %s, data: %lf\n", pvName, *(double*)payload);
        return 0;
    }

    //=========================================================================
    else if ( sscanf(pvName, "best_rbv_PidFreq%3s", pos_sel) == 1 ) {
        /*
        std::string pos_sel_str = std::string(pos_sel);
        char pid_sel;
        if      (pos_sel_str == "Xlf") pid_sel = 0;
        else if (pos_sel_str == "Xhf") pid_sel = 1;
        else if (pos_sel_str == "Ylf") pid_sel = 2;
        else if (pos_sel_str == "Yhf") pid_sel = 3;
        else                           return -1;
        uint32_t freq;
        getWindAvg(pid_sel, &freq);
        *(double *)payload = lround(10000.0 / freq);
        */
        mINI::INIFile file(CONFIG_PATH);
        mINI::INIStructure ini;
        std::string section;
        file.read(ini);
        std::string pos_sel_str = std::string(pos_sel);
        if      (pos_sel_str == "Xlf") section = "PID_Xlf";
        else if (pos_sel_str == "Xhf") section = "PID_Xhf";
        else if (pos_sel_str == "Ylf") section = "PID_Ylf";
        else if (pos_sel_str == "Yhf") section = "PID_Yhf";
        else                           return -1;
        std::string freq_str = ini[section]["freq"];
        *(double*) payload = std::stod(freq_str);
        PDEBUG(DEBUG_RET_DATA, "pv: %s, data: %lf\n", pvName, *(double*)payload);
        return 0;
    }

    //=========================================================================
    else if ( sscanf(pvName, "best_rbv_BPMorient%d", &ch_sel) == 1 ) {
        char sel;
        if       (ch_sel == 0) sel = 0x0;
        else if  (ch_sel == 1) sel = 0x1;
        else                   return -1;
        uint8_t is90deg;
        getBPMorient(sel, &is90deg);
        *(uint8_t *)payload = is90deg;
        PDEBUG(DEBUG_RET_DATA, "pv: %s, data: %lf\n", pvName, *(double*)payload);
        return 0;
    }

    //=========================================================================
    else if ( ( pv_name_str == "best_rbv_PidSetpointXlf") ||
              ( pv_name_str == "best_rbv_PidSetpointYlf") ) {
        int sel;
        if      (pv_name_str == "best_rbv_PidSetpointXlf")  sel = 0;
        else if (pv_name_str == "best_rbv_PidSetpointYlf")  sel = 2;
        else                                                return -1;
        getSetpoint2(sel, (double*)payload);
        PDEBUG(DEBUG_RET_DATA, "pv: %s, data: %lf\n", pvName, *(double*)payload);
        return 0;
    }

    //=========================================================================
    else if ( sscanf(pvName, "best_rbv_BPMcrossbar%1d", &ch_sel) == 1 ) {
        uint8_t sel, posXlf, posXhf, posYlf, posYhf;
        if      (ch_sel == 0) sel = 0;
        else if (ch_sel == 1) sel = 1;
        else                  return -1;
        getCrossBar(sel, &posXlf, &posXhf, &posYlf, &posYhf);
        *(double*)payload = (posXlf+1)*1000 + (posXhf+1)*100 + (posYlf+1)*10 + (posYhf+1);
        PDEBUG(DEBUG_RET_DATA, "pv: %s, data: %lf\n", pvName, *(double*)payload);
        return 0;
    }

    //=========================================================================
    else if ( sscanf(pvName, "best_rbv_BPMoffset%1d%1s", &ch_sel, pos_sel) == 2 ) {
        double offsetX, offsetY;
        getBPMoffset(&offsetX, &offsetY, ch_sel);
        std::string pos_sel_str = std::string(pos_sel);
        if      (pos_sel_str == "X") *(double*)payload = offsetX;
        else if (pos_sel_str == "Y") *(double *)payload = offsetY;
        else                         return -1;
        PDEBUG(DEBUG_RET_DATA, "pv: %s, data: %lf\n", pvName, *(double*)payload);
        return 0;
    }
    
    //=========================================================================
    else if ( pv_name_str == "best_rbv_BPMselector" ) {
        int posXlf, posXhf, posYlf, posYhf;
        getBPMselector(&posXlf, &posXhf, &posYlf, &posYhf);
        short int value = (posXlf << 3) | (posXhf << 2) | (posYlf << 1) | (posYhf);
        *(unsigned short *) payload = value;
        PDEBUG(DEBUG_RET_DATA, "pv: %s, data: %lf\n", pvName, *(double*)payload);
        return 0;
    }

    //=========================================================================
    else if ( sscanf(pvName, "best_rbv_Pidroi%1d%1s", &ch_sel, pos_sel) == 2 ) {
        double roiX, roiY, roiI0min, roiI0max, roc;
        getROCandROI_sel(ch_sel, &roiX, &roiY, &roiI0min, &roiI0max, &roc);
        std::string pos_sel_str = std::string(pos_sel);
        if      (pos_sel_str == "X") *(double *)payload = roiX;
        else if (pos_sel_str == "Y") *(double *)payload = roiY;
        else                         return -1;
        PDEBUG(DEBUG_RET_DATA, "pv: %s, data: %lf\n", pvName, *(double*)payload);
        return 0;
    }

    //=========================================================================
    else if ( sscanf(pvName, "best_rbv_FilterFreq%5s", pos_sel) == 1 ) {
        mINI::INIFile file(CONFIG_PATH);
        mINI::INIStructure ini;
        std::string section = "PositionFilters";
        std::string key;
        file.read(ini);
        std::string pos_sel_str = std::string(pos_sel);
        if      (pos_sel_str == "LPXlf") key = "XlfFcutLow"; 
        else if (pos_sel_str == "LPYlf") key = "YlfFcutLow";
        else if (pos_sel_str == "LPXhf") key = "XhfFcutLow";
        else if (pos_sel_str == "LPYhf") key = "YhfFcutLow";
        else if (pos_sel_str == "HPXhf") key = "XhfFcutHigh";
        else if (pos_sel_str == "HPYhf") key = "YhfFcutHigh";
        else                             return -1;
        std::string freq_str = ini[section][key];
        *(double*) payload = std::stod(freq_str);
        PDEBUG(DEBUG_RET_DATA, "pv: %s, data: %lf\n", pvName, *(double*)payload);
        return 0;
    }

    //=========================================================================
    else {
        PDEBUG(DEBUG_ERROR, " %s: unknown name\n", pvName);
    }

    return -1; // unknown variable
}

int writeBest(char *pvName, retType_t type, void* payload) {

    PDEBUG(DEBUG_LOW_FUNC, "func: %s(), pv: %s\n", __FUNCTION__, pvName);

    unsigned short value;
    int predac_ch;
    int ch_sel;
    static double predac_out[4];
    char kappa_sel[16] = {0};
    char pos_sel[16] = {0};
    char filt_sel[16] = {0}; 
    char param_sel[16] = {0};
    int coef_sel;
    mINI::INIFile file(CONFIG_PATH);
    mINI::INIStructure ini;
    std::string section;
    std::string key;
    file.read(ini);
    std::string pv_name_str = std::string(pvName);

    //=========================================================================
    if( ( pv_name_str == "best_PidSetpointXlf") ||
        ( pv_name_str == "best_PidSetpointYlf") ) {
        int sel;
        if      (pv_name_str == "best_PidSetpointXlf") sel = 0;
        else if (pv_name_str == "best_PidSetpointYlf") sel = 2;
        else                                           return -1;
        setSetpoint2(sel, *(double*)payload);
        PDEBUG(DEBUG_RET_DATA, "pv: %s, data: %lf\n", pvName, *(double*)payload);
        return 0;
    }

    //=========================================================================
    else if ( ( pv_name_str == "best_PidOffsetXlf") ||
              ( pv_name_str == "best_PidOffsetXhf") ||
              ( pv_name_str == "best_PidOffsetYlf") ||
              ( pv_name_str == "best_PidOffsetYhf") ) {
        key = "Offset";
        std::string payload_str = std::to_string(*(double *)payload);
        std::string payload_formatted_str = payload_str.substr(0, payload_str.find(".") + 6);
        enum best_pid_select pid_sel;
        if      (pv_name_str == "best_PidOffsetXlf") { pid_sel = BEST_PID_SELECT_0; ini["PID_Xlf"][key] = payload_formatted_str; }
        else if (pv_name_str == "best_PidOffsetXhf") { pid_sel = BEST_PID_SELECT_1; ini["PID_Xhf"][key] = payload_formatted_str; }
        else if (pv_name_str == "best_PidOffsetYlf") { pid_sel = BEST_PID_SELECT_2; ini["PID_Ylf"][key] = payload_formatted_str; }
        else if (pv_name_str == "best_PidOffsetYhf") { pid_sel = BEST_PID_SELECT_3; ini["PID_Yhf"][key] = payload_formatted_str; }
        else                                         return -1;
        setOffsetSingle(pid_sel, *(double*)payload);
        file.write(ini);
        PDEBUG(DEBUG_RET_DATA, "pv: %s, data: %lf\n", pvName, *(double*)payload);
        return 0;
    }

    //=========================================================================
    else if ( sscanf(pvName, "best_PidK%1s%3s", kappa_sel, pos_sel) == 2 ) {
        std::string K_sel_str = std::string(kappa_sel);
        int K_sel;
        if      (K_sel_str == "p") { K_sel = 4;  key = "Kp"; }
        else if (K_sel_str == "i") { K_sel = 12; key = "Ki"; }
        else if (K_sel_str == "d") { K_sel = 20; key = "Kd"; }
        else                       return -1;
        std::string pos_sel_str = std::string(pos_sel);
        enum best_pid_select pid_sel;
        std::string payload_str = std::to_string(*(double *)payload);
        std::string payload_formatted_str = payload_str.substr(0, payload_str.find(".") + 6);
        if      (pos_sel_str == "Xlf") { pid_sel = BEST_PID_SELECT_0; section = "PID_Xlf";}
        else if (pos_sel_str == "Xhf") { pid_sel = BEST_PID_SELECT_1; section = "PID_Xhf";}
        else if (pos_sel_str == "Ylf") { pid_sel = BEST_PID_SELECT_2; section = "PID_Ylf";}
        else if (pos_sel_str == "Yhf") { pid_sel = BEST_PID_SELECT_3; section = "PID_Yhf";}
        else                           return -1;
        setPIDparamSingle(K_sel, pid_sel, (int)pid_par_sel_K, *(double*)payload);
        ini[section][key] = payload_formatted_str;
        file.write(ini);
        PDEBUG(DEBUG_RET_DATA, "pv: %s, data: %lf\n", pvName, *(double*)payload);
        return 0;
    }

    //=========================================================================
    else if ( sscanf(pvName, "best_PidO%3s%3s", kappa_sel, pos_sel) == 2 ) {
        std::string K_sel_str = std::string(kappa_sel);
        int K_sel;
        if      (K_sel_str == "min") {  K_sel = 4; key = "Omin"; }
        else if (K_sel_str == "max") {  K_sel = 12; key = "Omax"; }
        else if (K_sel_str == "gai") {  K_sel = 20; key = "Ogain"; }
        else                           return -1;
        std::string payload_str = std::to_string(*(double *)payload);
        std::string payload_formatted_str = payload_str.substr(0, payload_str.find(".") + 6);
        std::string pos_sel_str = std::string(pos_sel);
        enum best_pid_select pid_sel;
        if      (pos_sel_str == "Xlf") { pid_sel = BEST_PID_SELECT_0; section = "PID_Xlf"; }
        else if (pos_sel_str == "Xhf") { pid_sel = BEST_PID_SELECT_1; section = "PID_Xhf"; }
        else if (pos_sel_str == "Ylf") { pid_sel = BEST_PID_SELECT_2; section = "PID_Ylf"; }
        else if (pos_sel_str == "Yhf") { pid_sel = BEST_PID_SELECT_3; section = "PID_Yhf"; }
        else                           return -1;    
        setPIDparamSingle(K_sel, pid_sel, (int)pid_par_sel_out, *(double*)payload);
        ini[section][key] = payload_formatted_str;
        file.write(ini);
        PDEBUG(DEBUG_RET_DATA, "pv: %s, data: %lf\n", pvName, *(double*)payload);
        return 0;
    }

    //=========================================================================
    else if ( sscanf(pvName, "best_PidImax%3s", pos_sel) == 1 ) {
        key = "Imax";
        std::string payload_str = std::to_string(*(double *)payload);
        std::string payload_formatted_str = payload_str.substr(0, payload_str.find(".") + 6);
        int K_sel = 12;
        std::string pos_sel_str = std::string(pos_sel);
        enum best_pid_select pid_sel;
        if      (pos_sel_str == "Xlf") {   pid_sel = BEST_PID_SELECT_0; section = "PID_Xlf"; }
        else if (pos_sel_str == "Xhf") {   pid_sel = BEST_PID_SELECT_1; section = "PID_Xhf"; }
        else if (pos_sel_str == "Ylf") {   pid_sel = BEST_PID_SELECT_2; section = "PID_Ylf"; }
        else if (pos_sel_str == "Yhf") {   pid_sel = BEST_PID_SELECT_3; section = "PID_Yhf"; }
        else                              return -1;      
        setPIDparamSingle(K_sel, pid_sel, (int)pid_par_sel_lim, *(double*)payload);
        ini[section][key] = payload_formatted_str;
        file.write(ini);
        //PDEBUG(DEBUG_SET_DATA, "pv: %s, setPIDparamSingle(%d, %d, %d, %lf)\n",pvName, K_sel, (int)pid_sel, (int)pid_par_sel_lim, *(double*)payload);
        return 0;
    }

    //=========================================================================
    else if ( sscanf(pvName, "best_Pidemin%3s", pos_sel) == 1) {
        int K_sel = 4;
        std::string key = "emin";
        std::string payload_str = std::to_string(*(double *)payload);
        std::string payload_formatted_str = payload_str.substr(0, payload_str.find(".") + 6);
        std::string pos_sel_str = std::string(pos_sel);
        enum best_pid_select pid_sel;
        if      (pos_sel_str == "Xlf") { pid_sel = BEST_PID_SELECT_0; section = "PID_Xlf"; }
        else if (pos_sel_str == "Xhf") { pid_sel = BEST_PID_SELECT_1; section = "PID_Xhf"; }
        else if (pos_sel_str == "Ylf") { pid_sel = BEST_PID_SELECT_2; section = "PID_Ylf"; }
        else if (pos_sel_str == "Yhf") { pid_sel = BEST_PID_SELECT_3; section = "PID_Yhf"; }
        else                           return -1;
        setPIDparamSingle(K_sel, pid_sel, (int)pid_par_sel_lim, *(double*)payload);
        ini[section][key] = payload_formatted_str;
        file.write(ini);
        //PDEBUG(DEBUG_SET_DATA, "pv: %s, setPIDparamSingle(%d, %d, %d, %lf)\n", pvName, K_sel, (int)pid_sel, (int)pid_par_sel_lim, *(double*)payload);
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
        return 0;
    }

    //=========================================================================
    else if (pv_name_str == "best_PreDACoutmux") {
        /*
        int fd = open(FILE_MBOX, O_RDWR | O_SYNC);
        if(fd < 0) {
            printf("Error opening FILE_MBOX\n");
            return -1; }
        struct mail comm;
        comm.cmd = CMD_SIG_DEMUX << 8 | CMD_WRITE;
        memset(comm.payload, 0, 28);
        comm.payload[0] = *(short int*)payload;
        // emit io control
        int ret = ioctl(fd, IOCTL_MAIL_COMM, &comm);
        if (ret < 0){
            printf("ioctl(IOCTL_MAIL_COMM) failed\n");
            return -1; }
        close(fd);
        return 0;
        */
        setOutputMux(*(short int*)payload);
        return 0;
    }

    //=========================================================================
    else if ( sscanf(pvName, "best_PreDACoutCh%d", &predac_ch) == 1 ) {
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
    else if ( sscanf(pvName, "best_Enaroc%3s", pos_sel) == 1 ) {
        bool rocX, rocY, rocI0, rocX2, rocY2, rocI02;
        getROCenable(&rocX, &rocY, &rocI0, &rocX2, &rocY2, &rocI02);
        std::string pos_sel_str = std::string(pos_sel);
        if      (pos_sel_str == "X")  { rocX = (int)*(double*)payload;   ini["ROCandROI"] ["RocXenable"]    = rocX   ? "true" : "false"; }
        else if (pos_sel_str == "Y")  { rocY = (int)*(double*)payload;   ini["ROCandROI"] ["RocYenable"]    = rocY   ? "true" : "false"; }
        else if (pos_sel_str == "I")  { rocI0 = (int)*(double*)payload;  ini["ROCandROI"] ["BeamOffEnable"] = rocI0  ? "true" : "false"; }
        else if (pos_sel_str == "X2") { rocX2 = (int)*(double*)payload;  ini["ROCandROI2"]["RocXenable"]    = rocX2  ? "true" : "false"; }
        else if (pos_sel_str == "Y2") { rocY2 = (int)*(double*)payload;  ini["ROCandROI2"]["RocYenable"]    = rocY2  ? "true" : "false"; }
        else if (pos_sel_str == "I2") { rocI02 = (int)*(double*)payload; ini["ROCandROI2"]["BeamOffEnable"] = rocI02 ? "true" : "false"; }
        else                          return -1;
        setROCenable(rocX, rocY, rocI0, rocX2, rocY2, rocI02);
        file.write(ini);
        PDEBUG(DEBUG_RET_DATA, "pv: %s, data: %lf\n", pvName, *(double*)payload);
        return 0;
    }

    //=========================================================================
    else if ( pv_name_str == "best_PidConfigOUT" ) {
        int conf = (int)*(double *)payload;
        switch (conf) {
            case 1234: setPIDoutSel((uint8_t)0,(uint8_t)1,(uint8_t)2,(uint8_t)3);
                ini["Beamline"].set({ {"DAC_posXlf","0"},{"DAC_posXhf","1"},{"DAC_posYlf","2"},{"DAC_posYhf","3"} });break;
            case 1243: setPIDoutSel((uint8_t)0,(uint8_t)1,(uint8_t)3,(uint8_t)2);
                ini["Beamline"].set({ {"DAC_posXlf","0"},{"DAC_posXhf","1"},{"DAC_posYlf","3"},{"DAC_posYhf","2"} });break;
            case 1324: setPIDoutSel((uint8_t)0,(uint8_t)2,(uint8_t)1,(uint8_t)3);
                ini["Beamline"].set({ {"DAC_posXlf","0"},{"DAC_posXhf","2"},{"DAC_posYlf","1"},{"DAC_posYhf","3"} });break;
            case 1342: setPIDoutSel((uint8_t)0,(uint8_t)2,(uint8_t)3,(uint8_t)1);
                ini["Beamline"].set({ {"DAC_posXlf","0"},{"DAC_posXhf","2"},{"DAC_posYlf","3"},{"DAC_posYhf","1"} });break;
            case 1423: setPIDoutSel((uint8_t)0,(uint8_t)3,(uint8_t)1,(uint8_t)2);
                ini["Beamline"].set({ {"DAC_posXlf","0"},{"DAC_posXhf","3"},{"DAC_posYlf","1"},{"DAC_posYhf","2"} });break;
            case 1432: setPIDoutSel((uint8_t)0,(uint8_t)3,(uint8_t)2,(uint8_t)1);
                ini["Beamline"].set({ {"DAC_posXlf","0"},{"DAC_posXhf","3"},{"DAC_posYlf","2"},{"DAC_posYhf","1"} });break;
            case 2134: setPIDoutSel((uint8_t)1,(uint8_t)0,(uint8_t)2,(uint8_t)3);
                ini["Beamline"].set({ {"DAC_posXlf","1"},{"DAC_posXhf","0"},{"DAC_posYlf","2"},{"DAC_posYhf","3"} });break;;
            case 2143: setPIDoutSel((uint8_t)1,(uint8_t)0,(uint8_t)3,(uint8_t)2);
                ini["Beamline"].set({ {"DAC_posXlf","1"},{"DAC_posXhf","0"},{"DAC_posYlf","3"},{"DAC_posYhf","2"} });break;
            case 2314: setPIDoutSel((uint8_t)1,(uint8_t)2,(uint8_t)0,(uint8_t)3);
                ini["Beamline"].set({ {"DAC_posXlf","1"},{"DAC_posXhf","2"},{"DAC_posYlf","0"},{"DAC_posYhf","3"} });break;
            case 2341: setPIDoutSel((uint8_t)1,(uint8_t)2,(uint8_t)3,(uint8_t)0);
                ini["Beamline"].set({ {"DAC_posXlf","1"},{"DAC_posXhf","2"},{"DAC_posYlf","3"},{"DAC_posYhf","0"} });break;
            case 2413: setPIDoutSel((uint8_t)1,(uint8_t)3,(uint8_t)0,(uint8_t)2);
                ini["Beamline"].set({ {"DAC_posXlf","1"},{"DAC_posXhf","3"},{"DAC_posYlf","0"},{"DAC_posYhf","2"} });break;
            case 2431: setPIDoutSel((uint8_t)1,(uint8_t)3,(uint8_t)2,(uint8_t)0);
                ini["Beamline"].set({ {"DAC_posXlf","1"},{"DAC_posXhf","3"},{"DAC_posYlf","2"},{"DAC_posYhf","0"} });break;
            case 3124: setPIDoutSel((uint8_t)2,(uint8_t)0,(uint8_t)1,(uint8_t)3);
                ini["Beamline"].set({ {"DAC_posXlf","2"},{"DAC_posXhf","0"},{"DAC_posYlf","1"},{"DAC_posYhf","3"} });break;
            case 3142: setPIDoutSel((uint8_t)2,(uint8_t)0,(uint8_t)3,(uint8_t)1);
                ini["Beamline"].set({ {"DAC_posXlf","2"},{"DAC_posXhf","0"},{"DAC_posYlf","3"},{"DAC_posYhf","1"} });break;
            case 3214: setPIDoutSel((uint8_t)2,(uint8_t)1,(uint8_t)0,(uint8_t)3);
                ini["Beamline"].set({ {"DAC_posXlf","2"},{"DAC_posXhf","1"},{"DAC_posYlf","0"},{"DAC_posYhf","3"} });break;
            case 3241: setPIDoutSel((uint8_t)2,(uint8_t)1,(uint8_t)3,(uint8_t)0);
                ini["Beamline"].set({ {"DAC_posXlf","2"},{"DAC_posXhf","1"},{"DAC_posYlf","3"},{"DAC_posYhf","0"} });break;
            case 3412: setPIDoutSel((uint8_t)2,(uint8_t)3,(uint8_t)0,(uint8_t)1);
                ini["Beamline"].set({ {"DAC_posXlf","2"},{"DAC_posXhf","3"},{"DAC_posYlf","0"},{"DAC_posYhf","1"} });break;
            case 3421: setPIDoutSel((uint8_t)2,(uint8_t)3,(uint8_t)1,(uint8_t)0);
                ini["Beamline"].set({ {"DAC_posXlf","2"},{"DAC_posXhf","3"},{"DAC_posYlf","1"},{"DAC_posYhf","0"} });break;
            case 4123: setPIDoutSel((uint8_t)3,(uint8_t)0,(uint8_t)1,(uint8_t)2);
                ini["Beamline"].set({ {"DAC_posXlf","3"},{"DAC_posXhf","0"},{"DAC_posYlf","1"},{"DAC_posYhf","2"} });break;
            case 4132: setPIDoutSel((uint8_t)3,(uint8_t)0,(uint8_t)2,(uint8_t)1);
                ini["Beamline"].set({ {"DAC_posXlf","3"},{"DAC_posXhf","0"},{"DAC_posYlf","2"},{"DAC_posYhf","1"} });break;
            case 4213: setPIDoutSel((uint8_t)3,(uint8_t)1,(uint8_t)0,(uint8_t)2);
                ini["Beamline"].set({ {"DAC_posXlf","3"},{"DAC_posXhf","1"},{"DAC_posYlf","0"},{"DAC_posYhf","2"} });break;
            case 4231: setPIDoutSel((uint8_t)3,(uint8_t)1,(uint8_t)2,(uint8_t)0);
                ini["Beamline"].set({ {"DAC_posXlf","3"},{"DAC_posXhf","1"},{"DAC_posYlf","2"},{"DAC_posYhf","0"} });break;
            case 4312: setPIDoutSel((uint8_t)3,(uint8_t)2,(uint8_t)0,(uint8_t)1);
                ini["Beamline"].set({ {"DAC_posXlf","3"},{"DAC_posXhf","2"},{"DAC_posYlf","0"},{"DAC_posYhf","1"} });break;
            case 4321: setPIDoutSel((uint8_t)3,(uint8_t)2,(uint8_t)1,(uint8_t)0);
                ini["Beamline"].set({ {"DAC_posXlf","3"},{"DAC_posXhf","2"},{"DAC_posYlf","1"},{"DAC_posYhf","0"} });break;
            default: printf("%s", "error"); return -1; break;
        }
        file.write(ini);
        PDEBUG(DEBUG_RET_DATA, "pv: %s, data: %lf\n", pvName, *(double*)payload);
        return 0;
    }

    //=========================================================================
    else if ( sscanf(pvName, "best_BPM%dscaling%s", &ch_sel, pos_sel) == 2) {
        double scaleX, scaleY;
        getBPMscale(ch_sel, &scaleX, &scaleY);
        std::string pos_sel_str = std::string(pos_sel);
        if      (pos_sel_str == "X") { scaleX = *(double*)payload; key = "Scale_X"; }
        else if (pos_sel_str == "Y") { scaleY = *(double*)payload; key = "Scale_Y"; }
        else                         return -1;
        setBPMscale(scaleX, scaleY, ch_sel);
        if      (ch_sel == 0) section = "BPM1";
        else if (ch_sel == 1) section = "BPM2";
        else                  return -1;
        std::string payload_str = std::to_string(*(double *)payload);
        std::string payload_formatted_str = payload_str.substr(0, payload_str.find(".") + 6);
        ini[section][key] = payload_formatted_str;
        file.write(ini);
        PDEBUG(DEBUG_RET_DATA, "pv: %s, data: %lf\n", pvName, *(double*)payload);
        return 0;
    }

    //=========================================================================
    else if ( pv_name_str == "best_PidFilterLPX" || pv_name_str == "best_PidFilterLPY" ) {
        section = "PositionFilters";
        char filt, pos;
        if      (pv_name_str == "best_PidFilterLPX") { filt = 0; pos = 0; key = "XlfFilter";}
        else if (pv_name_str == "best_PidFilterLPY") { filt = 0; pos = 1; key = "YlfFilter";}
        else                            return -1;
        value = *(unsigned short*)payload;
        setFiltControl(filt, pos, value);
        std::string payload_str = std::to_string(value);
        ini[section][key] = payload_str;
        file.write(ini);
        PDEBUG(DEBUG_RET_DATA, "pv: %s, data: %lf\n", pvName, *(double*)payload);
        return 0;
    }

    //=========================================================================
    else if ( sscanf(pvName, "best_PidFilterCoef%1sP%1s%1s%d", filt_sel, pos_sel, param_sel, &coef_sel) == 4) {
        std::string filt_sel_str = std::string(filt_sel);
        char filt;
        if      (filt_sel_str == "L") filt = 0;
        else if (filt_sel_str == "B") filt = 1;
        else                          return -1;
        std::string pos_sel_str = std::string(pos_sel);
        char pos;
        if      (pos_sel_str == "X") pos = 0;
        else if (pos_sel_str == "Y") pos = 1;
        else                         return -1;
        std::string param_sel_str = std::string(param_sel);
        char param;
        if      (param_sel_str == "A") param = 1;
        else if (param_sel_str == "B") param = 0;
        else                           return -1;
        char coef;
        if      (coef_sel == 0) coef = 0;
        else if (coef_sel == 1) coef = 1;
        else if (coef_sel == 2) coef = 2;
        else if (coef_sel == 3) coef = 3;
        else if (coef_sel == 4) coef = 4;
        else                    return -1;
        setFiltCoef(filt, pos, param, coef, *(double *)payload);
        PDEBUG(DEBUG_RET_DATA, "pv: %s, data: %lf\n", pvName, *(double*)payload);
        return 0;
    }

    //=========================================================================
    else if ( sscanf(pvName, "best_Pidroc%1d", &ch_sel) == 1) {
        std::string payload_str = std::to_string(*(double *)payload);
        std::string payload_formatted_str = payload_str.substr(0, payload_str.find("."));
        double roiX, roiY, roiI0min, roiI0max, roc;
        getROCandROI_sel(ch_sel, &roiX, &roiY, &roiI0min, &roiI0max, &roc);
        double scaleX, scaleY;
        getBPMscale(ch_sel, &scaleX, &scaleY);
        char roc_selX, roc_selY;
        if      (ch_sel == 0) { roc_selX = 0x0; roc_selY= 0x1; section = "ROCandROI"; }
        else if (ch_sel == 1) { roc_selX = 0x3; roc_selY= 0x4; section = "ROCandROI2"; }
        else                  return -1;
        setROClimits(roc_selX, 0, *(double *)payload / 100 * roiX / scaleX);
        setROClimits(roc_selY, 0, *(double *)payload / 100 * roiY / scaleY);
        ini[section]["Roc"] = payload_formatted_str;
        file.write(ini);
        PDEBUG(DEBUG_RET_DATA, "pv: %s, data: %lf\n", pvName, *(double*)payload);
        return 0;
    }

    //=========================================================================
    else if ( sscanf(pvName, "best_PidrocI%1d", &ch_sel) == 1) {
        char roc_sel;
        std::string payload_str = std::to_string(*(double *)payload);
        std::string payload_formatted_str = payload_str.substr(0, payload_str.find("."));
        if      (ch_sel == 0) { roc_sel = 0x2; section = "ROCandROI"; }
        else if (ch_sel == 1) {roc_sel = 0x5; section = "ROCandROI2"; }
        else                  return -1;
        setROClimits(roc_sel, 0, *(double *)payload * 0.000001);
        ini[section]["BeamOffTh"] = payload_formatted_str;
        file.write(ini);
        PDEBUG(DEBUG_RET_DATA, "pv: %s, data: %lf\n", pvName, *(double*)payload);
        return 0;
    }

    //=========================================================================
    else if ( sscanf(pvName, "best_PidFreq%3s", pos_sel) == 1 ) {
        key = "Freq";
        double freq = floor(10000.0 / *(double *)payload);
        std::string freq_str = std::to_string(*(double *)payload);
        std::string freq_formatted_str = freq_str.substr(0, freq_str.find("."));
        std::string pos_sel_str = std::string(pos_sel);
        enum best_pid_select pid_sel;
        if      (pos_sel_str == "Xlf") { pid_sel = BEST_PID_SELECT_0; ini["PID_Xlf"][key] = freq_formatted_str; }
        else if (pos_sel_str == "Xhf") { pid_sel = BEST_PID_SELECT_1; ini["PID_Xhf"][key] = freq_formatted_str; }
        else if (pos_sel_str == "Ylf") { pid_sel = BEST_PID_SELECT_2; ini["PID_Ylf"][key] = freq_formatted_str; }
        else if (pos_sel_str == "Yhf") { pid_sel = BEST_PID_SELECT_3; ini["PID_Yhf"][key] = freq_formatted_str; }
        else                           return -1;
        file.write(ini);
        setWindAvg(pid_sel, freq);
        PDEBUG(DEBUG_RET_DATA, "pv: %s, data: %lf\n", pvName, *(double*)payload);
        return 0;
    }

    //=========================================================================
    else if ( sscanf(pvName, "best_BPMorient%d", &ch_sel) == 1 ) {
        key = "Orient";
        std::string payload_str = std::to_string(*(bool*)payload);
        uint8_t sel;
        if      (ch_sel)      { sel = 0x1; ini["BPM2"][key] = payload_str; }
        else if (ch_sel == 0) { sel = 0x0; ini["BPM1"][key] = payload_str; }
        else                  return -1;
        uint8_t orient;
        *(bool*)payload ? orient = 1 : orient = 0;
        setBPMorient(sel, orient);
        file.write(ini);
        PDEBUG(DEBUG_RET_DATA, "pv: %s, data: %lf\n", pvName, *(double*)payload);
        return 0;
    }

    //=========================================================================
    else if ( sscanf(pvName, "best_Pidroi%1d%1s", &ch_sel, pos_sel) == 2 ) {
        std::string payload_str = std::to_string(*(double *)payload);
        std::string payload_formatted_str = payload_str.substr(0, payload_str.find(".") + 6);
        std::string pos_sel_str = std::string(pos_sel);
        if      (ch_sel == 0) section = "ROCandROI";
        else if (ch_sel == 1) section = "ROCandROI2";
        else                  return -1;
        if      (pos_sel_str == "X") key = "RoiX";
        else if (pos_sel_str == "Y") key = "RoiY";
        else                         return -1;
        ini[section][key] = payload_formatted_str;
        file.write(ini);
        PDEBUG(DEBUG_RET_DATA, "pv: %s, data: %lf\n", pvName, *(double*)payload);
        return 0;
    }
    
    //=========================================================================
    else if ( sscanf(pvName, "best_BPMcrossbar%1d", &ch_sel) == 1 ) {
        uint8_t sel;
        if      (ch_sel == 0) { sel = 0; section = "BPM1"; }
        else if (ch_sel == 1) { sel = 1; section = "BPM2"; }
        else                  return -1;
        int conf = (int) *(double *)payload;
        switch (conf) {
            case 1234: setCrossBar(sel, 0, 1, 2, 3);
                ini[section].set({ {"Top","0"},{"Right","1"},{"Bottom","2"},{"Left","3"} });break;
            case 1243: setCrossBar(sel, 0, 1, 3, 2);
                ini[section].set({ {"Top","0"},{"Right","1"},{"Bottom","3"},{"Left","2"} });break;
            case 1324: setCrossBar(sel, 0, 2, 1, 3);
                ini[section].set({ {"Top","0"},{"Right","2"},{"Bottom","1"},{"Left","3"} });break;
            case 1342: setCrossBar(sel, 0, 2, 3, 1);
                ini[section].set({ {"Top","0"},{"Right","2"},{"Bottom","3"},{"Left","1"} });break;
            case 1423: setCrossBar(sel, 0, 3, 1, 2);
                ini[section].set({ {"Top","0"},{"Right","3"},{"Bottom","1"},{"Left","2"} });break;
            case 1432: setCrossBar(sel, 0, 3, 2, 1);
                ini[section].set({ {"Top","0"},{"Right","3"},{"Bottom","2"},{"Left","1"} });break;
            case 2134: setCrossBar(sel, 1, 0, 2, 3);
                ini[section].set({ {"Top","1"},{"Right","0"},{"Bottom","2"},{"Left","3"} });break;;
            case 2143: setCrossBar(sel, 1, 0, 3, 2);
                ini[section].set({ {"Top","1"},{"Right","0"},{"Bottom","3"},{"Left","2"} });break;
            case 2314: setCrossBar(sel, 1, 2, 0, 3);
                ini[section].set({ {"Top","1"},{"Right","2"},{"Bottom","0"},{"Left","3"} });break;
            case 2341: setCrossBar(sel, 1, 2, 3, 0);
                ini[section].set({ {"Top","1"},{"Right","2"},{"Bottom","3"},{"Left","0"} });break;
            case 2413: setCrossBar(sel, 1, 3, 0, 2);
                ini[section].set({ {"Top","1"},{"Right","3"},{"Bottom","0"},{"Left","2"} });break;
            case 2431: setCrossBar(sel, 1, 3, 2, 0);
                ini[section].set({ {"Top","1"},{"Right","3"},{"Bottom","2"},{"Left","0"} });break;
            case 3124: setCrossBar(sel, 2, 0, 1, 3);
                ini[section].set({ {"Top","2"},{"Right","0"},{"Bottom","1"},{"Left","3"} });break;
            case 3142: setCrossBar(sel, 2, 0, 3, 1);
                ini[section].set({ {"Top","2"},{"Right","0"},{"Bottom","3"},{"Left","1"} });break;
            case 3214: setCrossBar(sel, 2, 1, 0, 3);
                ini[section].set({ {"Top","2"},{"Right","1"},{"Bottom","0"},{"Left","3"} });break;
            case 3241: setCrossBar(sel, 2, 1, 3, 0);
                ini[section].set({ {"Top","2"},{"Right","1"},{"Bottom","3"},{"Left","0"} });break;
            case 3412: setCrossBar(sel, 2, 3, 0, 1);
                ini[section].set({ {"Top","2"},{"Right","3"},{"Bottom","0"},{"Left","1"} });break;
            case 3421: setCrossBar(sel, 2, 3, 1, 0);
                ini[section].set({ {"Top","2"},{"Right","3"},{"Bottom","1"},{"Left","0"} });break;
            case 4123: setCrossBar(sel, 3, 0, 1, 2);
                ini[section].set({ {"Top","3"},{"Right","0"},{"Bottom","1"},{"Left","2"} });break;
            case 4132: setCrossBar(sel, 3, 0, 2, 1);
                ini[section].set({ {"Top","3"},{"Right","0"},{"Bottom","2"},{"Left","1"} });break;
            case 4213: setCrossBar(sel, 3, 1, 0, 2);
                ini[section].set({ {"Top","3"},{"Right","1"},{"Bottom","0"},{"Left","2"} });break;
            case 4231: setCrossBar(sel, 3, 1, 2, 0);
                ini[section].set({ {"Top","3"},{"Right","1"},{"Bottom","2"},{"Left","0"} });break;
            case 4312: setCrossBar(sel, 3, 2, 0, 1);
                ini[section].set({ {"Top","3"},{"Right","2"},{"Bottom","0"},{"Left","1"} });break;
            case 4321: setCrossBar(sel, 3, 2, 1, 0);
                ini[section].set({ {"Top","3"},{"Right","2"},{"Bottom","1"},{"Left","0"} });break;
            default: printf("%s", "error"); return -1; break;
        }
        file.write(ini);
        PDEBUG(DEBUG_RET_DATA, "pv: %s, data: %lf\n", pvName, *(double*)payload);
        return 0;
    }

    //=========================================================================
    else if ( sscanf(pvName, "best_BPMoffset%1d%1s", &ch_sel, pos_sel) == 2) {
        std::string payload_str = std::to_string(*(double *)payload);
        std::string payload_formatted_str = payload_str.substr(0, payload_str.find(".") + 6);
        std::string pos_sel_str = std::string(pos_sel);
        if      (ch_sel == 0) section = "BPM1";
        else if (ch_sel == 1) section = "BPM2"; 
        double offsetX, offsetY;
        getBPMoffset(&offsetX, &offsetY, ch_sel);
        if      (pos_sel_str == "X") { offsetX = *(double *)payload; key = "Offset_X"; }
        else if (pos_sel_str == "Y") { offsetY = *(double *)payload; key = "Offset_Y"; }
        else                         return -1;
        setBPMoffset(offsetX, offsetY, ch_sel);
        ini[section][key] = payload_formatted_str;
        file.write(ini);
        PDEBUG(DEBUG_RET_DATA, "pv: %s, data: %lf\n", pvName, *(double*)payload);
        return 0;
    }

    //=========================================================================
    else if ( pv_name_str == "best_BPMselector" ) {
        value = *(short int *)payload;
        setBPMselector(value & 0x1, value >> 1 & 0x1, value >> 2 & 0x1, value >> 3 & 0x1);
        PDEBUG(DEBUG_RET_DATA, "pv: %s, data: %lf\n", pvName, *(double*)payload);
        return 0;
    }

    //=========================================================================
    else if ( sscanf(pvName, "best_FilterFreq%5s", pos_sel) == 1) {
        section = "PositionFilters";
        std::string pos_sel_str = std::string(pos_sel);
        if      (pos_sel_str == "LPXlf") key = "XlfFcutLow"; 
        else if (pos_sel_str == "LPYlf") key = "YlfFcutLow";
        else if (pos_sel_str == "LPXhf") key = "XhfFcutLow";
        else if (pos_sel_str == "LPYhf") key = "YhfFcutLow";
        else if (pos_sel_str == "HPXhf") key = "XhfFcutHigh";
        else if (pos_sel_str == "HPYhf") key = "YhfFcutHigh";
        else                             return -1;
        std::string payload_str = std::to_string(*(double *)payload);
        std::string payload_formatted_str = payload_str.substr(0, payload_str.find(".") + 6);
        ini[section][key] = payload_formatted_str;
        file.write(ini);
        PDEBUG(DEBUG_RET_DATA, "pv: %s, data: %lf\n", pvName, *(double*)payload);
        return 0;
    }

    //=========================================================================
    else if ( sscanf(pvName, "best_PIDFiltercommit%3s", pos_sel) == 1) {
        section = "PositionFilters";
        vec a = {1, 2, 3, 4, 5};
    	vec b = {1, 2, 3, 4, 5};
        double low, high;
        std::string pos_sel_str = std::string(pos_sel);
        char filt, pos;
        if      (pos_sel_str == "Xlf") {
            filt = 0;
            pos = 0;
            key = "XlfFcutLow";
            low = std::stod(ini[section][key]);
            butterworthLow(4, low, 0.0001, b, a);
        }
        else if (pos_sel_str == "Ylf") {
            filt = 0;
            pos = 1;
            key = "YlfFcutLow";
            low = std::stod(ini[section][key]);
            butterworthLow(4, low, 0.0001, b, a);
        }
        else if (pos_sel_str == "Xhf") {
            filt = 1;
            pos = 0;
            key = "XhfFcutLow";
            low = std::stod(ini[section][key]);
            key = "XhfFcutHigh";
            high = std::stod(ini[section][key]);
            key = "XhfFilter";
            butterworthBand(4, low, high, 0.0001, b, a);
        }
        else if (pos_sel_str == "Yhf") {
            filt = 1;
            pos = 1;
            key = "YhfFcutLow";
            low = std::stod(ini[section][key]);
            key = "YhfFcutHigh";
            high = std::stod(ini[section][key]);
            key = "YhfFilter";
            butterworthBand(4, low, high, 0.0001, b, a);
        }
        else    return -1;
        for (int i = 0; i < 5; i++) {
            if (i > 0)
                setFiltCoef(filt, pos, 1, char(i), -a[i]);
            setFiltCoef(filt, pos, 0, char(i), b[i]);
        }
        *(unsigned short *) payload = (unsigned short) commitFiltCoef(filt, pos);
        PDEBUG(DEBUG_RET_DATA, "pv: %s, data: %lf\n", pvName, *(double*)payload);
        return 0;
    }

    else {
        PDEBUG(DEBUG_ERROR, " %s: unknown name\n", pvName);
    }
    return -1;
}
