///////////////////////////////////////////////////////////////////////////////
//              ____      _      _____   _   _          _                    //
//             / ___|    / \    | ____| | \ | |   ___  | |  ___              //
//            | |       / _ \   |  _|   |  \| |  / _ \ | | / __|             //
//            | |___   / ___ \  | |___  | |\  | |  __/ | | \__ \             //
//             \____| /_/   \_\ |_____| |_| \_|  \___| |_| |___/             //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include <stddef.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include "epicsExit.h"
#include "epicsThread.h"
#include "iocsh.h"

#include <getopt.h>
#include "best.h"

extern uint32_t debug;

int main(int argc,char *argv[])
{
    int c;
    char stCmd[128];
    strcpy(stCmd, argv[1]);

	while(1){
	    static struct option long_options[] =
	    {
	      {"debug",  required_argument, 0, 'd'},
	      {0, 0, 0, 0}
	    };

        c = getopt_long(argc, argv, "d:", long_options, NULL);

        if(c == -1) break; // end of options

        switch (c) {
            case 'd': 
                if(sscanf(optarg, "%i", &debug) < 1){
                    puts("Debug argument expects number (e.g. -d 2)");
                    return -1;
                }
                printf("Enabling debug, level: 0x%x\n", debug);
                break;
        }

    }

    if(argc>=2) {    
        iocsh(stCmd);
        epicsThreadSleep(.2);
    }
    iocsh(NULL);
    epicsExit(0);
    return(0);
}
