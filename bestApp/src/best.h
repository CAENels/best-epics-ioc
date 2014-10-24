
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

#define FILE_MBOX       "/dev/best_mailbox"
#define FILE_PREDAC     "/dev/best_predac0"
#define FILE_DISP       "/dev/best_dma_displ"

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
