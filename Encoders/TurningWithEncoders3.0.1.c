
int step = 0;

int ticksLI = 0; //just a variable to store which tick we're on (for the left side)
int ticksLF = 0;

int ticksRI = 0;
int ticksRF = 0;

int speedL = 0;
int speedR = 0;

void changeDelta()
{
	// speed (angular velocity, they call it) = ticks final - ticks initial
	speedL = ticksLF - ticksRI;
}

task main()
{
	step = 0;

	// ticks final = current # of ticks in wheel
	ticksLF = SensorValue[leftEncoder];
	ticksRF = SensorValue[rightEncoder];

	// if step % 100 == 0 (every 100 steps i guess): call change delta variable function
	step += 1;
	if (step % 10000 == 0)
	{
		changeDelta();
	}

	// ticks initial = ticks final
	ticksLI = ticksLF;
	ticksRI = ticksRF;

	// uh
}
