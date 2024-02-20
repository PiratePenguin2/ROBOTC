#pragma config(Motor,  port1,           armMotor,      tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port2,           rightFrontMotor, tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,           leftFrontMotor, tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port10,          clawMotor,     tmotorVex393_HBridge, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*----------------------------------------------------------------------------------------------------*\
|*                           - Dual Joystick Control with Arm - 1 Remote -                            *|
|*                                      ROBOTC on VEX 2.0 Cortex                                      *|
|*                                                                                                    *|
|*  This program uses the Left joystick to run the robot using "joystick control".    								*|
|*	The Right joystick is used to raise and lower the robot arm.																			*|
|*  The Group 5 button on the top-left-front of the VEXnet Joystick is used open the claw.						*|
|*	The Group 6 buttons on the top-right of the VEXnet Joystick are used close and lock the claw. 		*|
|*                                                                                                    *|
|*                                        ROBOT CONFIGURATION                                         *|
|*    NOTES:                                                                                          *|
|*    1)  Ch1 is the X axis and Ch2 is the Y axis for the RIGHT joystick.                             *|
|*    2)  Ch3 is the Y axis and Ch4 is the X axis for the LEFT joystick.                              *|
|*                                                                                                    *|
|*    MOTORS & SENSORS:                                                                               *|
|*    [I/O Port]              [Name]              [Type]              [Description]                   *|
|*    Motor - Port 1          armMotor             VEX Motor           Arm motor                      *|
|*    Motor - Port 2          rightMotor           VEX Motor           Right motor                    *|
|*    Motor - Port 4          leftMotor            VEX Motor           Left motor                     *|
\*----------------------------------------------------------------------------------------------------*/

//+++++++++++++++++++++++++++++++++++++++++++++| MAIN |+++++++++++++++++++++++++++++++++++++++++++++++
bool mainLoop = true;
bool clawLock = false;

float velL = 0.0;
float velLLast = 0.0;
float velR = 0.0;
float velRLast = 0.0;
float accelL = 0.0;
float accelR = 0.0;

float friction = 4.0; //Velocity divided by friction each step, more friction = slower
float maxVel = 127.0; //maximum velocity
float minVel = -100.0; //minimum velocity
float speedMult = 0.08;

//int autoStart = 0;


