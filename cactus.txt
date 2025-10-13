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
motor LeftDriveSmart = motor(PORT10, 1, false);
motor RightDriveSmart = motor(PORT4, 1, true);
drivetrain Drivetrain = drivetrain(LeftDriveSmart, RightDriveSmart, 200, 173, 76, mm, 1);
motor BeamArmMotorA = motor(PORT11, false);
motor BeamArmMotorB = motor(PORT5, true);
motor_group BeamArm = motor_group(BeamArmMotorA, BeamArmMotorB);

motor PinArmMotorA = motor(PORT12, false);
motor PinArmMotorB = motor(PORT6, true);
motor_group PinArm = motor_group(PinArmMotorA, PinArmMotorB);

pneumatic Pneumatic2 = pneumatic(PORT2);
pneumatic Pneumatic7 = pneumatic(PORT7);
sonar Distance9 = sonar(PORT9);


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

int pneumatic21state = 0;
int pneumatic22state = 0;
int pneumatic71state = 0;
int pneumatic72state = 0;

void controllerbuttonLDownPressed(){
    PinArm.spin(forward);
}
void controllerbuttonLDownReleased(){
  PinArm.stop();
}

void controllerbuttonRDownPressed(){
    PinArm.spin(reverse);  
}
void controllerbuttonRDownReleased(){
  PinArm.stop();
}

void controllerbuttonLUpPressed(){
    if (pneumatic71state == 1){
        Pneumatic7.retract(cylinder1);
        pneumatic71state = 0;      
    }else{
        Pneumatic7.extend(cylinder1);
        pneumatic71state = 1;

    }
}
void controllerbuttonRUpPressed(){
    if (pneumatic22state == 1){
        Pneumatic2.retract(cylinder2);
        pneumatic22state = 0;      
    }else{
        Pneumatic2.extend(cylinder2);
        pneumatic22state = 1;

    }

}
void controllerbuttonEUpPressed(){
      if (pneumatic72state == 1){
        Pneumatic7.retract(cylinder2);
        pneumatic72state = 0;
      }else{
        Pneumatic7.extend(cylinder2);
        pneumatic72state = 1;

      }
}

void controllerbuttonEDownPressed(){
    if (pneumatic21state == 1){
        Pneumatic2.retract(cylinder1);
        pneumatic21state = 0;      
    }else{
        Pneumatic2.extend(cylinder1);
        pneumatic21state = 1;

    }
}
// Global variables
bool f_buttons_control_motors_stopped = true;
bool drivetrain_l_needs_to_be_stopped_controller = false;
bool drivetrain_r_needs_to_be_stopped_controller = false;
bool remote_control_code_enabled = true;

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  Pneumatic2.pumpOn();
  Pneumatic7.pumpOn();
  Pneumatic7.retract(cylinder2);
  Pneumatic2.extend(cylinder2);
  Pneumatic7.extend(cylinder1);
  Pneumatic2.retract(cylinder1);
  BeamArm.setVelocity(100, percent);
  BeamArm.setMaxTorque(100, percent);
  PinArm.setVelocity(70, percent);
  PinArm.setMaxTorque(100, percent);
  Drivetrain.setTurnVelocity(70, percent);
  Drivetrain.setDriveVelocity(100, percent);
  BeamArm.setStopping(hold);
  PinArm.setStopping(hold);
  //system event handlers

  // Register controller callbacks
  Controller.ButtonLDown.pressed(controllerbuttonLDownPressed);
  Controller.ButtonLDown.released(controllerbuttonLDownReleased);
  Controller.ButtonRDown.pressed(controllerbuttonRDownPressed);
  Controller.ButtonRDown.released(controllerbuttonRDownReleased);
  Controller.ButtonEUp.pressed(controllerbuttonEUpPressed);
  Controller.ButtonRUp.pressed(controllerbuttonRUpPressed);
  Controller.ButtonEDown.pressed(controllerbuttonEDownPressed);
  Controller.ButtonLUp.pressed(controllerbuttonLUpPressed);

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

      // --- Beam arm control using FUp/FDown buttons ---
      if (Controller.ButtonFUp.pressing()) {
        BeamArm.spin(forward);
        f_buttons_control_motors_stopped = false;
      } else if (Controller.ButtonFDown.pressing()) {
        BeamArm.spin(reverse);
        f_buttons_control_motors_stopped = false;
      } else if (!f_buttons_control_motors_stopped) {
        BeamArm.stop();
        f_buttons_control_motors_stopped = true;
      }
    }

    // Wait before repeating (20 ms)
    wait(20, msec);
  } 
}
