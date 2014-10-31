/*
 * PiezoMotor PMD101 axis
 *
 * */
// vim: tabstop=2 shiftwidth=2
#ifndef _H_PMD_AXIS
#define _H_PMD_AXIS

#include "pmd101.h"

// vim: tabstop=2 shiftwidth=2
///// Axis
class pmdAxis : public asynMotorAxis
{
public:
  pmdAxis(pmdController *pC, int axis);
  asynStatus move(double position, int relative, double min_velocity, double max_velocity, double acceleration);
  asynStatus moveVelocity(double min_velocity, double max_velocity, double acceleration);
  asynStatus home(double min_velocity, double max_velocity, double acceleration, int forwards);
  asynStatus stop(double acceleration);
  asynStatus poll(bool *moving);

  asynStatus setEncoderPosition(double position);

  asynStatus setClosedLoop(bool closed);
  //asynStatus setPosition(double position);
  asynStatus setUIntDigitalParam(int index, epicsUInt32 value);

  /* And these are specific to this class: */
  asynStatus setEncoderResolution(double res);
  asynStatus queryStatus();

  int position_to_counts(double position);
  double counts_to_position(int counts);

  bool checkMoving();

  inline bool isFlagSet(unsigned int flag) { return (flags_ & flag) == flag; }
  inline void setFlag(unsigned int flag)   { flags_ |= flag; }
  inline void clearFlag(unsigned int flag) { flags_ &= ~flag; }
  inline void setFlag(unsigned int flag, bool set) {
    if (set)
      setFlag(flag);
    else
      clearFlag(flag);
  }

protected:

private:
  void motionFinished();

  friend class pmdController;
  pmdController *pc_;    /**< Pointer to the asynMotorController to which this axis belongs.
                          *   Abbreviated because it is used very frequently */
  double encoder_pos_;   /** < Cached copy of the encoder position */
  unsigned int flags_;   /** < Cached copy of the current flags */

  bool home_counts_;     // Maximum number of counts to use in a homing move

  bool has_encoder_;
  bool moving_;
  bool errored_;
  double limit_adc_[2]; 
  bool limits_[2];
  int axis_num_;         // according to asyn (0-based)
  int status_;
  //int param_num_;      // last parameter number (according to allmotion_params) to be queried
};

#endif
