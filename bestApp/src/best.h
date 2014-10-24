
#ifndef BEST_H_
#define BEST_H_

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include "pcie_driver/BEST_PCIe.h"


#define FILE_PREDAC     "/dev/best_predac0"


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
    int writeBest(char *pvName, retType_t type, void* payload);
#ifdef __cplusplus
}
#endif

#endif /* BEST_H_ */
