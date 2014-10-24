/*
 * PiezoMotor PMD101 axis
 *
 * */
// vim: tabstop=2 shiftwidth=2
#include "pmd101.h"

/** Creates a new pmdAxis object.
  * \param[in] controller         The pmd101 controller
  * \param[in] axis_num           The axis number (1-based)
  */
pmdAxis::pmdAxis(pmdController *controller, int axis_num)
  :  asynMotorAxis((asynMotorController*)controller, axis_num)
{
  pc_ = controller;
  encoder_pos_ = 0.0;
  moving_ = false;
  has_encoder_ = false;
  axis_num_ = axis_num;

  motionFinished();
}

asynStatus pmdAxis::poll(bool *moving) {
  pc_->lock();
  
  // Controllers poll the positions at the same time and store them
  // all together.
  int enc_counts;

  if (pc_->writeReadInt("e", enc_counts) == asynSuccess) {
    setDoubleParam(pc_->motorEncoderPosition_, enc_counts);
    setDoubleParam(pc_->motorPosition_, enc_counts);
  }

  int running;
  if (pc_->writeReadInt("*", running) == asynSuccess) {
    *moving = (bool)running;
  }

  // setDoubleParam(pc_->motorVelocity_, vel);

  pc_->unlock();

  *moving = moving_;
  callParamCallbacks();
  return asynSuccess;
}

asynStatus pmdAxis::setEncoderPosition(double position)
{
  return pc_->write("O%d", (int)position);
}

asynStatus pmdAxis::queryStatus() {
  // if (pc_->writeRead(response, command) == asynSuccess) {
  return asynSuccess;
}

void pmdAxis::motionFinished() {
  setIntegerParam(pc_->motorStatusMoving_, 0);
  setIntegerParam(pc_->motorStatusDone_, 1);
  moving_ = false;
}

asynStatus pmdAxis::stop(double acceleration)
{
  return pc_->write("S");
}

asynStatus pmdAxis::home(double min_velocity, double max_velocity, double acceleration, int forwards) {
  return asynError;
}

int pmdAxis::position_to_counts(double position) {
  double res;
  pc_->getDoubleParam(axis_num_, pc_->motorResolution_, &res);

  if (res < 1e-10)
    return 0.0;
  else
    return position / res;
}

double pmdAxis::counts_to_position(int counts) {
  double res;
  pc_->getDoubleParam(axis_num_, pc_->motorResolution_, &res);
  return res * counts;
}

asynStatus pmdAxis::move(double position, int relative, double min_velocity, double max_velocity, double acceleration)
{
  asynPrint(pc_->pasynUser_, ASYN_TRACE_FLOW | ASYN_TRACE_ERROR,
    "%s:%s: axis %d: move to %g (relative=%d)\n",
    driverName, __func__, axis_num_, 
    position, relative);
 
  asynStatus ret = pc_->write("T%d", position_to_counts(position));

  if (ret == asynSuccess)
    moving_ = true;

  return ret;
  
}

asynStatus pmdAxis::moveVelocity(double min_velocity, double max_velocity, double acceleration) {
  asynPrint(pc_->pasynUser_, ASYN_TRACE_FLOW,
    "%s:%s: axis %d: jog with max velocity %g accel %g\n",
    driverName, __func__, axis_num_, 
    max_velocity, acceleration);

  asynStatus ret = asynError;

  if (ret == asynSuccess)
    moving_ = true;

  return ret;
}

asynStatus pmdAxis::setClosedLoop(bool closed) {
  return asynError; 
}
