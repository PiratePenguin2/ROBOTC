#pragma config(Sensor, dgtl1,  armLoop,        sensorDigitalOut)
#pragma config(Sensor, dgtl3,  btnRequest,     sensorDigitalOut)
#pragma config(Sensor, dgtl8,  armSensor,      sensorRotation)
#pragma config(Sensor, dgtl12, systemStatus,   sensorDigitalOut)
#pragma config(Motor,  port1,           leftMotor,     tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port8,           armMotor,      tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port9,           clawMotor,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port10,          rightMotor,    tmotorVex393_HBridge, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//bool buttons = true;

bool loopingAllow = false;
bool loopDone = true;

bool allowPush = true;
int pushDelay = 1000;


void Move()
{
	loopDone = false;
	motor[armMotor] = 10;
	wait1Msec (2000);

	motor[armMotor] = 30;
	wait1Msec (1800);
	motor[armMotor] = 10;
	wait1Msec (1000);

	motor[armMotor] = -10;
	wait1Msec (800);
	motor[armMotor] = 0;

	SensorValue[armLoop] = 1;	//Green OFF
	loopDone = true;
}

void BtnPushDelay()
{
	SensorValue[btnRequest] = 0;	//Yellow ON
	allowPush = false;
	wait1Msec(pushDelay);
	allowPush = true;
}


/*---------- MAIN ----------*/

task main()
{
	SensorValue[systemStatus] = 0;	//Red ON
	SensorValue[btnRequest] = 1;	//Yellow OFF
	SensorValue[armLoop] = 1;	//Green OFF
	while (true)
	{
		if (loopDone && loopingAllow)
			{
				SensorValue[armLoop] = 0;	//Green ON
				SensorValue[btnRequest] = 1;	//Yellow OFF
				Move();
			}


		if (vexRT[Btn5U] == 1 || vexRT[Btn5D] == 1 ||

				vexRT[Btn6U] == 1 || vexRT[Btn6D] == 1 ||

				vexRT[Btn7U] == 1 || vexRT[Btn7D] == 1 ||
				vexRT[Btn7L] == 1 || vexRT[Btn7R] == 1 ||

				vexRT[Btn8U] == 1 || vexRT[Btn8D] == 1 ||
				vexRT[Btn8L] == 1 || vexRT[Btn8R] == 1   )
		{
			SensorValue[btnRequest] = 0;	//Yellow ON
			if (allowPush)
			{
				BtnPushDelay();
				loopingAllow = !loopingAllow;
			}
		}
		else
		{
			SensorValue[btnRequest] = 1;	//Yellow ON
		}



	} //While
}	//Main
