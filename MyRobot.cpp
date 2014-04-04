#include "WPILib.h"
#include "NetworkTables/NetworkTable.h"

class File2014 : public SimpleRobot
{
	Victor shooter; 
	Victor forklift; 
	Victor catcher; 
	Victor catcher2;
	
//#if 0
	Victor frontLeft;
	Victor frontRight;
	Victor rearLeft;
	Victor rearRight;
//#endif

	RobotDrive myRobot;
	Joystick stickA; 
	Joystick stickN; 
		
	DigitalInput stopCocking;
	DigitalInput shoot;
	DigitalInput catcherTop;
	DigitalInput catcherBottom;
	DigitalInput pickerStop;
	DigitalInput autoPick;
	
	Compressor compressor;
	DoubleSolenoid shooterSole;
	DoubleSolenoid pickerSoleR;
	DoubleSolenoid pickerSoleL;

	Timer autonTimer;

public:
	NetworkTable *table;
	NetworkTable *tableSmartDashboard;

	File2014():
		frontLeft(1),
		frontRight(2),
		rearLeft(3),
		rearRight(8),
		
		myRobot(&frontLeft, &frontRight, &rearLeft, &rearRight),
		//myRobot(1, 2, 3, 8),
		stickA(1),		
		stickN(2),
		
		shooter(6),
		forklift(5),
		catcher(9),
		catcher2(4),
		
		stopCocking(10),
		shoot(3),
		catcherTop(2),
		catcherBottom(5),
		pickerStop(1),
		autoPick(13),
		
		compressor(14,7),//switch, relay
		shooterSole(2,3),
		pickerSoleR(8,5),
		pickerSoleL(6,7)
	{
#if 1
		table = NetworkTable::GetTable("3045RobotVision");
		table->PutBoolean("hotTarget", false);
		table->PutNumber("throttle", 0.0);
#endif
		
		myRobot.SetExpiration(0.1);
	}

	void SetMotorDirections()
	{
		myRobot.SetInvertedMotor(myRobot.kRearLeftMotor, true);
		myRobot.SetInvertedMotor(myRobot.kRearRightMotor, true);
		myRobot.SetInvertedMotor(myRobot.kFrontLeftMotor, true);
		myRobot.SetInvertedMotor(myRobot.kFrontRightMotor, true);
	}

	void Autonomous()
	{
		/*
		 * Auton checklist:
		 * -set time to 0
		 * -0.5 seconds
		 * -drive(until in range for hotTarget)
		 * -cock shooter
		 * -0.5 seconds
		 * -shoot if hotTarget = true or if 8 seconds have passed
		 */
		
		autonTimer.Start();
		double autonStartTime = autonTimer.Get();

		myRobot.SetSafetyEnabled(false);
		
		SetMotorDirections();

		myRobot.Drive(0.0, 0.0); 	// make sure robot is stopped

		pickerSoleR.Set(pickerSoleR.kReverse);
		pickerSoleL.Set(pickerSoleL.kReverse);
		forklift.SetSpeed(-.25); // I don't know how long but not for long
		Wait(0.5);
		forklift.SetSpeed(0.00); 
		Wait(0.5);
		
#if 1
		float autonSpeed = table->GetNumber("autonSpeed", 0.0);
		float autonTime = table->GetNumber("autonTime", 0.0);
		float autonCurve = table->GetNumber("autonCurve", 0.0);
		bool auton0 = table->GetBoolean("auton0", false);
		bool auton1 = table->GetBoolean("auton1", false);
		bool auton2 = table->GetBoolean("auton2", false);
		bool shootEnable = table->GetBoolean("shootEnable", false);

		table->PutBoolean("robotTalking", true);

		if (auton0) {
#if 1
		myRobot.Drive(0.0, 0.0); 	// stop robot
		myRobot.Drive(-autonSpeed, autonCurve); 	// drive forwards half speed // left
									// - = left; + = right
		Wait(0.25); 				//    for .25 seconds
		myRobot.Drive(0.0, 0.0); 	// stop robot
#endif
		}

		else if (auton1) {
#if 1
			//printf("start loop\n");
			double throttle = 0.0;
			//table->PutNumber("throttle", throttle);
			bool firstTime = true;
			while (true) {
				if (firstTime) {
					//printf("start inner loop\n");
					firstTime = false;
				}
				myRobot.Drive(0.0, 0.0);
				throttle = table->GetNumber("throttle", 0.0);
				if (throttle != 0.0) {
					table->PutNumber("throttle", 0.0);
					//Wait(0.05);

					double cumTime = 0.0;
					double startTime = 0.0;

			// drive for autonomous_Forward_Time
					startTime = autonTimer.Get();
					//table->PutNumber("startTime", startTime);
					cumTime = autonTimer.Get();
					//printf("time start: %g\n", cumTime);
					while ((cumTime-startTime) < 0.1) {
						myRobot.Drive(-throttle, 0.0);
						cumTime = autonTimer.Get();
						//table->PutNumber("cumTime", cumTime);
						//printf("time end: %g\n", cumTime);
					}
				} else {
					// stop
					myRobot.Drive(0.0, 0.0);
				}
			}
#endif
			}
		else if (auton2) {

		}
		else {

		}
		
		//bool shootEnable = false;
		if (shootEnable) {
			printf("cock and shoot \n");
			bool stop = true;
			if (shoot.Get())
				stop = false;
			
			while(stop){
			shooter.SetSpeed(-1);
			if (shoot.Get())
				stop = false;
			}
			
			shooter.SetSpeed(0);
			//Check if Goal is hot
			
			shooterSole.Set(shooterSole.kForward);//Shoots the ball
		}
#endif


#if 0
#endif				

	}
	
