
#ifndef BEST_IO_H_
#define BEST_IO_H_

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdlib.h>

#include "pcie_driver/BEST_PCIe.h"
#include "pcie_mailbox/mailbox_comm_defs.h"

#include "best.h"
#include "aiRecord.h"
#include "aoRecord.h"
#include "waveformRecord.h"
#include "mbbiRecord.h"
#include "boRecord.h"
#include "mbboRecord.h"
#include "stringoutRecord.h"

extern uint32_t debug;

#define DEBUG_ERROR         (0x1)
#define DEBUG_WARN          (0x2)
#define DEBUG_LOW_FUNC      (0x4)
#define DEBUG_REC_INIT      (0x8)
#define DEBUG_REC_PROC      (0x10)
#define DEBUG_RET_DATA      (0x20)
#define DEBUG_SET_DATA      (0x40)
#define DEBUG_ACC_SEC       (0x80)

#define PDEBUG(level, fmt, args...) { \
                if (debug & (level)) \
                         printf(" [ %02x ] " fmt, (level),  ## args);  }

#define FILE_MBOX       "/dev/best_mailbox"
#define FILE_PREDAC     "/dev/best_predac0"
#define FILE_DISP       "/dev/best_dma_displ"

#define DISP_NR_CH      32
#define DISP_POSX       0
#define DISP_POSY       1
#define DISP_INT0       2

typedef enum retType {
    INT,
    DOUBLE,
    USHORT,
    BOOL,
    STRING
} retType_t;

int stripEpicsIocName(char *pvName, aoRecord *pao);
int stripEpicsIocName(char *pvName, waveformRecord *pwf);
int stripEpicsIocName(char *pvName, aiRecord *pai);
int stripEpicsIocName(char *pvName, mbbiRecord *pmbbi);
int stripEpicsIocName(char *pvName, boRecord *pbo);
int stripEpicsIocName(char *pvName, mbboRecord *pmbbo);
int stripEpicsIocName(char *pvName, stringoutRecord *psor);

#ifdef __cplusplus
extern "C" {
#endif
    int initBest(char *pvName, retType_t type, void *rec);
    int readBest(char *pvName, retType_t type, void* payload, int count);
    int writeBest(char *pvName, retType_t type, void* payload);
#ifdef __cplusplus
}
#endif

#endif /* BEST_IO_H_ */
