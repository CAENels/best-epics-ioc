
#ifndef BEST_H_
#define BEST_H_

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdlib.h>

#include "pcie_driver/BEST_PCIe.h"
#include "pcie_mailbox/mailbox_comm_defs.h"

#include "best_c_interface.h"


extern uint32_t debug;

#define DEBUG_ERROR         (0x1)
#define DEBUG_WARN          (0x2)
#define DEBUG_LOW_FUNC      (0x4)
#define DEBUG_REC_INIT      (0x8)
#define DEBUG_REC_PROC      (0x10)
#define DEBUG_RET_DATA      (0x20)
#define DEBUG_SET_DATA      (0x40)

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



#ifdef __cplusplus
extern "C" {
#endif
    int stripEpicsIocName(char *pvName, const char *name);
    int readBest(char *pvName, retType_t type, void* payload, int count);
    int writeBest(char *pvName, retType_t type, void* payload);
#ifdef __cplusplus
}
#endif

#endif /* BEST_H_ */
