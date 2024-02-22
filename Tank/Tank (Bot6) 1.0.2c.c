#pragma config(Sensor, dgtl9,  leftEncoder,    sensorQuadEncoder)
#pragma config(Sensor, dgtl11, rightEncoder,   sensorQuadEncoder)
#pragma config(Motor,  port1,           clawMotor,     tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port2,           rightMotor,    tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,           leftMotor,     tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port10,          armMotor,      tmotorVex393_HBridge, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

const int tol = 20;
const float tankMaxSpeed = 127;
int tps = 8;

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

const float speedSmoothing = 0.001;	//0.001

float targetSpeedL = 0.0;
float targetSpeedR = 0.0;
float smoothSpeedL = 0.0;
float smoothSpeedR = 0.0;

int step = 0;
int ticksLI = 0; //just a variable to store which tick we're on (for the left side)
int ticksLF = 0;
int ticksRI = 0;
int ticksRF = 0;
int speedL = 0;
int speedR = 0;

bool tank = true;
bool joystick = false;

bool clawClose = false;
bool clawGrip = false;


        float lerp(float a, float b, float weight)
        {
            return a * (1 - weight) + b*weight;
        }

        void zero()
        {
            SensorValue[leftEncoder] = 0;
            SensorValue[rightEncoder] = 0;
            smoothSpeedL = 0;
            smoothSpeedR = 0;

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


    void moveForward(int moveFT, int moveIN)
    {
        int distance = (12 * (moveFT) + moveIN);
        int ticks = (distance / (wheelDiam * PI)) * (encoderTicks);

        zero();
		    while(SensorValue[leftEncoder] < ticks || SensorValue[rightEncoder] < ticks)
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
                    smoothSpeedL = lerp(smoothSpeedL, motorLow, speedSmoothing);
                    motor[leftMotor] = smoothSpeedL;
                }
            }
            else
            {
                    smoothSpeedL = lerp(smoothSpeedL, motorStop, speedSmoothing);
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
                    smoothSpeedR = lerp(smoothSpeedR, motorLow, speedSmoothing);
                    motor[rightMotor] = smoothSpeedR;
                }
            }
            else
            {
                    smoothSpeedR = lerp(smoothSpeedR, motorStop, speedSmoothing);
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
    while(SensorValue[leftEncoder] > ticks || SensorValue[rightEncoder] > ticks)
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
                    smoothSpeedL = lerp(smoothSpeedL, -motorLow, speedSmoothing);
                    motor[leftMotor] = smoothSpeedL;
                }
            }
            else
            {
                    smoothSpeedL = lerp(smoothSpeedL, -motorStop, speedSmoothing);
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
                    smoothSpeedR = lerp(smoothSpeedR, -motorLow, speedSmoothing);
                    motor[rightMotor] = smoothSpeedR;
                }
            }
            else
            {
                    smoothSpeedR = lerp(smoothSpeedR, motorStop, speedSmoothing);
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

        while(abs(SensorValue[leftEncoder]) < abs(totalLticks) || abs(SensorValue[rightEncoder]) < abs(totalRticks))
    {
        step++;
        if (step % 10 == 0)
        {
            changeDelta();
        }

            //Left Motor
        if (abs(SensorValue[leftEncoder]) < abs(totalLticks))
            {
                    smoothSpeedL = lerp(smoothSpeedL, (totalLticks > 0 ? targetSpeedL : -targetSpeedL), speedSmoothing);
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
                    smoothSpeedR = lerp(smoothSpeedR, (totalRticks > 0 ? targetSpeedR : -targetSpeedR), speedSmoothing);
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



    void moveForwardTime(int mSec, int speed)
    {
        motor(leftMotor) = speed;
        motor(rightMotor) = speed;
        wait1Msec(mSec);

        motor(leftMotor) = 0;
        motor(rightMotor) = 0;
    }

    void pointTurnTime(int mSec, int speed)
    {

    }

    void liftArmTime(float armMove, int mSec, float armHold, float openSpeed, float gripStrength)
    {
        motor[clawMotor] = openSpeed;
        wait1Msec(mSec);

        motor[clawMotor] = gripStrength;
        motor[armMotor] = armMove;
        wait1Msec(mSec);

        motor[clawMotor] = gripStrength;
        motor[armMotor] = armHold;

    }


void autonomous()
{
    //Move forward 9ft 0in
    moveForward(9, 0);
        pointTurn(85, 0);   // Degrees(+counter clockwise, TurnOffset(0 is pivot)

    //Arm Raise
	liftArmTime(50, 1000, 0, 0, 0);	// +-ArmSpeed, delay, ArmHoldSpeed, +-ClawSpeed, ClawHoldSpeed

    //Move forward 2ft 0in
    moveForward(2, 0);
        pointTurn(85, 0);   // Degrees(+counter clockwise, TurnOffset(0 is pivot)

     //Move forward 4ft 0in
    moveForward(4, 0);

    wait1Msec(3000);

/* Arm and Claw Functions
    //Arm Raise
	//liftArm(50, 1000, 0, 0, 0);	// +-ArmSpeed, delay, ArmHoldSpeed, +-ClawSpeed, ClawHoldSpeed

    //Open Claw
	//liftArm(0, 1000, 0, 50, 0);	// +-ArmSpeed, delay, ArmHoldSpeed, +-ClawSpeed, ClawHoldSpeed

    //Claw Grip and Arm Raise
	//liftArm(50, 2100, 0, -50, -30);	// +-ArmSpeed, delay, ArmHoldSpeed, +-ClawSpeed, ClawHoldSpeed

    //Relax Claw and Arm
	//liftArm(0, 0, 0, 0, 0);	// +-ArmSpeed, delay, ArmHoldSpeed, +-ClawSpeed, ClawHoldSpeed
*/
}


void tankFunct()
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

    //Arm
    {
        if (vexRT[Btn5U] && vexRT[Btn5D])	//UP and Down
        {
            motor[clawMotor] = 20;
        }
        else if (vexRT[Btn5U])
        {
            motor[clawMotor] = 65;
        }
        else if (vexRT[Btn5D])
        {
            motor[clawMotor] = -25;
        }
        else
        {
            motor[clawMotor] = 0;
        }
    }   //Arm

    //Claw6
    {
        if (closeCount > 2 * 1000/tps)  //seconds * time per iteration
        {
            clawClose = false;
            clawGrip = true;
        }
        else if (vexRT[Btn6D])	//UP and Down
        {
            clawClose = true;
            clawGrip = false;
            closeCount = 0;
        }
        else if (vexRT[Btn6U])
        {
            clawClose = false;
            clawGrip = false;
        }


        if (clawGrip)
        {
            motor[armMotor] = -25;
        }
        else if (clawClose)
        {
            motor[armMotor] = 40;
            closeCount++;
        }
        else if (vexRT[Btn6D])
        {
            motor[armMotor] = -40;
        }
        else
        {
            motor[armMotor] = 0;
        }
    }   //Claw
}	//End tankFunct()

void joystickFunct()
{
    motor[leftMotor] = 0;
    motor[rightMotor] = 0;
}

/*--------------------*\
|*			Task Main			*|
\*--------------------*/


task main()
{

	tank = true;
	joystick = false;

	while (true)
	{

		if (tank)
			tankFunct();
		/*else if (joystick)
			joystickFunct();*/

		if (vexRT[Btn8U] == 1)
		{
			//autonomous();

		}
        wait1Msec(1000/tps);
	}
}

		/*if (vexRT[Btn7D])
		{
			tank = true;
			joystick = false;
		}
		else if (vexRT[Btn8D])
		{
			joystick = true;
			tank = false;
		}*/