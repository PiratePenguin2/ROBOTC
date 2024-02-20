#pragma config(Sensor, dgtl1,  leftEncoder,    sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  rightEncoder,   sensorQuadEncoder)
#pragma config(Sensor, dgtl5,  armPotent,      sensorRotation)
#pragma config(Motor,  port1,           leftMotor,     tmotorVex393_HBridge, PIDControl, driveLeft, encoderPort, dgtl1)
#pragma config(Motor,  port2,           armMotor,      tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,           clawMotor,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port10,          rightMotor,    tmotorVex393_HBridge, PIDControl, reversed, driveRight, encoderPort, dgtl3)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

float wheelDiameter = 4.0;
float ticksPerInch = 360 / (wheelDiameter * 3.14);

// Function prototypes
void moveInches(float inches, int targetSpeed, float totalDuration);
void pivotTurn(int degrees, int targetSpeed, float totalDuration);



task main() {
    float distanceToMove = 24.0;
    int targetSpeed = 50;
    float totalDuration = 4.0; // Total duration in seconds for acceleration, constant speed, and deceleration

    moveInches(distanceToMove, targetSpeed, totalDuration);
    wait1Msec(1000); // Wait for 1 second between movements
    pivotTurn(90, targetSpeed, totalDuration);
}





void moveInches(float inches, int targetSpeed, float totalDuration) {
    int targetTicks = inches * ticksPerInch;
    SensorValue[leftEncoder] = 0;
    SensorValue[rightEncoder] = 0;

    int currentTicks = 0;
    int currentSpeed = 0;
    float timeElapsed = 0;

    float accelerationTime = totalDuration / 4; // Divide total time into quarters

    while (abs(SensorValue[leftEncoder]) < targetTicks && abs(SensorValue[rightEncoder]) < targetTicks) {
        float ratio = ((float)abs(currentTicks)) / targetTicks;

        if (timeElapsed < accelerationTime) {
            currentSpeed = (int)(targetSpeed * (timeElapsed / accelerationTime)); // Accelerate during the first quarter
        } else if (timeElapsed < accelerationTime * 3) {
            currentSpeed = targetSpeed; // Maintain constant speed during the second and third quarters
        } else {
            float decelerationTime = totalDuration - accelerationTime * 3;
            float timeRemaining = timeElapsed - accelerationTime * 3;
            currentSpeed = (int)(targetSpeed * (1 - timeRemaining / decelerationTime)); // Decelerate during the fourth quarter
        }

        motor[leftMotor] = currentSpeed;
        motor[rightMotor] = currentSpeed;

        currentTicks = SensorValue[leftEncoder];
        timeElapsed += 0.1;
        wait1Msec(100);
    }

    motor[leftMotor] = 0;
    motor[rightMotor] = 0;
}

void pivotTurn(int degrees, int targetSpeed, float totalDuration) {
    float turningRadius = 7.0; // Adjust the turning radius based on the robot's dimensions
    float wheelbase = 10.0; // Adjust the wheelbase based on the robot's dimensions
    float radians = degrees * 3.14 / 180.0;
    float targetDistance = radians * (turningRadius + wheelbase / 2);

    SensorValue[leftEncoder] = 0;
    SensorValue[rightEncoder] = 0;

    int currentTicks = 0;
    int currentSpeed = 0;
    float timeElapsed = 0;

    float accelerationTime = totalDuration / 4; // Divide total time into quarters

    while (abs(SensorValue[leftEncoder]) < abs(targetDistance) && abs(SensorValue[rightEncoder]) < abs(targetDistance)) {
        float ratio = ((float)abs(currentTicks)) / abs(targetDistance);

        if (timeElapsed < accelerationTime) {
            currentSpeed = (int)(targetSpeed * (timeElapsed / accelerationTime)); // Accelerate during the first quarter
        } else if (timeElapsed < accelerationTime * 3) {
            currentSpeed = targetSpeed; // Maintain constant speed during the second and third quarters
        } else {
            float decelerationTime = totalDuration - accelerationTime * 3;
            float timeRemaining = timeElapsed - accelerationTime * 3;
            currentSpeed = (int)(targetSpeed * (1 - timeRemaining / decelerationTime)); // Decelerate during the fourth quarter
        }

        if (degrees > 0) {
            motor[leftMotor] = currentSpeed;
            motor[rightMotor] = -currentSpeed;
        } else {
            motor[leftMotor] = -currentSpeed;
            motor[rightMotor] = currentSpeed;
        }

        currentTicks = SensorValue[leftEncoder];
        timeElapsed += 0.1;
        wait1Msec(100);
    }

    motor[leftMotor] = 0;
    motor[rightMotor] = 0;
}
