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
CXXFLAGS += -I/usr/include -std=c++11
best_LDFLAGS += -L/lib -L/usr/lib
best_SYS_LIBS += bibest fftw3 armadillo lapack blas

# Extra flags
# USR_CXXFLAGS += -std=c++11
USR_CXXFLAGS += -DARMA_DONT_USE_WRAPPER

# best_registerRecordDeviceDriver.cpp derives from best.dbd
best_SRCS += best_registerRecordDeviceDriver.cpp
best_SRCS += butterworth.cpp
best_SRCS += best_io.cpp
best_SRCS += devAiBest.cpp
best_SRCS += devAoBest.cpp
best_SRCS += devBoBest.cpp
best_SRCS += devMbbiBest.cpp
best_SRCS += devMbboBest.cpp
best_SRCS += devWaveformBest.cpp
best_SRCS += devStringoutBest.cpp
best_SRCS += devBiBest.cpp
best_SRCS += devStringinBest.cpp

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
