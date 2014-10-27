#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include "pcie_driver/BEST_PCIe.h"
#include "pcie_mailbox/mailbox_comm_defs.h"
#include <iostream>
#include "inih/cpp/INIReader.h"
#include <complex>
#include <fftw3.h>
#include <signal.h>


#define BEST_FILE_MAILBOX		"/dev/best_mailbox"
#define BEST_FILE_DISP_DMA		"/dev/best_dma_displ"
#define LOCK_FILE				"/var/lock/best_lock"
#define BEST_FILE_CONFIG_INI	"/opt/CAENels/BEST/config.ini"

#define FFT_LEN					(2048)

extern "C" {


//===========================================================================//
//                                                                           //
//                            Data functions                                 //
//                                                                           //
//===========================================================================//

/** @defgroup dataFunc Data functions
 *  Functions which read measurement and calulcation data from system
 *  @{
 */

///////////////////////////////////////////////////////////////////////////////
/// \brief Gets arrays of postions and intensity from display DMA
/// \param posX Pointer to array where position X will be written
/// \param posY Pointer to array where position Y will be written
/// \param I0 Pointer to array where Intensity will be written
/// \param length Length of the arrays
/// \return numbers of samples copies (should be equal to length)
int getPosArray(double* posX, double* posY, double* I0, unsigned int length);

///////////////////////////////////////////////////////////////////////////////
/// \brief Gets arrays of currents from display DMA
/// \param length Length of the arrays
/// \return numbers of samples copies (should be equal to length)
int getCurrentArray(double (*currents)[4], unsigned int length);

///////////////////////////////////////////////////////////////////////////////
/// \brief Calcualtes FFT
/// \param selector Selects source for FFT (
///         offset_adc0_ch0 = 24,
///         offset_pos0_posX = 0,
///         offset_pos0_posY = 1,
///         offset_pos0_I0 = 2,
///         offset_pos1_posX = 3,
///         offset_pos1_posY = 4,
///         offset_pos1_I0 = 5,
///         offset_dac0_ch0 = 16,
///         offset_dac0_ch1 = 17,
///         offset_dac0_ch2 = 18,
///         offset_dac0_ch3 = 19
/// \param outM magnitude output (size \ref FFT_LEN / 2)
/// \param outF frequency output (size \ref FFT_LEN / 2)
/// \param removeDC removes DC component
int getFFT(unsigned int selector, double *outM, double *outF, int removeDC);


///////////////////////////////////////////////////////////////////////////////
/// \brief Get position statistics
/// \param selector Selects which TetrAmm
int getPosStats(int selector, 	double* meanX,
								double* stdX,
								double* meanY,
								double* stdY,
								double* meanI0,
								double* stdI0	);


/** @} */ // end of dataFunc


//===========================================================================//
//                                                                           //
//                        Configuration functions                            //
//                                                                           //
//===========================================================================//

/** @defgroup configFunc Configuration functions
 *  Functions which configure system
 *  @{
 */

///////////////////////////////////////////////////////////////////////////////
/// \brief Gets SFP info
/// \param selector Selects which SFP port (A=0, B=1, ...)
/// \param id
/// \param timestamp
/// \param fw_ver
/// \param ser_nr
/// \return 0 on success, -1 on fail
int getSFPinfo(int selector, 	uint16_t *id, 
								uint32_t *timestamp, 
								uint32_t *fw_ver,
								uint32_t *ser_nr);
}


///////////////////////////////////////////////////////////////////////////////
/// \brief Gets BPM scaling parameters for position X and Y in um
/// \return 0 on success, -1 on fail
int getBPMscaling(double* scaleX, double* scaleY);


///////////////////////////////////////////////////////////////////////////////
/// \brief Gets ROC and ROI from config file
/// \return 0 on success, -1 on fail
int getROCandROI(  	double *roiX,
					double *roiY,
					double *roiI0min,
					double *roiI0max,
					double *roc );


///////////////////////////////////////////////////////////////////////////////
/// \brief Sets output offsets (output=this+PID value, updated even when
///        PID is not running)
/// \param offsetX
/// \param offsetY
/// \param offsetI0
/// \return 0 on success, negative on fail
/// \note Access level must be admin
int setOffset(double offsetX, double offsetY, double offsetI0);

///////////////////////////////////////////////////////////////////////////////
/// \brief Sets number of samples for window averaging
/// \param sel (0 = X, 1 = Y, 2 = I0)
/// \param nr_samp Number of samples
/// \return 0 on success, negative on fail
/// \note Access level must be admin
int setWindAvg(char sel, uint32_t nr_samp); 

