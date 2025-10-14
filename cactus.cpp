#pragma region VEXcode Generated Robot Configuration
// Make sure all required headers are included.
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>


#include "vex.h"

using namespace vex;

// Brain should be defined by default
brain Brain;


// START IQ MACROS
#define waitUntil(condition)                                                   \
  do {                                                                         \
    wait(5, msec);                                                             \
  } while (!(condition))

#define repeat(iterations)                                                     \
  for (int iterator = 0; iterator < iterations; iterator++)
// END IQ MACROS


// Robot configuration code.
inertial BrainInertial = inertial();
controller Controller = controller();
motor LeftDriveSmart = motor(PORT10, 1, true);
motor RightDriveSmart = motor(PORT4, 1, false);
drivetrain Drivetrain = drivetrain(LeftDriveSmart, RightDriveSmart, 200, 173, 76, mm, 1);
motor BeamArmMotorA = motor(PORT12, false);
motor BeamArmMotorB = motor(PORT6, true);
motor_group BeamArm = motor_group(BeamArmMotorA, BeamArmMotorB);

motor PinArmMotorA = motor(PORT11, false);
motor PinArmMotorB = motor(PORT5, true);
motor_group PinArm = motor_group(PinArmMotorA, PinArmMotorB);

pneumatic Pneumatic2 = pneumatic(PORT2);
pneumatic Pneumatic7 = pneumatic(PORT7);
distance Distance9 = distance(PORT9);


// generating and setting random seed
void initializeRandomSeed(){
  wait(100,msec);
  double xAxis = BrainInertial.acceleration(xaxis) * 1000;
  double yAxis = BrainInertial.acceleration(yaxis) * 1000;
  double zAxis = BrainInertial.acceleration(zaxis) * 1000;
  // Combine these values into a single integer
  int seed = int(
    xAxis + yAxis + zAxis
  );
  // Set the seed
  srand(seed);
}



void vexcodeInit() {

  // Initializing random seed.
  initializeRandomSeed();
}


// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;

#pragma endregion VEXcode Generated Robot Configuration

//----------------------------------------------------------------------------
//
//    Module:       main.cpp
//    Author:       {author}
//    Created:      {date}
//    Description:  IQ project
//
//----------------------------------------------------------------------------

// Include the IQ Library
#include "iq_cpp.h"

// Allows for easier use of the VEX Library
using namespace vex;

#pragma region PIN CLAW
bool pinClaw = true;//
bool pinClawLast = false;
bool isPinClawChange(){
  if(pinClaw!=pinClawLast){
    pinClawLast = pinClaw;
    return true;
  }
  return false;
}
void pinClawEvent(){
    pinClaw = !pinClaw;
}
#pragma endregion PIN CLAW

#pragma region BEAM CLAW
bool beamClaw = false;
bool beamClawLast = true;
bool isBeamClawChange(){
  if(beamClaw!=beamClawLast){
    beamClawLast = beamClaw;
    return true;
  }
  return false;
}
void beamClawEvent(){
    beamClaw = !beamClaw;
}
#pragma endregion BEAM CLAW

#pragma region Push CLAW
bool pushClaw = true;
bool pushClawLast = false;
bool isPushClawChange(){
  if(pushClaw!=pushClawLast){
    pushClawLast = pushClaw;
    return true;
  }
  return false;
}
void pushClawEvent(){
    pushClaw = !pushClaw;
}
#pragma endregion Push CLAW

#pragma region BEAM ARM
const int beam_arm_position 		[4] = { -50, 50, 580 , 950 };
int beamArmLevel = 0;
int beamArmLastLevel = 1;
bool isBeamArmChange(){
  if(beamArmLevel!=beamArmLastLevel){
    beamArmLastLevel = beamArmLevel;
    return true;
  }
  return false;
}
void beamArmUpEvent(){
  beamArmLevel = fmod(beamArmLevel+1,4);
}
void beamArmDownEvent(){
  beamArmLevel = fmod(beamArmLevel+3,4);
}
#pragma endregion BEAM ARM

#pragma region PIN ARM
const int pin_arm_position 	[4] = { 30, -250 ,-370, -780 };
int pinArmLevel = 0;
int pinArmLastLevel = 1;
bool isPinArmChange(){
  if(pinArmLevel!=pinArmLastLevel){
    pinArmLastLevel = pinArmLevel;
    return true;
  }
  return false;
}
void pinArmUpEvent(){
  pinArmLevel = fmod(pinArmLevel+1,4);
}
void pinArmDownEvent(){
  pinArmLevel = fmod(pinArmLevel+3,4);

}
#pragma endregion PIN ARM

