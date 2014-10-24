TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = pmdTest
# pmdTest.dbd will be created and installed
DBD += pmdTest.dbd

# pmdTest.dbd will be made up from these files:
pmdTest_DBD += base.dbd

# Include dbd files from all support applications:
pmdTest_DBD += stream.dbd
pmdTest_DBD += pmd101.dbd
pmdTest_DBD += motorSupport.dbd calcSupport.dbd 
pmdTest_DBD += drvAsynIPPort.dbd asyn.dbd

# Add all the support libraries needed by this IOC
# Note: pmd101 must come before motor, due to copied asynMotorController/etc
pmdTest_LIBS += pmd101 stream asyn calc motor

# pmdTest_registerRecordDeviceDriver.cpp derives from pmdTest.dbd
pmdTest_SRCS += pmdTest_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
pmdTest_SRCS_DEFAULT += pmdTestMain.cpp
pmdTest_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#pmdTest_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
pmdTest_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

