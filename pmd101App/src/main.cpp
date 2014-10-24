#include "pmd101.h"
// vim: tabstop=2 shiftwidth=2

static ELLLIST pmdList;
static int pmdListInitialized = 0;

bool addToList(const char *portName, pmdController *drv) {
    if (!pmdListInitialized) {
        pmdListInitialized = 1;
        ellInit(&pmdList);
    } else if (findByPortName(portName) != NULL) {
        fprintf(stderr, "ERROR: Re-using portName=%s\n", portName);
        return false;
    }

    pmdNode *pNode = (pmdNode*)calloc(1, sizeof(pmdNode));
    pNode->portName = epicsStrDup(portName);
    pNode->pController = drv;
    ellAdd(&pmdList, (ELLNODE*)pNode);
    return true;
}

pmdController* findByPortName(const char *portName) {
    pmdNode *pNode;
    static const char *functionName = "findByPortName";

    // Find this 
    if (!pmdListInitialized) {
        printf("%s:%s: ERROR, pmd list not initialized\n",
            driverName, functionName);
        return NULL;
    }

    pNode = (pmdNode*)ellFirst(&pmdList);
    while(pNode) {
        if (!strcmp(pNode->portName, portName)) {
            return pNode->pController;
        }
        pNode = (pmdNode*)ellNext((ELLNODE*)pNode);
    }

    printf("%s: pmd on port %s not found\n",
        driverName, portName);
    return NULL;
}


///// pmdCreateController
//
/** Creates a new pmdController object.
  * Configuration command, called directly or from iocsh
  * \param[in] portName          The name of the asyn port that will be created for this driver
  * \param[in] asynPortName      The name of the drvAsynIPPPort that was created previously to connect to the pmd controller 
  * \param[in] movingPollPeriod  The time in ms between polls when any axis is moving
  * \param[in] idlePollPeriod    The time in ms between polls when no axis is moving 
  */
extern "C" int pmdCreateController(const char *portName, const char *asynPortName,
                                   int movingPollPeriod, int idlePollPeriod)
{
  new pmdController(portName, asynPortName, movingPollPeriod, idlePollPeriod);
  return(asynSuccess);
}

/*
 Code for iocsh registration */
static const iocshArg pmdCreateControllerArg0 = {"Asyn port name", iocshArgString};
static const iocshArg pmdCreateControllerArg1 = {"PMD101 port name", iocshArgString};
static const iocshArg pmdCreateControllerArg2 = {"Moving poll period (ms)", iocshArgInt};
static const iocshArg pmdCreateControllerArg3 = {"Idle poll period (ms)", iocshArgInt};
static const iocshArg * const pmdCreateControllerArgs[] = {&pmdCreateControllerArg0,
                                                           &pmdCreateControllerArg1,
                                                           &pmdCreateControllerArg2,
                                                           &pmdCreateControllerArg3
                                                           };

static const iocshFuncDef pmdCreateControllerDef = {"pmdCreateController", 4, pmdCreateControllerArgs};
static void pmdCreateControllerCallFunc(const iocshArgBuf *args)
{
  if (!args[0].sval)
    return;
  pmdCreateController(args[0].sval, args[1].sval, args[2].ival, args[3].ival);
}

#if 0
///// pmdConfigureAxis
/** Configures an pmdAxis object.
  * Configuration command, called directly or from iocsh
  * \param[in] portName          The name of the controller's asyn port
  * \param[in] axis              The axis
  * \param[in] movingPollPeriod  The time in ms between polls when any axis is moving
  * \param[in] idlePollPeriod    The time in ms between polls when no axis is moving 
  */
extern "C" int pmdConfigureAxis(const char *portName, int axisNum,
                                      int movingPollPeriod, int idlePollPeriod)
{

  pmdController *controller;
  if ((controller = findByPortName(portName)) == NULL) {
    return 1;
  }
  
  pmdAxis *axis = controller->getAxis(axisNum);
  if (!axis) {
    printf("Bad axis number #%d (axis count = %d)", axisNum, controller->getAxisCount());
    return 1;
  }

  axis->configure();
}

/** Code for iocsh registration */
static const iocshArg pmdConfigureAxisArg0 = {"Port name", iocshArgString};
static const iocshArg pmdConfigureAxisArg1 = {"PMD101 port name", iocshArgString};
static const iocshArg pmdConfigureAxisArg2 = {"RS485 address", iocshArgInt};
static const iocshArg pmdConfigureAxisArg3 = {"Number of axes", iocshArgInt};
static const iocshArg pmdConfigureAxisArg4 = {"Moving poll period (ms)", iocshArgInt};
static const iocshArg pmdConfigureAxisArg5 = {"Idle poll period (ms)", iocshArgInt};
static const iocshArg * const pmdConfigureAxisArgs[] = {&pmdConfigureAxisArg0,
                                                                 &pmdConfigureAxisArg1,
                                                                 &pmdConfigureAxisArg2,
                                                                 &pmdConfigureAxisArg3,
                                                                 &pmdConfigureAxisArg4,
                                                                 &pmdConfigureAxisArg5
                                                                };

static const iocshFuncDef pmdConfigureAxisDef = {"pmdConfigureAxis", 5, pmdConfigureAxisArgs};
static void pmdConfigureAxisCallFunc(const iocshArgBuf *args)
{
  pmdConfigureAxis(args[0].sval, args[1].sval, args[2].ival, args[3].ival, args[4].ival, args[5].ival);
}
#endif


/***********************************************************************/
static void pmd101MotorRegister(void)
{
  iocshRegister(&pmdCreateControllerDef, pmdCreateControllerCallFunc);
  //iocshRegister(&pmdConfigureAxisDef, pmdConfigureAxisCallFunc);
}

extern "C" {
epicsExportRegistrar(pmd101MotorRegister);
}
