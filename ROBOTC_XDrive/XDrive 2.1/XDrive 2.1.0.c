#pragma config(Sensor, dgtl1,  NEencoder,      sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  SEencoder,      sensorQuadEncoder)
#pragma config(Sensor, dgtl5,  SWencoder,      sensorQuadEncoder)
#pragma config(Sensor, dgtl7,  NWencoder,      sensorQuadEncoder)
#pragma config(Sensor, dgtl9,  ESTOPbtn,       sensorTouch)
#pragma config(Sensor, dgtl10, ,               sensorLEDtoVCC)
#pragma config(Sensor, dgtl11, lampENABLE,     sensorLEDtoVCC)
#pragma config(Sensor, dgtl12, lampESTOP,      sensorLEDtoVCC)
#pragma config(Motor,  port3,           NE,            tmotorVex393_MC29, openLoop, encoderPort, dgtl1)
#pragma config(Motor,  port4,           SE,            tmotorVex393_MC29, openLoop, encoderPort, dgtl3)
#pragma config(Motor,  port5,           SW,            tmotorVex393_MC29, openLoop, encoderPort, dgtl5)
#pragma config(Motor,  port6,           NW,            tmotorVex393_MC29, openLoop, encoderPort, dgtl7)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

const float tps = 10;

/*----------------*\
|*			Main			*|
\*----------------*/

task main()
{
	while	(true)
	{
		motor[NE] = 50;
		motor[SE] = 50;
		motor[SW] = 50;
		motor[NW] = 50;

		wait1Msec(1000 / tps);	//Msec per tick, inverse of ticks per second
	}

}
