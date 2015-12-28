///******************************************************************************
// * 	Sprockets 2013 code
// * 	Author: 	William Surmak 
// * Last Updated:	April 25, 2013
// ******************************************************************************/
// Include our own header file with our own macros
#include "NewRobot.h"

//Prototype of our custom functions
float Abs(float number);
int AbsI(int number);

// Our main and only class. You could consider this an alteration of "int main(void)" if it makes it easier.
class SimpleTracker : public SimpleRobot
{
    // Local variables 
    DRIVEMOTOR *lfMotor; // Left-front drive motor
    DRIVEMOTOR *lrMotor; // Left-rear drive motor
    DRIVEMOTOR *rfMotor; // Right-front drive motor
    DRIVEMOTOR *rrMotor; // Right-rear drive motor
    Talon *shooterMotor; //Motor for shooting
    Talon *shooterMotor2; //Motor for shooting
    Compressor *airCompressor; //The air compressor
    Relay *compressorSpike; //Spike to turn the compressor on and off
    Relay *LED;
    DigitalInput *pressureSwitch; //The switch that tells whether the compressor has reached the desired psi
    Joystick *rightJoystick; // Right joystick
    Joystick *leftJoystick; // Left joystick
    Joystick *speedStick; //Speed stick
    Servo *frontDriveServo; // Servo to shift front drive 
    Servo *rearDriveServo; // Servo to shift rear drive
    Solenoid *stinger;  //Upside-down stinger for our climbing
    Solenoid *airRam1; //Soon

    /* "Constructor". This code is run when the robot is first turned on.*/ 
    public:
	SimpleTracker(void)
	{	
		// Joysticks
		rightJoystick = new Joystick(RIGHTJOYSTICKPORT); // Right-joystick
		leftJoystick = new Joystick(LEFTJOYSTICKPORT); // Left-joystick
		speedStick = new Joystick(SPEEDSTICKPORT); //Speed stick
		
		// Motors
		lfMotor = new DRIVEMOTOR(LFMOTORPORT); // Left-front drive motor
		lrMotor = new DRIVEMOTOR(LRMOTORPORT); // Left-rear drive motor
		rfMotor = new DRIVEMOTOR(RFMOTORPORT); // Right-front drive motor
		rrMotor = new DRIVEMOTOR(RRMOTORPORT); // Right-rear drive motor
		shooterMotor = new Talon(SHOOTERMOTORPORT); //Shooting motor
		shooterMotor2 = new Talon(SHOOTERMOTORPORTTWO); //Other shooting motor
		
		//Air compressor
		pressureSwitch = new DigitalInput(COMPRESSORSWITCHPORT); //Pressure switch
		compressorSpike = new Relay(COMPRESSORSPIKEPORT); //Spike to run the compressor
		airCompressor = new Compressor(COMPRESSORSWITCHPORT, COMPRESSORSPIKEPORT); //The air compressor
		
		//Digital inputs
		frontDriveServo = new Servo(FRONTDRIVESERVOPORT); //Front drive servo
		rearDriveServo = new Servo(REARDRIVESERVOPORT); //Rear drive servo
		
		//Relays
		LED = new Relay(LEDPORT); //yeeee leds
		
		//Solenoids
		stinger = new Solenoid(STINGERPORT); //the air rams
		airRam1 = new Solenoid(AIRRAM1PORT);
		
		//Starts the air compressor
		airCompressor->Start();
	}
	
