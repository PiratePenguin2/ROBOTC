#pragma config(Sensor, dgtl9,  leftEncoder,    sensorQuadEncoder)
#pragma config(Sensor, dgtl11, rightEncoder,   sensorQuadEncoder)
#pragma config(Motor,  port1,           leftMotor,     tmotorVex393_HBridge, openLoop, driveLeft, encoderPort, dgtl9)
#pragma config(Motor,  port2,           armMotor,      tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port3,           clawMotor,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port10,          rightMotor,    tmotorVex393_HBridge, openLoop, reversed, driveRight, encoderPort, dgtl11)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

const int tol = 5;
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



void moveForward(int distance)
{
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

void moveBackward(int distance)
{
	int ticks = (distance / (wheelDiam * PI)) * (encoderTicks);

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


//pointTurn			//(r +- offset/2) * (deg/360) * (1/(wheelDiam * PI)) * 360
void pointTurn(float deg, float offset)
{
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

void liftArm(int mSec)
{
	motor[armMotor] = 50;
	wait1Msec(mSec);
	motor[armMotor] = 0;
}


task main()
{
	//for (int i = 0; i < 1; i++)
	while (true)
	{
		if (vexRT[Ch2] >= 20 || vexRT[Ch2] <= -20){
			motor[leftMotor] = vexRT[Ch2];
		} else {
		motor[leftMotor] = 0;
		}

		if (vexRT[Ch3] >= 20 || vexRT[Ch3] <= -20){
			motor[rightMotor] = vexRT[Ch3];
		} else {
		motor[rightMotor] = 0;
		}

		if (vexRT[Btn8U] == 1)
		{
			moveForward( (14-4.5) + 6 + 11);
			liftArm(1800);
				pointTurn(85, 0);	//Positive is counter clockwise
			moveForward(30 -1);
				pointTurn(-90, 0);	//Positive is counter clockwise
			moveForward(11 + 18 - 9 +1);
				pointTurn(-90, 0);	//Positive is counter clockwise
			moveForward((9 + 7 -3));
			wait1Msec(3000);
			motor[armMotor] = 0;
		}
	}
}


			/*zero();
			moveForward(12);
			zero();
		  pointTurn(85, 0);	//Positive is counter clockwise

		  zero();
			moveForward(12);
			zero();
		  pointTurn(-85, 0);	//Positive is counter clockwise

		  zero();
			moveBackward(-12);
			zero();
		  pointTurn(85, 0);	//Positive is counter clockwise

		  zero();
			moveBackward(-10);
			zero();
		  pointTurn(-85, 0);	//Positive is counter clockwise*/
