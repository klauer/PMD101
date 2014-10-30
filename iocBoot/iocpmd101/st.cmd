#!../../bin/linux-x86/pmdTest
## Piezomotor PMD101 IOC startup script example

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
dbLoadRecords("$(PMD101)/db/pmd101.db", "Sys=$(Sys),Dev=$(Dev),PORT=$(ASYN_P),DEBUG=1")

asynSetTraceMask("$(PMD_P)", -1, 0x01)
asynSetTraceMask("$(ASYN_P)", -1, 0x01)

# asynSetTraceIOMask("$(PMD_P)", 0, 0xff)
# asynSetTraceIOMask("$(ASYN_P)", 0, 0xff)

< save_restore.cmd

# var streamDebug 1
iocInit()

create_monitor_set("info_settings.req", 30)
create_monitor_set("info_positions.req", 15)
