#!../../bin/linux-x86_64/hxnPmd
## Piezomotor PMD101 IOC startup script example

< envPaths

## Register all support components
dbLoadDatabase("$(TOP)/dbd/hxnPmd.dbd",0,0)
hxnPmd_registerRecordDeviceDriver(pdbbase) 

epicsEnvSet(STREAM_PROTOCOL_PATH, ".:$(PMD101)/pmd101App/Db")
epicsEnvSet(Sys     , "XF:03IDC-ES")
epicsEnvSet(DevX    , "{ZpTpa-Ax:X}")
epicsEnvSet(DevY    , "{ZpTpa-Ax:Y}")
epicsEnvSet(DevZ    , "{ZpTpa-Ax:Z}")
epicsEnvSet(PMD_IP  , "10.3.2.59")
# epicsEnvSet(PMD_PORTX, 4005)
# epicsEnvSet(PMD_PORTY, 4006)
# epicsEnvSet(PMD_PORTZ, 4007)

# -- for some reason it is on real com mode! (port numbers not normal moxa ones)
epicsEnvSet(PMD_PORTX, 954)  
epicsEnvSet(PMD_PORTY, 955)
epicsEnvSet(PMD_PORTZ, 956)

epicsEnvSet(ASYN_PX , "IPX")
epicsEnvSet(PMD_PX  , "PMDX")
epicsEnvSet(ASYN_PY , "IPY")
epicsEnvSet(PMD_PY  , "PMDY")
epicsEnvSet(ASYN_PZ , "IPZ")
epicsEnvSet(PMD_PZ  , "PMDZ")
epicsEnvSet(AS      , "IOC:PMD:AS:")

epicsEnvSet(EPICS_CA_AUTO_ADDR_LIST, "NO")
epicsEnvSet(EPICS_CA_ADDR_LIST, "10.3.0.255")

# drvAsynIPPortConfigure 'port name' 'host:port [protocol]' priority 'disable auto-connect' noProcessEos
drvAsynIPPortConfigure("$(ASYN_PX)", "$(PMD_IP):$(PMD_PORTX)", 0, 0, 0)
drvAsynIPPortConfigure("$(ASYN_PY)", "$(PMD_IP):$(PMD_PORTY)", 0, 0, 0)
drvAsynIPPortConfigure("$(ASYN_PZ)", "$(PMD_IP):$(PMD_PORTZ)", 0, 0, 0)

asynOctetSetOutputEos("$(ASYN_PX)", 0, "\r")
asynOctetSetInputEos("$(ASYN_PX)", 0, "\r")
asynOctetSetOutputEos("$(ASYN_PY)", 0, "\r")
asynOctetSetInputEos("$(ASYN_PY)", 0, "\r")
asynOctetSetOutputEos("$(ASYN_PZ)", 0, "\r")
asynOctetSetInputEos("$(ASYN_PZ)", 0, "\r")

# PMDCreateController(AllMotion port name, asyn port name, Moving poll period (ms), Idle poll period (ms))
pmdCreateController("$(PMD_PX)", "$(ASYN_PX)", 100, 250)
pmdCreateController("$(PMD_PY)", "$(ASYN_PY)", 100, 250)
pmdCreateController("$(PMD_PZ)", "$(ASYN_PZ)", 100, 250)

pmdConfigureAxis("$(PMD_PX)", 0, 0.005)
pmdConfigureAxis("$(PMD_PY)", 0, 0.005)
pmdConfigureAxis("$(PMD_PZ)", 0, 0.005)

dbLoadTemplate("motors.sub")
dbLoadRecords("$(PMD101)/db/pmd101.db", "Sys=$(Sys),Dev=$(DevX),PORT=$(ASYN_PX),DEBUG=1")
dbLoadRecords("$(PMD101)/db/pmd101.db", "Sys=$(Sys),Dev=$(DevY),PORT=$(ASYN_PY),DEBUG=1")
dbLoadRecords("$(PMD101)/db/pmd101.db", "Sys=$(Sys),Dev=$(DevZ),PORT=$(ASYN_PZ),DEBUG=1")

asynSetTraceMask("$(PMD_PX)", -1, 0x01)
asynSetTraceMask("$(ASYN_PX)", -1, 0x01)
asynSetTraceMask("$(PMD_PY)", -1, 0x01)
asynSetTraceMask("$(ASYN_PY)", -1, 0x01)
asynSetTraceMask("$(PMD_PZ)", -1, 0x01)
asynSetTraceMask("$(ASYN_PZ)", -1, 0x01)

< save_restore.cmd

# var streamDebug 1
iocInit()

create_monitor_set("info_settings.req", 30)
create_monitor_set("info_positions.req", 15)

dbpf("$(Sys)$(DevX)Cmd:TgtLimA-Out", -15000000)
dbpf("$(Sys)$(DevX)Cmd:TgtLimB-Out", 15000000)
dbpf("$(Sys)$(DevY)Cmd:TgtLimA-Out", -15000000)
dbpf("$(Sys)$(DevY)Cmd:TgtLimB-Out", 15000000)
dbpf("$(Sys)$(DevZ)Cmd:TgtLimA-Out", -15000000)
dbpf("$(Sys)$(DevZ)Cmd:TgtLimB-Out", 15000000)
