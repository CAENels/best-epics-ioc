#!/bin/bash

UBUNTU_VER=$(lsb_release -sr)

if [ $UBUNTU_VER = "14.04" ]
then
    # this is for bestepicsioc on ubuntu 14.04
    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/opt/CAENels/BEST/best_epics_ioc/lib
else
    # this is for bestepicsioc on ubuntu 20.04
    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/opt/EPICS/lib/linux-x86_64/
fi

eval /opt/CAENels/BEST/best_epics_ioc/iocBoot/iocbest/st.cmd
