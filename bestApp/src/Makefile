TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = best
# best.dbd will be created and installed
DBD += best.dbd

# best.dbd will be made up from these files:
best_DBD += base.dbd
best_DBD += bestdev.dbd

# Include dbd files from all support applications:
#best_DBD += xxx.dbd

# Add all the support libraries needed by this IOC
#best_LIBS += xxx
best_LDFLAGS += -L/lib
best_SYS_LIBS += best_if fftw3

# best_registerRecordDeviceDriver.cpp derives from best.dbd
best_SRCS += best_registerRecordDeviceDriver.cpp
best_SRCS += best.cpp
best_SRCS += devAiBest.cpp
best_SRCS += devAoBest.cpp
best_SRCS += devMbbiBest.cpp
best_SRCS += devMbboBest.cpp
best_SRCS += devWaveformBest.cpp

# Build the main IOC entry point on workstation OSs.
best_SRCS_DEFAULT += bestMain.cpp
best_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#best_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
best_LIBS += $(EPICS_BASE_IOC_LIBS)


#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