///////////////////////////////////////////////////////////////////////////////
/// \brief Sets PID parameters
/// \param pid_sel (0 = X, 1 = Y, 2 = I0)
/// \param Kp See BEST User's Manual for detailed explanation
/// \param Ki See BEST User's Manual for detailed explanation
/// \param Kd See BEST User's Manual for detailed explanation
/// \param emin See BEST User's Manual for detailed explanation
/// \param Imax See BEST User's Manual for detailed explanation
/// \param Omin See BEST User's Manual for detailed explanation
/// \param Omax See BEST User's Manual for detailed explanation
/// \param Ogain See BEST User's Manual for detailed explanation
/// \return 0 on success, negative on fail
/// \note Access level must be admin
int setPIDparams( 	char pid_sel,
					double Kp,
                    double Ki,
                    double Kd,
                    double emin,
                    double Imax,
                    double Omin,
                    double Omax,
                    double Ogain);

///////////////////////////////////////////////////////////////////////////////
/// \brief Sets PID parameters
/// \param sel (0 = X, 1 = Y, 2 = I0)
/// \param min 
/// \param max
/// \return 0 on success, negative on fail
/// \note Access level must be admin
int setROC(char sel, double min, double max);

///////////////////////////////////////////////////////////////////////////////
/// \brief Sets PID output cross switch
/// \param posX DAC channel for posX
/// \param posY DAC channel for posY
/// \param I0 DAC channel for I0
/// \return 0 on success, negative on fail
/// \note Access level must be admin
int setPIDoutSel(uint8_t posX, uint8_t posY, uint8_t I0);

///////////////////////////////////////////////////////////////////////////////
/// \brief Sets input cross switch
/// \param sel Selects TetrAmm (in case of 2 TetrAmms)
/// \param ch0 
/// \param ch1
/// \param ch2
/// \param ch3
/// \return 0 on success, negative on fail
/// \note Access level must be admin
int setCrossBar(uint8_t sel, uint8_t ch0, uint8_t ch1, uint8_t ch2, uint8_t ch3);

///////////////////////////////////////////////////////////////////////////////
/// \brief Sets BPM orreintation for diffOverSum
/// \param sel Selects TetrAmm (in case of 2 TetrAmms)
/// \param is90deg 
/// \return 0 on success, negative on fail
/// \note Access level must be admin
int setBPMorient(uint8_t sel, uint8_t is90deg);

/** @} */ // end of configFunc



//===========================================================================//
//                                                                           //
//                        Access control functions                           //
//                                                                           //
//===========================================================================//

/** @defgroup loginFunc Access control functions
 *  Functions which allow to elevate user access level
 *  @{
 */

///////////////////////////////////////////////////////////////////////////////
/// \brief Acquires lock on /var/lock/best_lock
/// \param usr Username
/// \param pass Password
/// \return login level (0=cruise, 1=user, 2=admin)
int getLock(const char *usr, const char *pass);

///////////////////////////////////////////////////////////////////////////////
/// \brief Gets login level, which can be changed with call on getLock()
/// \return login level (0=cruise, 1=user, 2=admin)
int getLockStatus();

/** @} */ // end of loginFunc


//===========================================================================//
//                                                                           //
//                          PID control functions                            //
//                                                                           //
//===========================================================================//

/** @defgroup PIDfunc PID control functions
 *  Functions which control PID behaviour
 *  @{
 */

///////////////////////////////////////////////////////////////////////////////
/// \brief Gets PID status
/// \return stoppedByUser = 0, stoppedByROC = 1, paused = 2, running = 3
unsigned int getPIDstatus(void);

///////////////////////////////////////////////////////////////////////////////
/// \brief Enables or disables feedback
/// \return 0 on success, negative on fail (e.g. authentication)
int setFBenable(int enable);

///////////////////////////////////////////////////////////////////////////////
/// \brief Resets internal states of controller
/// \return 0 on success, negative on fail (e.g. authentication)
int setCtrlReset();

///////////////////////////////////////////////////////////////////////////////
/// \brief Sets PID controller configuration
/// \param conf PID conf (X = 0, XY = 1, XI0 = 2, Y = 3, YI0 = 4, XYI0 = 5, I0 = 6)
/// \return 0 on success, negative on fail (e.g. authentication)
int setPIDconf(char conf);

///////////////////////////////////////////////////////////////////////////////
/// \brief Sets PID controller configuration
/// \param conf Pointer to return PID conf
/// \return 0 on success, negative on fail
int getPIDconf(char* conf);

///////////////////////////////////////////////////////////////////////////////
/// \brief Sets new setpoint 
/// \param sel Selects which PID ( X = 0, Y = 1, I0 = 2 )
/// \param setpt Setpoint in um (takes into accout values in config.ini)
/// \return 0 on success, negative on fail (e.g. authentication)
int setSetpoint(char sel, double setpt);

///////////////////////////////////////////////////////////////////////////////
/// \brief Gets setpoint 
/// \param sel Selects which PID ( X = 0, Y = 1, I0 = 2 )
/// \param setpt Setpoint in um (takes into accout values in config.ini)
/// \return 0 on success, negative on fail
int getSetpoint(char sel, double* setpt);

/** @} */ // end of PIDfunc


