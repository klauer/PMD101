TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = hxnPmd
# hxnPmd.dbd will be created and installed
DBD += hxnPmd.dbd

# hxnPmd.dbd will be made up from these files:
hxnPmd_DBD += base.dbd

# Include dbd files from all support applications:
hxnPmd_DBD += stream.dbd
hxnPmd_DBD += pmd101.dbd
hxnPmd_DBD += motorSupport.dbd calcSupport.dbd 
hxnPmd_DBD += drvAsynIPPort.dbd asyn.dbd
hxnPmd_DBD += system.dbd
hxnPmd_DBD += asSupport.dbd

hxnPmd_DBD += devIocStats.dbd
hxnPmd_DBD += caPutLog.dbd

# Add all the support libraries needed by this IOC
# Note: pmd101 must come before motor, due to copied asynMotorController/etc
hxnPmd_LIBS += pmd101 stream asyn calc motor autosave
hxnPmd_LIBS += devIocStats
hxnPmd_LIBS += caPutLog

# hxnPmd_registerRecordDeviceDriver.cpp derives from hxnPmd.dbd
hxnPmd_SRCS += hxnPmd_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
hxnPmd_SRCS_DEFAULT += hxnPmdMain.cpp
hxnPmd_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#hxnPmd_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
hxnPmd_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