    /* Code to run while in tele-op, or driver-controlled  mode. */
	void OperatorControl(void)
	{	
		double leftY, rightY; // Y-positions of the left and right joysticks
		double lfSpeed = 0; //All speeds for drifting tank
		double lrSpeed = 0;
		double rfSpeed = 0;
		double rrSpeed = 0;
		
		while(IsOperatorControl())
		{
			// Teleop code
			GetWatchdog().Feed();
			
			DISPLAYPRINTF(1, "leftY : %0.5f", leftY); //Value of the y axis of the left joystick
			DISPLAYPRINTF(2, "rightY : %0.5f", rightY); //Value of the y axis of the right joystick
			DISPLAYPRINTF(3, "lfMotor : %f", lfSpeed); //Value of the lf Motor for drifting tank
			DISPLAYPRINTF(4, "lrMotor : %f", lrSpeed); //Value of the lr Motor for drifiting tank
			DISPLAYPRINTF(5, "rfMotor : %f", rfSpeed); //Value of the rf motor for drifting tank
			DISPLAYPRINTF(6, "rrMotor : %f", rrSpeed); //Value of the rr motor for drifting tank
			
			//Clears driver station LCD
			CLEARMESSAGE;
			
			//Runs the air compressor until the switch is triggered 
			if(!pressureSwitch->Get()) compressorSpike->Set(Relay::kForward);
			else compressorSpike->Set(Relay::kOff);
			
			//Set the LEDS on
			LED->Set(Relay::kForward);
			
			//Set the motors to spin up
			if(ROLLERBUTTON)
			{
				shooterMotor->Set(1.0f);
				shooterMotor2->Set(1.0f);
			}
			
			//Set the motors to stop
			else 
			{
				shooterMotor->Set(0.0f);
				shooterMotor2->Set(0.0f);
			}
			
			if(SHOOTBUTTON) //if the button is pressed activate the air ram
			{
				airRam1->Set(false);
			}
			else airRam1->Set(true); //else de activate the air ram
			
			//Sets the solenoids either on or off
			if(STINGERBUTTON) stinger->Set(true);
			else stinger->Set(false);
		
			// Get joystick positions
			leftY = leftJoystick->GetRawAxis(2);
			rightY = rightJoystick->GetRawAxis(2);
			
			if(HIGHGEARBUTTON)  //Changes to high gear
			{
				frontDriveServo->SetAngle(HIGHGEARANGLE); //Sets front drive to desired angle
				rearDriveServo->SetAngle(HIGHGEARANGLE); //Sets rear drive to desired angle
				leftY = 0; //Make sure we are not driving when switching gears
				rightY = 0;  //Make sure we are not driving when switching gears
			}
			if(LOWGEARBUTTON) //Changes to low gear
			{
				frontDriveServo->SetAngle(LOWGEARANGLE); //Sets front drive to desired angle 
				rearDriveServo->SetAngle(LOWGEARANGLE); //Sets rear drive to desired angle 
				leftY = 0; //Make sure we are not switching gears
				rightY = 0; //Make sure we are not switching gears
			} 
			
			//If we have have minimal detectable value we count it as 0
			if(Abs(leftY) < JOYSTICKDEADBAND) leftY = 0;
			if(Abs(rightY) < JOYSTICKDEADBAND) rightY = 0;
			
			//If we need to move both motors the same speed to go straight we set both speeds to one joystick
			if(STRAIGHTMODEBUTTON) rightY = leftY;
			
			 //Set motor speeds (calculate the speed for drifting tank) ****UNCOMMENT for DRIFTING TANK****
//			if(lfSpeed < leftY) lfSpeed += SPEEDINCREMENT;
//			if(lfSpeed > leftY) lfSpeed -= SPEEDINCREMENT;
//			if(lrSpeed < leftY) lrSpeed += SPEEDINCREMENT;
//			if(lrSpeed > leftY) lrSpeed -= SPEEDINCREMENT;
//			if(rfSpeed < rightY) rfSpeed += SPEEDINCREMENT;
//			if(rfSpeed > rightY) rfSpeed -= SPEEDINCREMENT;
//			if(rrSpeed < rightY) rrSpeed += SPEEDINCREMENT;
//			if(rrSpeed > rightY) rrSpeed -= SPEEDINCREMENT;
			
			/* ****UNCOMMENT for DRIFTING TANK**** */
//			lfMotor->Set(lfSpeed);
//			lrMotor->Set(lrSpeed);
//			rfMotor->Set(rfSpeed);
//			rrMotor->Set(rrSpeed);
			
			//Test values
//			lfMotor->Set(0.4f);
//			lrMotor->Set(0.4f);
//			rfMotor->Set(-0.4f);
//			rrMotor->Set(-0.4f);
			
			/*UNCOMMENT for NORMAL TANK**** */
			lfMotor->Set(leftY);
			lrMotor->Set(leftY);
			rfMotor->Set(-rightY);
			rrMotor->Set(-rightY);	
		}
	} // End of teleop class functions
	
	/* Code to run while in autonomous, or fully-automated mode. */
	void Autonomous(void)
	{
		int initalPause = 20000;
		int rollerPause = 8000;
		int ramPause = 4000;
		int flag = 0;
		
		while(IsAutonomous())
		{
			// Autonomous code
			GetWatchdog().Feed();
			
			DISPLAYPRINTF(1, "IP %d", initalPause); //Value of our inital pause
			DISPLAYPRINTF(2, "Rollers %d", rollerPause); //Value of our roller pause
			DISPLAYPRINTF(3, "Ram %d", ramPause); //Value of our ram pause
			DISPLAYPRINTF(4, "flag %d", flag);
			
			CLEARMESSAGE; //Clears the driver station LCD
			
			if(initalPause > 0)
			{
				//Wait for the motors to speed up
				shooterMotor->Set(1.0f);
				shooterMotor2->Set(1.0f);
				
				//decrease the pause time
				initalPause--;
				
				//Deactivate the first air ram
				airRam1->Set(true);
			}
			else if(rollerPause > 0)
			{
				//Re wait for the motors to speed up
				shooterMotor->Set(1.0f);
				shooterMotor2->Set(1.0f);
				
				//dcrease the pause time
				rollerPause--;
				
				//Deactivate the first air ram
				airRam1->Set(true);
			}
			else if(ramPause > 0 && flag != 3)
			{
				//Wait for the motors to speed up
				shooterMotor->Set(1.0f);
				shooterMotor2->Set(1.0f);
				
				//Activate the first air ram to shoot
				airRam1->Set(false);
				
				//Decrease the time for the ram to stay activated
				ramPause--;
				
				//If the ram has been activated for a set amount of cycles
				if(ramPause == 0) 
				{
					flag++;
					
					//Re activate the pause time so the rollers can speed up
					rollerPause += 8000;
					ramPause += 4000;
				}
			}
			else
			{
				shooterMotor->Set(0.0f);
				shooterMotor2->Set(0.0f);
				
				airRam1->Set(true);
			}
		}
	} // End of Autonomous

};
// Custom functions
float Abs(float number)
{
	// Return the absolute magnitude of the given number (make it positive)
	if(number < 0.0f) number  = -number;
	return number;
}

int AbsI(int number)
{
	//Returns the absolute magnitude of the given integer (makes it positive)
	if(number < 0) number = -number;
	return number;	
}
/* The entry point is FRC_UserProgram_StartupLibraryInit. Tell the linker where our code is. */
START_ROBOT_CLASS(SimpleTracker);
