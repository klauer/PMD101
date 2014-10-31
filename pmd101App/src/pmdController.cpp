/*
 * PiezoMotor PMD101 controller
 * */

// vim: tabstop=2 shiftwidth=2
#include "pmd101.h"

/** Creates a new pmdController object.
  * \param[in] portName          The name of the asyn port that will be created for this driver
  * \param[in] asynPort          The name of the drvAsynIPPPort that was created previously to connect to the controller
  * \param[in] movingPollPeriod  The time between polls when any axis is moving
  * \param[in] idlePollPeriod    The time between polls when no axis is moving
  */
pmdController::pmdController(const char *portName, const char *asynPortName,
                             double movingPollPeriod, double idlePollPeriod)
  :  asynMotorController(portName, 1, NUM_PMD_PARAMS,
                         asynInt32Mask | asynFloat64Mask | asynUInt32DigitalMask | asynOctetMask,
                         asynInt32Mask | asynFloat64Mask | asynUInt32DigitalMask | asynOctetMask,
                         ASYN_CANBLOCK | ASYN_MULTIDEVICE,
                         1, // autoconnect
                         0, 0)  // Default priority and stack size
{
  idlePollPeriod_ = idlePollPeriod;
  movingPollPeriod_ = movingPollPeriod;

  if (!addToList(portName, this)) {
    printf("%s:%s: Init failed", driverName, portName);
    return;
  }

  // Write-only
  createParam(PMD_PSTR_KILL_ALL          ,    asynParamInt32,  &param_kill_all_);

#if 0
  const char *pname;
  for (i=0; i < 100; i++) {
    getParamName(i, &pname);
    printf("%d %s\n", i, pname);
  }
#endif
  
  timeout_ = 1.0;

  setIntegerParam(motorStatusHasEncoder_, 1);

  /* Connect to the pmd101 controller */
  asynStatus status = pasynOctetSyncIO->connect(asynPortName, 0, &pasynUser_, NULL);
  if (status) {
    asynPrint(pasynUser_, ASYN_TRACE_ERROR,
      "%s:%s: cannot connect to pmd101 controller\n",
      driverName, __func__);
  }

  status = pasynOctetSyncIO->setInputEos(pasynUser_, PMD_INPUT_EOS, 1);
  if (status) {
    asynPrint(pasynUser_, ASYN_TRACE_ERROR|ASYN_TRACE_FLOW,
      "%s: Unable to set input EOS on %s: %s\n",
      __func__, asynPortName, pasynUser_->errorMessage);
  }

  status = pasynOctetSyncIO->setOutputEos(pasynUser_, PMD_OUTPUT_EOS, 1);
  if (status) {
    asynPrint(pasynUser_, ASYN_TRACE_ERROR|ASYN_TRACE_FLOW,
      "%s: Unable to set output EOS on %s: %s\n",
      __func__, asynPortName, pasynUser_->errorMessage);
  }

  // Create the axis
  axis_ = new pmdAxis(this, 0);
  
  startPoller(movingPollPeriod/1000., idlePollPeriod/1000., 2);
  
}

asynStatus pmdController::poll()
{
  return asynSuccess;
}

/** Called when asyn clients call pasynFloat64->write().
  * Extracts the function and axis number from pasynUser.
  * Sets the value in the parameter library.
  * Calls any registered callbacks for this pasynUser->reason and address.  
  *
  * \param[in] pasynUser asynUser structure that encodes the reason and address.
  * \param[in] value     Value to write. */
asynStatus pmdController::writeFloat64(asynUser *pasynUser, epicsFloat64 value)
{
  int function = pasynUser->reason;
  asynStatus status = asynSuccess;
  pmdAxis *pAxis = getAxis(pasynUser);
  const char *paramName = "(unset)";

  if (!pAxis) 
    return asynError;

  /* Fetch the parameter string name for possible use in debugging */
  getParamName(function, &paramName);

  /* Set the parameter and readback in the parameter library.  This may be overwritten when we read back the
   * status at the end, but that's OK */
  status = setDoubleParam(pAxis->axisNo_, function, value);

#if DEBUG
  printf("Param %s value %g\n", paramName, value);
#endif

  if (0) { // function == param_input_threshold_[0]) {
  } else {
    /* Call base class method */
    status = asynMotorController::writeFloat64(pasynUser, value);
  }
  
  /* Do callbacks so higher layers see any changes */
  callParamCallbacks(pAxis->axisNo_);
  if (status) 
    asynPrint(pasynUser, ASYN_TRACE_ERROR, 
        "%s:%s: error, status=%d function=%s (%d), value=%f\n", 
        driverName, __func__, status, paramName, function, value);
  else    
    asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, 
        "%s:%s: function=%s (%d), value=%f\n", 
        driverName, __func__, paramName, function, value);
  return status;
}