	void OperatorControl()
	{
		myRobot.SetSafetyEnabled(true);
		GetWatchdog().SetEnabled(true);
		compressor.Start();
		//Inverting all true for bag
		myRobot.SetInvertedMotor(myRobot.kRearLeftMotor, true);
		myRobot.SetInvertedMotor(myRobot.kRearRightMotor, true);
		myRobot.SetInvertedMotor(myRobot.kFrontLeftMotor, true);
		myRobot.SetInvertedMotor(myRobot.kFrontRightMotor, true);
		shooterSole.Set(shooterSole.kForward);
	
		while (true)
		{
			GetWatchdog().Feed();
			myRobot.ArcadeDrive(stickA);
			//Forklift Code
			double forkliftSpeed = 0;
			if (stickA.GetRawButton(6)){
				forkliftSpeed = .25;
			}
						
			if (stickA.GetRawButton(5)){
				forkliftSpeed = -.25;
			}
		
			if (!(stickA.GetRawButton(6)||stickA.GetRawButton(5))){
				forkliftSpeed = 0;
			}

			/*if (pickerStop.Get()){
				forkliftSpeed = 0;
			}
			if (pickerStop.Get()&&stickA.GetRawButton(6)){
				forkliftSpeed = -.5;
			}*/
			
			forklift.SetSpeed(forkliftSpeed);
	
			//Shooter Motor Code
			double shooterSpeed = 0;
			
			if (stickN.GetRawButton(4)) {
				shooterSpeed = 1;
			}
			if (stickN.GetRawButton(2)){
				shooterSpeed = -1;
			}
			if (!(stickN.GetRawButton(2)||stickN.GetRawButton(4))){
				shooterSpeed = 0;
			}

			if (shoot.Get()){
				shooterSpeed = 0;
			}
						
#if 0

			if (stopCocking.Get()){
							shooterSpeed = 0;
						}
			if (stopCocking.Get()&&stickN.GetRawButton(2)){
				shooterSpeed = -1;
			}
#endif						
			if (shoot.Get()&&stickN.GetRawButton(4)){
				shooterSpeed = 1;
			}
			
			shooter.SetSpeed(shooterSpeed);
			
			//Shooter Pneumatic Code
			if(stickN.GetRawButton(1)){
				shooterSole.Set(shooterSole.kForward);
			}
			if(stickN.GetRawButton(3)){
				shooterSole.Set(shooterSole.kReverse);
			}
			if(stickA.GetRawButton(8)){
				pickerSoleR.Set(shooterSole.kForward);
				pickerSoleL.Set(shooterSole.kForward);
			}
			if(stickA.GetRawButton(7)){
				pickerSoleR.Set(shooterSole.kReverse);
				pickerSoleL.Set(shooterSole.kReverse);
			}
			
			//Catcher Code
			double catcherSpeed = 0;
			if (stickA.GetRawButton(2)){
				catcherSpeed = 1;
			}
						
			if (stickA.GetRawButton(4)){
				catcherSpeed = -1;
			}
						
			if (!(stickA.GetRawButton(2)||stickA.GetRawButton(4))){
				catcherSpeed = 0;
			}
			
			catcher.SetSpeed(catcherSpeed);
			catcher2.SetSpeed(catcherSpeed);
			
			/*if (autoPick.Get()){
					//catcher.SetSpeed(0);
					pickerSole.Set(pickerSole.kForward);
			}*/
			GetWatchdog().Feed();
		}//Close Loop
	}//Close Operator Control Method
	
	void Test() {

	}
};

START_ROBOT_CLASS(File2014);

