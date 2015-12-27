// This file contains all macros and preprocessor directives

// Header files
// Header file for WPI standard hardware/robot overhead.
#include "WPILib.h" 

// Header file for math functions
#include <math.h>

//Left joystick button
#define HIGHGEARBUTTON leftJoystick->GetRawButton(3) //Button to go into high gear
#define LOWGEARBUTTON leftJoystick->GetRawButton(4) //Button to go into low gear

//Right joystick buttons
#define STRAIGHTMODEBUTTON rightJoystick->GetRawButton(1) //Button for going straight

//Speed joystick buttons 
#define STINGERBUTTON speedStick->GetRawButton(10) //Button for climbing
#define AIRRAMFOUR speedStick->GetRawButton(8) //
#define AIRRAMONE speedStick->GetRawButton(6) //
#define AIRRAMTHREE speedStick->GetRawButton(5)//
#define AIRRAMTWO speedStick->GetRawButton(4) //
#define ROLLERBUTTON speedStick->GetRawButton(2) //Button to spin up the rollers
#define SHOOTBUTTON speedStick->GetRawButton(1)  //Button to activate the air ram to fire the frisbees

// Macro Functions 
// Clears the Driver Station output display
#define CLEARMESSAGE DriverStationLCD::GetInstance()->Clear()

// Provides full free-form formatting, as per the usual printf(). Displays formatted text, a, on Driver Station on line, b.
#define DISPLAYPRINTF(b, a, ...) {DriverStationLCD::GetInstance()->Printf(DriverStationLCD::kUser_Line ## b, 1, a, ## __VA_ARGS__); DriverStationLCD::GetInstance()->UpdateLCD();}

// Macros
// Object type macros
#define DRIVEMOTOR Talon // Change to type of PWM controller being used for drive motors

// Port number macros
// Motors
#define LFMOTORPORT 7 // Port number for the left front motor 7
#define LRMOTORPORT 6 // Port number for the left rear motor 6
#define RFMOTORPORT 5 // Port number for the right front motor 5
#define RRMOTORPORT 2 // Port number for the right rear motor 2
#define SHOOTERMOTORPORTTWO 1 //Port number for the arm motor 
#define SHOOTERMOTORPORT 4 // 3Port number for the rear arm motor

// Joysticks
#define RIGHTJOYSTICKPORT 1 // Port number for the right jostick
#define LEFTJOYSTICKPORT 2 // Port number for the left joystick
#define SPEEDSTICKPORT 3 //Port number for the speed stick

//Digital input
#define FRONTDRIVESERVOPORT 10 //Port numbers for the servo //10
#define REARDRIVESERVOPORT 9 //9

//Relay
#define LEDPORT 1

//Compressor
#define COMPRESSORSPIKEPORT 8 //Port number for the spike that controls the compressor
#define COMPRESSORSWITCHPORT 2 //Port number for the switch that controls whether to turn the compressor on or off

//Solenoids
#define STINGERPORT 2
#define AIRRAM1PORT 1

//Deadbands
#define JOYSTICKDEADBAND 0.08 //minimal detection of the joystick

//Servo angles
#define LOWGEARANGLE 200.0 //Low gear
#define HIGHGEARANGLE 10.0 //High gear

//WAGWON
#define SPEEDINCREMENT 0.0009f //speed increment for drifting tank