/** Called when asyn clients call pasynUInt32Digital->write().
  * Extracts the function and axis number from pasynUser.
  * Sets the value in the parameter library.
  *
  * Calls any registered callbacks for this pasynUser->reason and address.  
  * \param[in] pasynUser asynUser structure that encodes the reason and address.
  * \param[in] value     Value to write. */
asynStatus pmdController::writeUInt32Digital(asynUser *pasynUser, epicsUInt32 value, epicsUInt32 mask)
{
  int function = pasynUser->reason;
  asynStatus status = asynSuccess;
  pmdAxis *pAxis = getAxis(pasynUser);
  const char *paramName = "(unset)";

  /* Fetch the parameter string name for possible use in debugging */
  getParamName(function, &paramName);

  /* Set the parameter and readback in the parameter library.  This may be overwritten when we read back the
   * status at the end, but that's OK */
  status = setUIntDigitalParam(pAxis->axisNo_, function, value, mask);

#if DEBUG
  printf("%s:%s: mask=%x function=%s (%d), value=%d\n", 
        driverName, __func__, mask, paramName, function, value);
#endif

  if (0) {
  } else {
    /* Call base class method */
    status = asynMotorController::writeUInt32Digital(pasynUser, value, mask);
  }
  
  /* Do callbacks so higher layers see any changes */
  callParamCallbacks(pAxis->axisNo_);
  if (status) 
    asynPrint(pasynUser, ASYN_TRACE_ERROR, 
        "%s:%s: error, status=%d function=%s (%d), value=%d\n", 
        driverName, __func__, status, paramName, function, value);
  else    
    asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, 
        "%s:%s: function=%s (%d), value=%d\n", 
        driverName, __func__, paramName, function, value);
  return status;
}

/** Called when asyn clients call pasynOctet->write().
  * Extracts the function and axis number from pasynUser.
  * Sets the value in the parameter library.
  *
  * \param[in] pasynUser asynUser structure that encodes the reason and address.
  * \param[in] value     Value to write. */
asynStatus pmdController::writeOctet(asynUser *pasynUser, const char *value, 
                                     size_t nChars, size_t *nActual)
{
  int addr=0;
  int function = pasynUser->reason;
  asynStatus status = asynSuccess;
  const char *paramName = "(unset)";

  /* Fetch the parameter string name for possible use in debugging */
  getParamName(function, &paramName);

#if DEBUG
  printf("%s:%s: function=%s (%d), value=%s\n", 
        driverName, __func__, paramName, function, value);
#endif

  status = getAddress(pasynUser, &addr);
  if (status != asynSuccess) 
    return status;

  /* Set the parameter in the parameter library. */
  status = (asynStatus)setStringParam(addr, function, (char *)value);
  *nActual = nChars;

  if (0) {
  } else {
    return asynPortDriver::writeOctet(pasynUser, value, nChars, nActual);
  }

  if (status) 
      epicsSnprintf(pasynUser->errorMessage, pasynUser->errorMessageSize, 
                "%s:%s: status=%d, function=%d, value=%s", 
                driverName, __func__, status, function, value);
  else        
      asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, 
            "%s:%s: function=%d, value=%s\n", 
            driverName, __func__, function, value);

  return status;
}

/** Called when asyn clients call pasynInt32->write().
  * Extracts the function and axis number from pasynUser.
  * Sets the value in the parameter library.
  *
  * \param[in] pasynUser asynUser structure that encodes the reason and address.
  * \param[in] value     Value to write. */
