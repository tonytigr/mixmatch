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
motor LeftDriveSmart = motor(PORT4, 1, false);
motor RightDriveSmart = motor(PORT10, 1, true);
drivetrain Drivetrain = drivetrain(LeftDriveSmart, RightDriveSmart, 200, 173, 76, mm, 1);
motor BeamArmMotorA = motor(PORT12, false);
motor BeamArmMotorB = motor(PORT6, true);
motor_group BeamArm = motor_group(BeamArmMotorA, BeamArmMotorB);

motor PinArmMotorA = motor(PORT5, false);
motor PinArmMotorB = motor(PORT11, true);
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

#pragma region Global
  bool pinClaw = true;
  bool beamClaw = false;
  bool pushClaw = true;
  const int beam_arm_position 		[4] = { 0, 200, 660 , 1080 };
  int beamArmLevel = 0;
  const int pin_arm_position 	[4] = { 0, 280 ,390, 800 };
  int pinArmLevel = 0;

  bool drivetrain_l_needs_to_be_stopped_controller = false;
  bool drivetrain_r_needs_to_be_stopped_controller = false;
  bool remote_control_code_enabled = true;
#pragma endregion Global

#pragma region Event
  //PIN CLAW
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

  //BEAM CLAW
  bool beamClawLast = true;
  bool isBeamClawChange(){
    if(beamClaw!=beamClawLast){
      beamClawLast = beamClaw;
      return true;
    }
    return false;
  }
  void beamClawEvent(){
    if(beamClaw==false){
      if(beamArmLevel==0){
          beamClaw = true;
          wait(100,msec);
          beamArmLevel =1;
      }
    }else{
      beamClaw = false;
    }
  }

  //Push CLAW
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

  //BEAM ARM
  int beamArmLastLevel = 1;
  bool isBeamArmChange(){
    if(beamArmLevel!=beamArmLastLevel){
      beamArmLastLevel = beamArmLevel;
      return true;
    }
    return false;
  }
  void beamArmUpEvent(){
    if(Controller.ButtonFDown.pressing()==1){
      BeamArm.spinFor(forward,20,degrees,false);
    }else{
      int targetLevel = fmod(beamArmLevel+1,4);
      //move beam arm only when pin arm is not flipped
      if(targetLevel==0 || pinArmLevel !=3){
        beamArmLevel = targetLevel;
      }
    }
  }
  void beamArmDownEvent(){
    if(Controller.ButtonFDown.pressing()==1){
      BeamArm.spinFor(reverse,20,degrees,false);
    }else{
      int targetLevel = fmod(beamArmLevel+3,4);
      if(targetLevel!=3){
        targetLevel=0;
      }
      ///move beam arm only when pin arm is not flipped
      if(targetLevel==0 || pinArmLevel !=3){
        beamArmLevel = targetLevel;
      }
    }
  }

  //PIN ARM
  int pinArmLastLevel = 1;
  bool isPinArmChange(){
    if(pinArmLevel!=pinArmLastLevel){
      //recover beam arm carry position
      if(pinArmLastLevel==3 && beamClaw){
        beamArmLevel = 1;
      }
      pinArmLastLevel = pinArmLevel;
      return true;
    }
    return false;
  }
  void pinArmUpEvent(){
    if(Controller.ButtonEDown.pressing()==1){
      PinArm.spinFor(forward,10,degrees,false);
    }else{
      int targetLevel = fmod(pinArmLevel+1,4);
      //flip pin arm only when beam arm is down
      if(targetLevel==3 && beamArmLevel==1){
        beamArmLevel =0;
      }
      pinArmLevel = targetLevel;
    }
  }
  void pinArmDownEvent(){
    if(Controller.ButtonEDown.pressing()==1){
      PinArm.spinFor(reverse,10,degrees,false);
    }else{
      int targetLevel = fmod(pinArmLevel+3,4);
      if(targetLevel!=3){
        targetLevel=0;
      }
      //flip pin arm only when beam arm is down
      if(targetLevel==3 && beamArmLevel==1){
        beamArmLevel =0;
      }
      pinArmLevel = targetLevel;
    }
  }
  void registerButtonEvents(){
    // Register controller callbacks
    Controller.ButtonLUp.pressed(pinArmUpEvent);
    Controller.ButtonLDown.pressed(pinArmDownEvent);

    Controller.ButtonRUp.pressed(beamArmUpEvent);
    Controller.ButtonRDown.pressed(beamArmDownEvent);

    Controller.ButtonEUp.pressed(pinClawEvent);
    Controller.ButtonFUp.pressed(beamClawEvent);

    // Delay to ensure events register properly
    wait(15, msec);
  }
#pragma endregion Event

#pragma region Action

  void initializeMotors() {
    Pneumatic2.pumpOn();
    Pneumatic7.pumpOn();
    Drivetrain.setTurnVelocity(70, percent);
    Drivetrain.setDriveVelocity(100, percent);
    //reset Beam Arm
    BeamArm.setMaxTorque(50, percent);
    BeamArm.setVelocity(100,percent);
    BeamArm.setStopping(hold);
    while (true){
      BeamArm.spin(reverse);
      if(BeamArm.current(percent) > 70) {
        BeamArm.stop();
        BeamArm.spinFor(forward,150,degrees);
        BeamArm.stop();
        BeamArm.setMaxTorque(100, percent);
        BeamArm.setPosition(0,degrees);
        break;
      }
    }
    //reset Pin Arm
    PinArm.setMaxTorque(50, percent);
    PinArm.setVelocity(20,percent);
    PinArm.setStopping(hold);
    while (true){
      PinArm.spin(reverse);
      if(PinArm.current(percent) > 30) {
        PinArm.spinFor(forward,10,degrees);
        PinArm.stop();
        PinArm.setVelocity(70,percent);
        PinArm.setMaxTorque(100, percent);
        PinArm.setPosition(0,degrees);
        break;
      }
    }
  }
  void ArmActionController(){
    while (true){
        if(isBeamArmChange()){
          BeamArm.spinToPosition(beam_arm_position[beamArmLevel],degrees,false);
          printf("beam arm set to level %d \n",beamArmLevel);
        }
        if(abs(PinArm.position(degrees)-pin_arm_position[pinArmLevel])<10 && (pinArmLevel==1)){
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
        //printf("distance to beam arm %.2f \n",Distance9.objectDistance(mm));
        //printf("pin arm position %.2f \n",PinArm.current(percent));
        printf("beam arm position %.2f \n",BeamArm.position(degrees));

        wait(100,msec);
    }
  }
  void driveActionController(){
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
    wait(20, msec);
  }

#pragma endregion Action

int main() {

  //system event aaction handlers
  thread armController = thread(ArmActionController);
  thread driveController = thread(driveActionController);

  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  initializeMotors();

  registerButtonEvents();

  Brain.playSound(tada);

  while (true) {
   // Wait before repeating (20 ms)
    wait(20, msec);
  }
}