//arm controller
//move arms to attended positions based on instructions
void ArmActionController(){
	while (true){
			if(isBeamArmChange()){
        BeamArm.spinToPosition(beam_arm_position[beamArmLevel],degrees,false);
        printf("beam arm set to level %d \n",beamArmLevel);
 			}
      if(abs(PinArm.position(degrees)-pin_arm_position[pinArmLevel])<10 && pinArmLevel==1){
        pushClaw = false;
      }else{
        pushClaw = true;
      }
      if(isPinArmChange()){
        PinArm.spinToPosition(pin_arm_position[pinArmLevel],degrees,false);
        printf("Pin arm set to level %d \n",pinArmLevel);
 			}
      if(isPinClawChange()){
        if (pinClaw ){
          Pneumatic7.extend(cylinder1);
          Pneumatic2.extend(cylinder2);
        }else{
          Pneumatic7.retract(cylinder1);
          Pneumatic2.retract(cylinder2);
        }
      }
      if(isBeamClawChange()){
        if (beamClaw ){
          Pneumatic7.extend(cylinder2);
        }else{
          Pneumatic7.retract(cylinder2);
        }
      }
      if(isPushClawChange()){
        if (pushClaw ){
          Pneumatic2.extend(cylinder1);
        }else{
          Pneumatic2.retract(cylinder1);
        }
      }
			wait(10,msec);
	}
}

bool drivetrain_l_needs_to_be_stopped_controller = false;
bool drivetrain_r_needs_to_be_stopped_controller = false;
bool remote_control_code_enabled = true;

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  Pneumatic2.pumpOn();
  Pneumatic7.pumpOn();
  BeamArm.setVelocity(100, percent);
  BeamArm.setMaxTorque(100, percent);
  PinArm.setVelocity(70, percent);
  PinArm.setMaxTorque(100, percent);
  Drivetrain.setTurnVelocity(70, percent);
  Drivetrain.setDriveVelocity(100, percent);
  BeamArm.setStopping(hold);
  PinArm.setStopping(hold);

  //system event handlers
  thread armController = thread(ArmActionController);

  // Register controller callbacks
  Controller.ButtonLUp.pressed(pinArmUpEvent);
  Controller.ButtonLDown.pressed(pinArmDownEvent);

  Controller.ButtonRUp.pressed(beamArmUpEvent);
  Controller.ButtonRDown.pressed(beamArmDownEvent);

  Controller.ButtonEUp.pressed(pinClawEvent);
  //Controller.ButtonEDown.pressed(controllerbuttonEDownPressed);

  Controller.ButtonFUp.pressed(beamClawEvent);
  //Controller.ButtonFDown.pressed(controllerbuttonFDownPressed);

  // Delay to ensure events register properly
  wait(15, msec);

  while (true) {
    if (remote_control_code_enabled) {

      // Calculate drivetrain speeds from joystick positions
      // left = AxisA + AxisC
      // right = AxisA - AxisC
      int drivetrain_left_side_speed =
          Controller.AxisA.position() + Controller.AxisC.position();
      int drivetrain_right_side_speed =
          Controller.AxisA.position() - Controller.AxisC.position();

      // --- Deadband control for left motor ---
      if (drivetrain_left_side_speed < 5 && drivetrain_left_side_speed > -5) {
        if (drivetrain_l_needs_to_be_stopped_controller) {
          LeftDriveSmart.stop();
          drivetrain_l_needs_to_be_stopped_controller = false;
        }
      } else {
        drivetrain_l_needs_to_be_stopped_controller = true;
      }

      // --- Deadband control for right motor ---
      if (drivetrain_right_side_speed < 5 && drivetrain_right_side_speed > -5) {
        if (drivetrain_r_needs_to_be_stopped_controller) {
          RightDriveSmart.stop();
          drivetrain_r_needs_to_be_stopped_controller = false;
        }
      } else {
        drivetrain_r_needs_to_be_stopped_controller = true;
      }

      // --- Drive control ---
      if (drivetrain_l_needs_to_be_stopped_controller) {
        LeftDriveSmart.setVelocity(drivetrain_left_side_speed, percent);
        LeftDriveSmart.spin(forward);
      }
      if (drivetrain_r_needs_to_be_stopped_controller) {
        RightDriveSmart.setVelocity(drivetrain_right_side_speed, percent);
        RightDriveSmart.spin(forward);
      }

    }
    //printf("distance to beam arm %.2f \n",Distance9.objectDistance(mm));
    // Wait before repeating (20 ms)
    wait(20, msec);
  }
}