asynStatus pmdController::writeInt32(asynUser *pasynUser, epicsInt32 value)
{
  int function = pasynUser->reason;
  asynStatus status = asynSuccess;
  pmdAxis *pAxis = getAxis(pasynUser);
  const char *paramName = "(unset)";

  /* Fetch the parameter string name for possible use in debugging */
  getParamName(function, &paramName);

#if DEBUG
  printf("%s:%s: function=%s (%d), value=%d\n", 
        driverName, __func__, paramName, function, value);
#endif

  /* Set the parameter and readback in the parameter library.  This may be overwritten when we read back the
   * status at the end, but that's OK */
  status = setIntegerParam(pAxis->axisNo_, function, value);
  if (function == param_kill_all_) {
  } else {
    /* Call base class method */
    status = asynMotorController::writeInt32(pasynUser, value);
  }
  
  /* Do callbacks so higher layers see any changes */
  callParamCallbacks(pAxis->axisNo_);
  if (status) 
    asynPrint(pasynUser, ASYN_TRACE_ERROR, 
        "%s:%s: error, status=%d function=%s (%d), value=%d\n", 
        driverName, __func__, status, paramName, function, value);
  else    
    asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, 
        "%s:%s: function=%s (%d), value=%d\n", 
        driverName, __func__, paramName, function, value);
  return status;
}

asynStatus pmdController::writeReadInt(const char *command, int &value, char delim) {
  size_t nwrite;
  asynStatus status;
  int eomReason;
  size_t nread;

  char input[PMD_LEN];

  lock();

  status = pasynOctetSyncIO->writeRead(pasynUser_,
                                       command, strlen(command),
                                       input, PMD_LEN - 1,
                                       timeout_, &nwrite, &nread, &eomReason);
  
  unlock();
  
  if (status == asynSuccess) {
    input[nread] = 0;

    char *ptr = &input[0];

    if (delim != '\0') {
      do {
        ptr++;
      } while (*ptr != delim && *ptr != 0);
    }

    if (*ptr != 0) {
      value = atoi(ptr);
      status = asynSuccess;
    } else {
      value = 0;
      status = asynError;
    }
  }

  return status;
}

asynStatus pmdController::write(const char *fmt, va_list argptr) {
  size_t nwrite;
  asynStatus status;
  const char *functionName="write";
  const int buf_size = PMD_LEN;
  char buf[buf_size];

  vsnprintf(buf, buf_size, fmt, argptr);

  lock();

#if DEBUG
  fprintf(stderr, "%s:%s: %s\n", driverName, functionName, buf);
#endif

  asynPrint(pasynUser_, ASYN_TRACE_FLOW,
    "%s:%s: %s\n",
    driverName, functionName, buf);

  status = pasynOctetSyncIO->write(pasynUser_,
                                   buf, strlen(buf),
                                   timeout_, &nwrite);
  unlock();

  return status;
}

asynStatus pmdController::write(const char *fmt, ...) {
  va_list argptr;
  va_start(argptr,fmt);
  asynStatus ret=write(fmt, argptr);
  va_end(argptr);
  return ret;
}


asynStatus pmdController::writeRead(char *input, size_t* nread, const char *fmt, va_list argptr) {

  size_t nwrite;
  asynStatus status;
  int eomReason;
  const char *functionName="writeRead";
  const int buf_size = PMD_LEN;
  char buf[buf_size];

  vsnprintf(buf, buf_size, fmt, argptr);

  strncpy(outString_, buf, buf_size);
  lock();
#if DEBUG
  fprintf(stderr, "%s:%s: write: %s\n",
    driverName, functionName, buf);
#endif

  asynPrint(pasynUser_, ASYN_TRACEIO_DRIVER,
    "%s:%s: Write: %s\n",
    driverName, functionName, buf);

  status = pasynOctetSyncIO->writeRead(pasynUser_,
                                       buf, strlen(buf),
                                       input, PMD_LEN,
                                       timeout_, &nwrite, nread, &eomReason);

#if DEBUG
  fprintf(stderr, "%s:%s: Read (%db): %s\n",
    driverName, functionName, *nread, input);
#endif
  asynPrint(pasynUser_, ASYN_TRACEIO_DRIVER,
    "%s:%s: Read (%db): %s\n",
    driverName, functionName, *nread, input);

  unlock();

  return status;
}

asynStatus pmdController::writeRead(char *input, size_t* nread, const char *fmt, ...) {
  va_list argptr;
  va_start(argptr,fmt);
  asynStatus ret=writeRead(input, nread, fmt, argptr);
  va_end(argptr);
  return ret;
}
