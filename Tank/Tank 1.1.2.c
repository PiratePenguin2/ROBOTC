#pragma config(Sensor, dgtl9,  leftEncoder,    sensorQuadEncoder)
#pragma config(Sensor, dgtl11, rightEncoder,   sensorQuadEncoder)
#pragma config(Motor,  port1,           rightMotor,    tmotorVex393HighSpeed_HBridge, openLoop)
#pragma config(Motor,  port2,           armMotor,      tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port9,           clawMotor,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port10,          leftMotor,     tmotorVex393HighSpeed_HBridge, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

const int tol = 20;
const float tankMaxSpeed = 127;
int tps = 20;

const int encoderTicks = 360;
const int wheelDiam = 4;
const int wheelWidth = 12;

//const float motorMax = 127;
const float motorHigh = 75;	//75
const float motorLow = 50;	//50

const float motorTurnStart = 50;
const float motorTurnHigh = 55;
const float motorTurnLow = 45;

const float motorStop = 0;

const float speedSmoothingFast = 0.001;
const float speedSmoothingSlow = 0.25;

float targetSpeedL = 0.0;
float targetSpeedR = 0.0;
float smoothSpeedL = 0.0;
float smoothSpeedR = 0.0;

const float armRaise = 60;
const float armLower = -40;
const float armHold  = 20;

const float clawOpen  = 70;
//const float clawClose = -30;
const float clawGrip  = -1;

int step = 0;
int ticksLI = 0; //just a variable to store which tick we're on (for the left side)
int ticksLF = 0;
int ticksRI = 0;
int ticksRF = 0;
int speedL = 0;
int speedR = 0;

bool tank = true;
bool joystick = false;

bool clawCloseState = false;
bool clawGripState = false;


/*------------------------*\
|*    Helpful Equations   *|
\*------------------------*/

            /*void beep()
			{
                if (time1[T3] < 1000 + 500)
                {
                    clearTimer(T3);
                }
                while (time1[T3] < 1000)
                {
				    playTone(220, 500);
                }
			}*/

            float lerp(float a, float b, float weight)
            {
                return a * (1 - weight) + b*weight;
            }

        void zero()
        {
            clearTimer(T1);
            clearTimer(T2);
            SensorValue[leftEncoder] = 0;
            SensorValue[rightEncoder] = 0;
            smoothSpeedL = 0;
            smoothSpeedR = 0;
            targetSpeedL = 0;
            targetSpeedR = 0;
            motor[leftMotor] = 0;
            motor[rightMotor] = 0;

            step = 0;
            ticksLI = 0;	ticksRI = 0;
            ticksLF = 0;	ticksRF = 0;
            speedL 	= 0;	speedR 	= 0;
        }

        void changeDelta()
				{
					// ticks final = current # of ticks in wheel
					ticksLF = abs(SensorValue[leftEncoder]);
					ticksRF = abs(SensorValue[rightEncoder]);

					// speed (angular velocity, they call it) = ticks final - ticks initial
					//speedL = ticksLF - ticksLI;
					//speedR = ticksRF - ticksRI;

					if (ticksLF - ticksRF >= 10)
					{
						if (targetSpeedL > motorTurnLow)	{ targetSpeedL -= 0.25; }
						if (targetSpeedR < motorTurnHigh)	{ targetSpeedR += 0.25; }
					}
					else if (ticksRF - ticksLF >= 10)
					{
						if (targetSpeedR > motorTurnLow)	{ targetSpeedR -= 0.25; }
						if (targetSpeedL < motorTurnHigh)	{ targetSpeedL += 0.25; }
					}

					// ticks initial = ticks final
					//ticksLI = ticksLF;
					//ticksRI = ticksRF;
				}

        void accelHandling(bool fast)
        {
          smoothSpeedL = lerp(smoothSpeedL, targetSpeedL, fast ? speedSmoothingFast : speedSmoothingSlow);
          smoothSpeedR = lerp(smoothSpeedR, targetSpeedR, fast ? speedSmoothingFast : speedSmoothingSlow);

          if (abs(smoothSpeedL) < 1)	{smoothSpeedL = 0;}
          if (abs(smoothSpeedR) < 1)	{smoothSpeedR = 0;}

          motor(leftMotor) = smoothSpeedL;
          motor(rightMotor) = smoothSpeedR;
        }

