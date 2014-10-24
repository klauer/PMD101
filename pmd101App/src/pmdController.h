/*
 * PiezoMotor PMD101 controller
 * */

// vim: tabstop=2 shiftwidth=2
//
#ifndef _H_PMD_CONTROLLER
#define _H_PMD_CONTROLLER

#include "pmd101.h"

// vim: tabstop=2 shiftwidth=2
///// Controller
class pmdController : public asynMotorController {
public:
  pmdController(const char *portName, const char *asynPortName, 
                double movingPollPeriod, double idlePollPeriod);
  pmdAxis* getAxis(int axisNo) {
    return (pmdAxis*)asynMotorController::getAxis(axisNo);
  }

  /** Returns a pointer to an pmd101Axis object.
    * Returns NULL if the axis number encoded in pasynUser is invalid.
    * \param[in] pasynUser asynUser structure that encodes the axis index number. */
  pmdAxis* getAxis(asynUser *pasynUser)
  {
    return static_cast<pmdAxis*>(asynMotorController::getAxis(pasynUser));
  }

  virtual ~pmdController() {}

  asynStatus write(const char* fmt, ...);
  asynStatus write(const char* fmt, va_list);
  asynStatus writeRead(char* input, size_t* nread, const char* fmt, ...);
  asynStatus writeRead(char* input, size_t* nread, const char* fmt, va_list);

  asynStatus writeReadInt(const char *command, int &value, char delim=' ');

  int getAxisCount() { return numAxes_; }
  virtual asynStatus writeInt32(asynUser *pasynUser, epicsInt32 value);
  virtual asynStatus writeFloat64(asynUser *pasynUser, epicsFloat64 value);
  virtual asynStatus writeUInt32Digital(asynUser *pasynUser, epicsUInt32 value, epicsUInt32 mask);
  virtual asynStatus writeOctet(asynUser *pasynUser, const char *value, size_t maxChars,
                                size_t *nActual);

protected:
  virtual asynStatus poll();

  int param_kill_all_;
#define FIRST_PMD_PARAM param_kill_all_
#define LAST_PMD_PARAM param_kill_all_
#define NUM_PMD_PARAMS (&LAST_PMD_PARAM - &FIRST_PMD_PARAM + 1)
  
  float timeout_;

  asynUser *pasynUser_;

private:
  friend class pmdAxis;
  
  pmdAxis *axis_;
};

#endif
