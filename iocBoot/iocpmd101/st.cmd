#!../../bin/linux-x86/pmdTest

< envPaths

## Register all support components
dbLoadDatabase("$(TOP)/dbd/pmdTest.dbd",0,0)
pmdTest_registerRecordDeviceDriver(pdbbase) 

epicsEnvSet(STREAM_PROTOCOL_PATH, ".:$(PMD101)/pmd101App/Db")
epicsEnvSet(Sys     , "IOC:")
epicsEnvSet(Dev     , "PMD:")
epicsEnvSet(PMD_IP  , "10.0.0.11")
epicsEnvSet(PMD_PORT, 4005)
epicsEnvSet(ASYN_P  , "IP1")
epicsEnvSet(PMD_P   , "PMD1")
epicsEnvSet(AS      , "IOC:PMD:AS:")

# drvAsynIPPortConfigure 'port name' 'host:port [protocol]' priority 'disable auto-connect' noProcessEos
drvAsynIPPortConfigure("$(ASYN_P)", "$(PMD_IP):$(PMD_PORT)", 0, 0, 0)

asynOctetSetOutputEos("$(ASYN_P)", 0, "\r")
asynOctetSetInputEos("$(ASYN_P)", 0, "\r")

# PMDCreateController(AllMotion port name, asyn port name, Moving poll period (ms), Idle poll period (ms))
pmdCreateController("$(PMD_P)", "$(ASYN_P)", 100, 250)

dbLoadTemplate("motors.sub")
dbLoadRecords("$(PMD101)/db/pmd101.db", "Sys=$(Sys),Dev=$(Dev),PORT=$(ASYN_P),DEBUG=0")

asynSetTraceMask("$(PMD_P)", -1, 0x01)
asynSetTraceMask("$(ASYN_P)", -1, 0x01)

#asynSetTraceMask("$(PMD_P)", -1, 0x010)
#asynSetTraceMask("$(ASYN_P)", -1, 0x010)
# < save_restore.cmd

# var streamDebug 1
iocInit()