/*--------------------------*\
|*    Encoder Autonomous    *|
\*--------------------------*/

    void moveForward(int moveFT, int moveIN)
    {
        int distance = (12 * (moveFT) + moveIN);
        int ticks = (distance / (wheelDiam * PI)) * (encoderTicks);

        zero();
		    while(SensorValue[leftEncoder] < ticks && SensorValue[rightEncoder] < ticks)
		    {
          //Left Motor
        	if (SensorValue[leftEncoder] < ticks)
            {
                if (SensorValue[rightEncoder] - SensorValue[leftEncoder] > tol)  // While less than 5 rotations on the leftEncoder...
                {
                    motor[leftMotor] = motorHigh;
                }
                else
                {
                    smoothSpeedL = lerp(smoothSpeedL, motorLow, speedSmoothingFast);
                    motor[leftMotor] = smoothSpeedL;
                }
            }
            else
            {
                    smoothSpeedL = lerp(smoothSpeedL, motorStop, speedSmoothingFast);
                    motor[leftMotor] = smoothSpeedL;
            }

            //Right Motor
            if (SensorValue[rightEncoder] < ticks)
            {
                if (SensorValue[leftEncoder] - SensorValue[rightEncoder] > tol)  // While less than 5 rotations on the rightEncoder...
                {
                    motor[rightMotor] = motorHigh;
                }
                else
                {
                    smoothSpeedR = lerp(smoothSpeedR, motorLow, speedSmoothingFast);
                    motor[rightMotor] = smoothSpeedR;
                }
            }
            else
            {
                    smoothSpeedR = lerp(smoothSpeedR, motorStop, speedSmoothingFast);
                    motor[rightMotor] = smoothSpeedR;
            }
        }

        motor[leftMotor] = 0;
        motor[rightMotor] = 0;
        wait1Msec(1000);
    }

    void moveBackward(int moveFT, int moveIN)
    {
        int distance = (12 * (moveFT) + moveIN);
        int ticks = (-distance / (wheelDiam * PI)) * (encoderTicks);

        zero();

    while(SensorValue[leftEncoder] > ticks && SensorValue[rightEncoder] > ticks)
    {
            //Left Motor
        if (SensorValue[leftEncoder] > ticks)
            {
                if (SensorValue[leftEncoder] - SensorValue[rightEncoder] > tol)  // While less than 5 rotations on the leftEncoder...
                {
                    motor[leftMotor] = -motorHigh;
                }
                else
                {
                    smoothSpeedL = lerp(smoothSpeedL, -motorLow, speedSmoothingFast);
                    motor[leftMotor] = smoothSpeedL;
                }
            }
            else
            {
                    smoothSpeedL = lerp(smoothSpeedL, -motorStop, speedSmoothingFast);
                    motor[leftMotor] = smoothSpeedL;
            }

            //Right Motor
            if (SensorValue[rightEncoder] > ticks)
            {
                if (SensorValue[rightEncoder] - SensorValue[leftEncoder] > tol)  // While less than 5 rotations on the leftEncoder...
                {
                    motor[rightMotor] = -motorHigh;
                }
                else
                {
                    smoothSpeedR = lerp(smoothSpeedR, -motorLow, speedSmoothingFast);
                    motor[rightMotor] = smoothSpeedR;
                }
            }
            else
            {
                    smoothSpeedR = lerp(smoothSpeedR, motorStop, speedSmoothingFast);
                    motor[rightMotor] = smoothSpeedR;
            }
        }

        motor[leftMotor] = 0;
        motor[rightMotor] = 0;
        wait1Msec(1000);
    }

    void pointTurn(float deg, float offset)
    {
        //pointTurn			//(r +- offset/2) * (deg/360) * (1/(wheelDiam * PI)) * 360
        int totalLticks  = (int)( ((offset + wheelWidth/1) * PI) * (deg/360) * (1/(wheelDiam * PI)) * 360 );//((deg/2) / (wheelDiam * PI)) * (encoderTicks);
        int totalRticks  = (int)( ((offset - wheelWidth/1) * PI) * (deg/360) * (1/(wheelDiam * PI)) * 360 );

        zero();
        targetSpeedL = motorTurnStart;
        targetSpeedR = motorTurnStart;

        while(abs(SensorValue[leftEncoder]) < abs(totalLticks) && abs(SensorValue[rightEncoder]) < abs(totalRticks))
    {
        step++;
        if (step % 10 == 0)
        {
            changeDelta();
        }

            //Left Motor
        if (abs(SensorValue[leftEncoder]) < abs(totalLticks))
            {
                    smoothSpeedL = lerp(smoothSpeedL, (totalLticks > 0 ? targetSpeedL : -targetSpeedL), speedSmoothingFast);
                    motor[leftMotor] = smoothSpeedL;
            }
            else
            {
                    smoothSpeedL = lerp(smoothSpeedL, motorStop, 1);
                    motor[leftMotor] = smoothSpeedL;
            }

            //Right Motor
            if (abs(SensorValue[rightEncoder]) < abs(totalRticks))
            {
                    smoothSpeedR = lerp(smoothSpeedR, (totalRticks > 0 ? targetSpeedR : -targetSpeedR), speedSmoothingFast);
                    motor[rightMotor] = smoothSpeedR;
            }
            else
            {
                    smoothSpeedR = lerp(smoothSpeedR, motorStop, 1);
                    motor[rightMotor] = smoothSpeedR;
            }
        }

        smoothSpeedL = 0;
        smoothSpeedR = 0;
        motor[leftMotor] = 0;
        motor[rightMotor] = 0;
        wait1Msec(1000);
    }