task main()
{
	while(1 == 1)
	{
		//-----Blue Autonomous
		if (vexRT[Btn7L] == 1)
		{
		mainLoop = false; //Start Autonomous for Blue

		motor[rightFrontMotor] = 127;
		motor[leftFrontMotor] = 127;
		delay(3000);

		motor[leftFrontMotor] = -127; //Turn Left
		delay(500);

		motor[leftFrontMotor] = 127; //Straight
		delay(500);

		motor[leftFrontMotor] = -127; //Turn Left
		delay(500);

		motor[leftFrontMotor] = 127; //Straight
		delay(500);

		motor[rightFrontMotor] = 0; //Stop
		motor[leftFrontMotor] = 0;

		mainLoop = true; //Reset autostart variable
		}


		//-----Red Autonomous-----
		if (vexRT[Btn7R] == 1)
		{
		mainLoop = false; //Start Autonomous for Blue

		motor[rightFrontMotor] = 127;
		motor[leftFrontMotor] = 127;
		delay(3000);

		motor[rightFrontMotor] = -127; //Turn Right
		delay(500);

		motor[rightFrontMotor] = 127; //Straight
		delay(500);

		motor[rightFrontMotor] = -127; //Turn Right
		delay(500);

		motor[rightFrontMotor] = 127; //Straight
		delay(500);

		motor[rightFrontMotor] = 0; //Stop
		motor[leftFrontMotor] = 0;

		mainLoop = true; //Reset autostart variable
		}


		if (mainLoop == true)
		{
			//Right side of the robot is controlled by the right joystick, Y-axis
			//motor[rightFrontMotor] = vexRT[Ch2];
			//motor[rightBackMotor]  = vexRT[Ch2];
			//Left side of the robot is controlled by the left joystick, Y-axis
			//motor[leftFrontMotor] = vexRT[Ch3];
			//motor[leftBackMotor]  = vexRT[Ch3];

		//============================| Drive |============================
			//Lx is Ch1, Ly is Ch2, Ry is Ch3, Rx is Ch4
  	  //If Lx [Ch1] is right, slow down right motor. If it's left, slow down left motor
			if (vexRT[Ch4] >= 0)
			{
				accelL = (vexRT[Ch3] * ((-2 * vexRT[Ch4]) + 127)/127);
				accelR = vexRT[Ch3];
  		}
  		else if (vexRT[Ch4] < 0)
  		{
				accelR = (vexRT[Ch3] * ((2 * vexRT[Ch4]) + 127)/127);
				accelL = vexRT[Ch3];
			}

		//====================| Assign motor speeds to interpolated velocity |======================
			velR = velRLast * (1.0 / friction) + (accelR * speedMult);
			velL = velLLast * (1.0 / friction) + (accelL * speedMult);

			//"""clamp""" motor speeds between the set minimum and maximum

			float a = abs(vexRT[Ch4]) * maxVel;
			float b = -abs(vexRT[Ch4]) * minVel;

			if (velR > a) velR = a;
			else if (velR < b) velR = b;
			if (velL > a) velL = a;
			else if (velL < b) velL = b;

			motor[rightFrontMotor] = velR;
			motor[leftFrontMotor] = velL;

			velRLast = velR;
			velLLast = velL;




		//============================| Arm |============================

			motor[armMotor] = vexRT[Ch2];

	  //============================| BEEEEEEEP |=======================
			if(vexRT[Btn8U] == 1)
			{
					playTone(220, 500);
			}

		//============================| Claw |============================
			// Open, close or do not more claw
			if(vexRT[Btn5U] == 1) 				//If button 5U is pressed...
			{
				clawLock = true;

			}


			if(vexRT[Btn6U] == 1)  	//Else, if button 6U is pressed...
			{
				clawLock = false;
				motor[clawMotor] = 127; 		//...open the gripper.
			}
			else if (vexRT[Btn6D] == 1)    //Else, if Button 6D is pressed...
			{
				motor[clawMotor] = -127;  		//...close the gripper.
			}
			else if (clawLock == true)
			{
				motor[clawMotor] = -20;			//...clamp the gripper
			}
			else
			{
				motor[clawMotor] = 0;    		//...stop the gripper.
			} //Claw IF ELSE
		} 	//Manual Loop
	} 		//While
} 			//Task Main
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


//======= Arm =======

		/*if(vexRT[Btn7U] == 1 & vexRT[Btn5U] == 1)       	//If button 5U is pressed...
		{
			motor[armMotor] = 127/3;    	//...raise the arm.
		}
		else if(vexRT[Btn7U] == 1 & vexRT[Btn5D] == 1)  	//Else, if button 5D is pressed...
		{
			motor[armMotor] = -127/3;   	//...lower the arm.
		}
		else if(vexRT[Btn5U] == 1)
		{
			motor[armMotor] = 127;
		}
		else if(vexRT[Btn5D] == 1)
		{
			motor[armMotor] = -127;
		}
		else {                      	//Else (neither button is pressed)...
			motor[armMotor] = 0;      	//...stop the arm.
		}*/


//======= Claw =======

/*else if(vexRT[Btn8D] == 1)                     		//Else (neither button is pressed)...
		{
			while(vexRT[Btn8D] == 0 && vexRT[Btn6U] == 0)
			{
				motor[clawMotor] = -64;    //...lock the gripper
				if(vexRT[Btn7U] == 1 & vexRT[Btn5U] == 1)       	//If button 5U is pressed...
				{
					motor[armMotor] = 127/3;    	//...raise the arm.
				}
				else if(vexRT[Btn7U] == 1 & vexRT[Btn5D] == 1)  	//Else, if button 5D is pressed...
				{
					motor[armMotor] = -127/3;   	//...lower the arm.
				}
				else if(vexRT[Btn5U] == 1)
				{
					motor[armMotor] = 127;
				}
				else if(vexRT[Btn5D] == 1)
				{
					motor[armMotor] = -127;
				}
				else                      		//Else (neither button is pressed)...
				{
					motor[armMotor] = 0;      	//...stop the arm.
				}
			}*/
