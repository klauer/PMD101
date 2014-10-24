/*
 * PiezoMotor PMD101
 * */
// vim: tabstop=2 shiftwidth=2

#ifndef _PMD101_H
#define _PMD101_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cmath>
#include <iostream>
#include <memory>

#include <iocsh.h>
#include <epicsTypes.h>
#include <epicsTime.h>
#include <epicsThread.h>
#include <epicsString.h>
#include <epicsTimer.h>
#include <epicsMutex.h>
#include <epicsEvent.h>
#include <epicsExport.h>

#include <asynOctetSyncIO.h>
#include "asynMotorController.h"
#include "asynMotorAxis.h"

#ifndef DEBUG
// Set this to 0 to disable debug messages
#define DEBUG 0
#endif

#define PMD_LEN                         128

// Parameter string identifiers (matched in INP/OUT in records)
#define PMD_PSTR_KILL_ALL               "PMD_KILLALL"

// EOS terminators
#define PMD_INPUT_EOS                   "\r"
#define PMD_OUTPUT_EOS                  "\r"


#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef max
#define max(a, b) (((a) > (b)) ? (a) : (b))
#endif

static const char *driverName = "PMD101";
class pmdController;
class pmdAxis;
char *strnchr(const char* str, size_t len, char c);

#include "pmdAxis.h"
#include "pmdController.h"

/* Use the following structure and functions to manage multiple instances
 * of the driver */
typedef struct pmdNode {
    ELLNODE node;
    const char *portName;
    pmdController *pController;
} pmdNode;

bool addToList(const char *portName, pmdController *drv);
pmdController* findByPortName(const char *portName);

double adc_to_volts(int value);
unsigned int volts_to_adc(double volts);

#endif
