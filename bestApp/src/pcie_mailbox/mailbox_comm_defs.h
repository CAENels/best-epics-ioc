////////////////////////////////////////////////////
/// Mailbox Communication definitions
/// \author Jan Marjanovic
/// \date March 2013
///
/// Meta project to keep headers in sync

#ifndef MAILBOX_COMM_DEFS_H
#define MAILBOX_COMM_DEFS_H

#include <stdint.h>

//////////////////////////////////////////////////////
// list of commands
//-----------------------------------------------

#define CMD_GET_VERSION		0x000001
#define CMD_GET_HW_TIME		0x000002
#define CMD_GET_PID_VER		0x000003
#define CMD_BPM_SCALE		0x000004

#define CMD_SIG_WINDAVG		0x010002
#define CMD_SIG_CROSSBAR	0x010003
#define CMD_SIG_DIFFOSUM	0x010004
#define CMD_SIG_PID_EN		0x010005
//#define CMD_SIG_SHUTLIM		0x010006
#define CMD_SIG_DEMUX		0x010007
#define CMD_SIG_STATS		0x010008
#define CMD_SIG_DMA_PRSC	0x010009
#define CMD_SIG_POS_OFFS	0x01000A
#define CMD_SIG_SEL_BPM		0x01000B

enum sig_demux{
	sig_demux_direct = 0,
	sig_demux_pid    = 1
};


//-----------------------------------------------
#define CMD_PID_PARAM		0x020001
#define CMD_PID_SETPT		0x020002
#define CMD_PID_CONF		0x020003
//#define CMD_PID_P_UPDT		0x020004
#define CMD_PID_OFFSET		0x020005
#define CMD_PID_ROC			0x020006

enum roc_select {
	roc_sel_X = 0,
	roc_sel_Y = 1,
	roc_sel_I0 = 2,
	roc_sel_X2 = 3,
	roc_sel_Y2 = 4,
	roc_sel_I02 = 5
};

#define CMD_PID_STATUS		0x020007
#define CMD_PID_DL_PARMS	0x020008
#define CMD_PID_RESET		0x020009
#define CMD_PID_OUT_SEL		0x02000A
#define CMD_PID_ROC_EN		0x02000B

enum pid_select {pid_sel_X = 0,
                 pid_sel_Y = 1,
                 pid_sel_I0 = 2};

enum pid_par_sel {pid_par_sel_K = 0,
                  pid_par_sel_lim = 0x10,
                  pid_par_sel_out = 0x20};

enum pid_conf	{ 	pid_conf_X 		= 0,
					pid_conf_XY		= 1,
					pid_conf_XI0	= 2,
					pid_conf_Y		= 3,
					pid_conf_YI0	= 4,
					pid_conf_XYI0	= 5,
					pid_conf_I0		= 6
};

typedef struct __attribute__((__packed__)) {
	double min;
	double max;
} ROC_Type;


typedef enum {
	PID_Status_stoppedByUser 	= 0,
	PID_Status_stoppedByROC  	= 1,
	PID_Status_paused	    	= 2,
	PID_Status_running       	= 3
} PID_Status_Type;


//-----------------------------------------------
#define CMD_SFP_GETINFO		0x030002


//-----------------------------------------------
#define CMD_ADC_HV_EN		0x040001
#define CMD_ADC_HV_SETPT	0x040002
#define CMD_ADC_RNG			0x040003

//-----------------------------------------------
#define CMD_FLASH_RW		0x050001
#define CMD_START_RECOV		0x050005

//////////////////////////////////////////////////////////

// if we are not on linux ( AKA we are on Nios )
#ifndef __linux__
#define	uint16_t	alt_u16
#define	uint32_t	alt_u32
#endif


struct __attribute__((__packed__)) sfp_info{
	uint16_t dev_ident;
	uint32_t timestamp;
	uint32_t fw_ver;
	uint32_t sn;
};

//////////////////////////////////////////////////////
// command controls and responses
#define CMD_WRITE			0x0
#define CMD_READ			0x1
#define CMD_ACK				0x2
#define CMD_NAK				0xF



#endif // MAILBOX_COMM_DEFS_H