/*------------------------*\
|*    Timed Autonomous    *|
\*------------------------*/

    void moveForwardTime(int speed, int mSec)
    {
      zero();
      //Speed Up
      while(time1[T1] < mSec)
      {
        targetSpeedL = speed;
        targetSpeedR = speed * 0.95;
        accelHandling(false);
        wait1Msec(1000/tps);
      }

      while(abs(targetSpeedL) > 2 || abs(targetSpeedR) > 2)
      { //Slow Down
        targetSpeedL = 0;
        targetSpeedR = 0;
        accelHandling(true);
      }
      wait1Msec(500);
      zero();
    }

    void pointTurnTime(int speed, int mSec, bool dir)
    {
      zero();
      //Speed Up
      while(time1[T1] < mSec)
      {
        targetSpeedL = dir ? speed : -speed;
        targetSpeedR = 0.95 * dir ? -speed : speed;
        accelHandling(false);
        wait1Msec(1000/tps);
      }

      while(abs(targetSpeedL) > 2 || abs(targetSpeedR) > 2)
      { //Slow down
        targetSpeedL = 0;
        targetSpeedR = 0;
        accelHandling(true);
      }
      wait1Msec(500);
      zero();
    }

    void liftArmTime(float armMove, int mSec, float armHold, float openSpeed, float gripStrength)
    {
        motor[armMotor] = openSpeed;
        wait1Msec(mSec);

        motor[armMotor] = gripStrength;
        motor[clawMotor] = armMove;
        wait1Msec(mSec);

        motor[armMotor] = gripStrength;
        motor[clawMotor] = armHold;

    }


void autonomousTime()
{
  zero();
  //Move forward at speed 50 for 5 seconds
  moveForwardTime(50, 5000);
    //Turn true (right) at speed 50 for 2 seconds
    pointTurnTime(25, 2000, true);

  //Arm Raise & Hold
	liftArmTime(50, 1000, armHold, 0, clawGrip);	// +-ArmSpeed, delay, ArmHoldSpeed, +-ClawSpeed, ClawHoldSpeed

  //Move forward at speed 50 for 1 second
  moveForwardTime(50, 1000);
    //Turn true (right) at speed 50 for 2 seconds
    pointTurnTime(25, 2000, true);

  //Move forward at speed 50 for 3 seconds
  moveForwardTime(50, 3000);

  //Relax Claw and Arm
	liftArmTime(0, 0, 0, 0, 0);	// +-ArmSpeed, delay, ArmHoldSpeed, +-ClawSpeed, ClawHoldSpeed

  wait1Msec(50);
  zero();

/* Arm and Claw Functions
    //Arm Raise & Hold
	//liftArmTime(50, 1000, armHold, 0, 0);	// +-ArmSpeed, delay, ArmHoldSpeed, +-ClawSpeed, ClawHoldSpeed

    //Open Claw
	//liftArmTime(0, 1000, 0, 50, 0);	// +-ArmSpeed, delay, ArmHoldSpeed, +-ClawSpeed, ClawHoldSpeed

    //Claw Grip and Arm Raise
	//liftArmTime(50, 2100, armHold, -50, clawGrip);	// +-ArmSpeed, delay, ArmHoldSpeed, +-ClawSpeed, ClawHoldSpeed

    //Relax Claw and Arm
	//liftArmTime(0, 0, 0, 0, 0);	// +-ArmSpeed, delay, ArmHoldSpeed, +-ClawSpeed, ClawHoldSpeed
*/
}

void autonomousRed()
{
    moveForwardTime(40, 7250); //Feet, Inches
        pointTurnTime(40, 250, false);
    moveForwardTime(40, 1200); //Feet, Inches
        pointTurnTime(40, 250, false);
    moveForwardTime(40, 2000); //Feet, Inches
}

