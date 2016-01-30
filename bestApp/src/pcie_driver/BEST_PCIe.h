//////////////////////////////////////////////////////////////////////////
///
/// \file   BEST_PCIe.h
/// \author Jan Marjanovic (j.marjanovic@caenels.com)
/// \date	February 2014
/// \brief  Headers for BEST PCIe driver
///
///
#ifndef BEST_PCIE_H_
#define BEST_PCIE_H_

#include <asm/ioctl.h>

#ifndef __KERNEL__
#include <stdint.h>
#endif


/// single display buffer size
#define DMA_DISP_BUF_SIZE	(128*1024)

/// number of display buffers
#define DMA_DISP_BUF_NR		30



//////////////////////////////////////////////////////////////////////////
// IOCTL commands

/// chosen at random
#define	BEST_IOCTL_NUMBER		'B'



//------------------------------------------------
/// TetrAMM data
struct __attribute__((__packed__)) tetramm_data {
	uint32_t	comm_resp_val1;
	uint32_t	comm_resp_val0;
	uint32_t	comm_resp;
	float		hvi;
	float		hvv;
	uint64_t	status;
	double		ch3;
	double		ch2;
	double		ch1;
	double		ch0;
	uint32_t 	seq_nr;
};

//------------------------------------------------
/// TetrAMM data small (aka fast)
struct __attribute__((__packed__)) tetramm_data_fast {
	double		ch3;
	double		ch2;
	double		ch1;
	double		ch0;
	uint32_t 	seq_nr;
};


//------------------------------------------------
/// DAC data
struct __attribute__((__packed__)) dac_data {
	double		ch0;
	double		ch1;
	double		ch2;
	double		ch3;
	uint32_t	control;

};

//------------------------------------------------
/// Mailbox data
struct __attribute__((__packed__)) mail {
	uint32_t	cmd;
	char		payload[28];
};

//------------------------------------------------
/// Mailbox stats
struct __attribute__((__packed__)) mail_stats_s {
	double filt_freq;
	struct __attribute__((__packed__)) {
		double mean;
		double stdev;
		double stdev_lim;
	} stat[6];
};


/// send command and receive response via Mailbox
#define IOCTL_MAIL_COMM		_IOWR(BEST_IOCTL_NUMBER, 10, struct mail *)

/// read statistics from registers
#define IOCTL_MAIL_STATS	_IOR(BEST_IOCTL_NUMBER, 11, struct mail_stats_s *)

/// write filter freq for limites std
#define IOCTL_WRITE_STD_FREQ	_IOW(BEST_IOCTL_NUMBER, 11, double*)

/// reads tetramm data
#define IOCTL_READ_TETRAMM	_IOR(BEST_IOCTL_NUMBER, 20, struct tetramm_data *)

/// reads tetramm data fast
#define IOCTL_READ_T_FAST	_IOR(BEST_IOCTL_NUMBER, 30, struct tetramm_data_fast *)

/// write DAC data
#define IOCTL_WRITE_DAC		_IOW(BEST_IOCTL_NUMBER, 40, struct dac_data *)

/// password for flash write and erase
#define IOCTL_FLASH_PASSWD	_IOW(BEST_IOCTL_NUMBER, 50, char*)

/// erases flash page 128kB
#define IOCTL_FLASH_ERASE	_IOW(BEST_IOCTL_NUMBER, 60, unsigned int)


/// this is used to unlock flash, it's not a real password but
/// an obstacle for people who don't know what are they doing
#define FLASH_PASSWORD		"careful!!"





#endif /* BEST_PCIE_H_ */
