TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

LIBRARY_IOC += pmd101

# pmd101.dbd will be created and installed
DBD += pmd101.dbd

# Add all the support libraries needed by this IOC
pmd101_LIBS += asyn motor

pmd101_SRCS += main.cpp pmdController.cpp pmdAxis.cpp
pmd101_SRCS += asynMotorController.cpp asynMotorAxis.cpp

# Finally link to the EPICS Base libraries
pmd101_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