void autonomousBlue()
{
    moveForwardTime(40, 7250); //Feet, Inches
        pointTurnTime(40, 250, true);
    moveForwardTime(40, 1200); //Feet, Inches
        pointTurnTime(40, 250, true);
    moveForwardTime(40, 2000); //Feet, Inches
}

/*------------------*\
|*    Normal Mode   *|
\*------------------*/

void tankDrive()
{
    //Left Motor
    {
        if (abs(vexRT[Ch3]) >= tol){
            motor[leftMotor] = vexRT[Ch3] * tankMaxSpeed / 127;
        } else {
            motor[leftMotor] = 0;
        }
    }   //Left Motor

    //Right Motor
    {
        if (abs(vexRT[Ch2]) >= tol){
            motor[rightMotor] = vexRT[Ch2] * tankMaxSpeed / 127;
        } else {
            motor[rightMotor] = 0;
        }
    }   //Right Motor
}	//End tankDrive()

void joystickDrive()
{
	float drive = vexRT[Ch3];	//LY
	float steer = vexRT[Ch1];	//RX

	if (abs(drive) < tol)
	{
		drive = 0;
	}
	if (abs(steer) < tol)
	{
		steer = 0;
	}

  targetSpeedL = (drive + steer);  // (y + x)/2
	targetSpeedR = (drive - steer);  // (y - x)/2

  accelHandling(false);
}   //End joystickDrive

void armAndClaw()
{
//Arm
    {
        if (vexRT[Btn5U] && vexRT[Btn5D])	//Hold
        {
            motor[armMotor] = armHold;
        }
        else if (vexRT[Btn5U])  //Raise
        {
            motor[armMotor] = armRaise;
        }
        else if (vexRT[Btn5D])  //Lower
        {
            motor[armMotor] = armLower;
        }
        else    //Idle
        {
            motor[armMotor] = 0;
        }
    }   //Arm

    //Claw
    {
        //Choose Claw Mode
        if (clawCloseState && time1[T2] > 1000)  //Set to Grip
        {
            clawCloseState = false;
            clawGripState = true;
        }
        else if (vexRT[Btn6D])	//Set to Close
        {
            clawCloseState = true;
            clawGripState = false;
            clearTimer(T2);
        }
        if (vexRT[Btn6U])   //Set to Open
        {
            clawCloseState = false;
            clawGripState = false;
        }

        //Execute
        /*if (clawGripState)   //Grip
        {
            motor[clawMotor] = clawGrip;
        }
        else if (clawCloseState) //Close
        {
            motor[clawMotor] = clawClose;
        }*/
        if (vexRT[Btn6D])	//Grip
        {
        	motor[clawMotor] = clawGrip;
        }
        else if (vexRT[Btn6U])  //Open
        {
            motor[clawMotor] = clawOpen;
        }
        else    //Idle
        {
            motor[clawMotor] = 0;
        }
    }   //Claw
}


/*--------------------*\
|*      Task Main     *|
\*--------------------*/

task main()
{
	tank = false;
	joystick = true;

	while (true)
	{
        //Choose Mode
        if (vexRT[Btn7D])
        {
            tank = true;
            joystick = false;
            zero();
        }
        else if (vexRT[Btn8D])
        {
            joystick = true;
            tank = false;
            zero();
        }

        //Stupid
        if (vexRT[Btn8R] == 1)
        {
            playTone(220, 500);
            //beep();
        }

        //Execute
        if (vexRT[Btn7U] == 1)
        {
            autonomousRed();
        }
        else if (vexRT[Btn8U] == 1)
        {
            autonomousBlue();
        }

        else if (tank)
        {
            tankDrive();
            armAndClaw();
        }
        else if (joystick)
        {
            joystickDrive();
            armAndClaw();
        }

        wait1Msec(1000/tps);
	}

	//Unreferenced functions
	if (false)
	{
		moveForward(0, 0);
		moveBackward(0, 0);
		pointTurn(0, 0);
    autonomousTime();
	}
}


/*
        if (abs(vexRT[Ch2]) >= tol || abs(vexRT[Ch4]) >= tol){
            targetSpeedL = (vexRT[Ch2] + vexRT[Ch4])/2 * (tankMaxSpeed / 127);
            targetSpeedR = (vexRT[Ch2] - vexRT[Ch4])/2 * (tankMaxSpeed / 127);
            accelHandling(false);
        }
        else
        {
            motor[leftMotor] = 0;
            motor[rightMotor] = 0;
        }
*/
