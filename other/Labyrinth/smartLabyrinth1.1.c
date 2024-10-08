#pragma config(Sensor, dgtl9,  leftEncoder,    sensorQuadEncoder)
#pragma config(Sensor, dgtl11, rightEncoder,   sensorQuadEncoder)
#pragma config(Motor,  port1,           leftMotor,     tmotorVex393_HBridge, PIDControl, encoderPort, dgtl9)
#pragma config(Motor,  port10,          rightMotor,    tmotorVex393_HBridge, PIDControl, reversed, encoderPort, dgtl11)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

// Constants for controlling the speed and measurements
const int fullSpeed = 127;
const float wheelDiameter = 4; // Diameter of the wheel in inches










// Function to smoothly move the robot forward or backward a certain distance in inches
void MoveInches(int inches) {
    float wheelCircumference = wheelDiameter * PI; // Calculate the wheel circumference

    // Convert inches to encoder ticks
    int target = inches * 360 / wheelCircumference;

    // Reset the encoders
    SensorValue[leftEncoder] = 0;
    SensorValue[rightEncoder] = 0;

    // Calculate the distances for acceleration, constant speed, and deceleration
    int accDistance = target / 4;
    int decDistance = accDistance; // Deceleration distance is the same as acceleration

    // Acceleration phase
    for (int i = 0; i < accDistance; i++) {
        int power = fullSpeed * i * 4 / accDistance;
        motor[leftMotor] = power;
        motor[rightMotor] = power;
        wait1Msec(1);
    }

    // Constant speed phase
    for (int i = 0; i < accDistance * 2; i++) {
        motor[leftMotor] = fullSpeed;
        motor[rightMotor] = fullSpeed;
        wait1Msec(1);
    }

    // Deceleration phase
    for (int i = 0; i < decDistance; i++) {
        int power = fullSpeed - (fullSpeed * i * 4 / decDistance);
        motor[leftMotor] = power;
        motor[rightMotor] = power;
        wait1Msec(1);
    }

    // Stop the motors
    motor[leftMotor] = 0;
    motor[rightMotor] = 0;
}

// Function to rotate the robot a certain number of degrees
void PivotDegrees(int degrees) {
    float wheelCircumference = wheelDiameter * PI; // Calculate the wheel circumference

    // Calculate the distance each wheel should travel to make the turn
    float wheelBase = 12; // Distance between the wheels in inches
    float distanceToTravel = wheelBase * abs(degrees) * PI / 360; // Convert degrees to radians

    // Convert the distance to encoder ticks
    int target = distanceToTravel * 360 / wheelCircumference;

    // Reset the encoders
    SensorValue[leftEncoder] = 0;
    SensorValue[rightEncoder] = 0;

    // Calculate the distances for acceleration, constant speed, and deceleration
    int accDistance = target / 4;
    int decDistance = accDistance; // Deceleration distance is the same as acceleration

    // Acceleration phase
    for (int i = 0; i < accDistance; i++) {
        int power = fullSpeed * i * 4 / accDistance;
        int leftPower = (degrees > 0) ? power : -power;
        int rightPower = -leftPower;
        motor[leftMotor] = leftPower;
        motor[rightMotor] = rightPower;
        wait1Msec(1);
    }

    // Constant speed phase
    for (int i = 0; i < accDistance * 2; i++) {
        int leftPower = (degrees > 0) ? fullSpeed : -fullSpeed;
        int rightPower = -leftPower;
        motor[leftMotor] = leftPower;
        motor[rightMotor] = rightPower;
        wait1Msec(1);
    }

    // Deceleration phase
    for (int i = 0; i < decDistance; i++) {
        int power = fullSpeed - (fullSpeed * i * 4 / decDistance);
        int leftPower = (degrees > 0) ? power : -power;
        int rightPower = -leftPower;
        motor[leftMotor] = leftPower;
        motor[rightMotor] = rightPower;
        wait1Msec(1);
    }

    // Stop the motors
    motor[leftMotor] = 0;
    motor[rightMotor] = 0;
}

task main() {
    while (true) {
        if (vexRT[Btn7U] == 1) {
            MoveInches(24);  // Move forward 12 inches
            wait1Msec(500);  // Wait for 0.5 seconds
            PivotDegrees(360);  // Rotate 90 degrees clockwise
            wait1Msec(500);  // Wait for 0.5 seconds
            MoveInches(-24);  // Move backward 12 inches
            wait1Msec(500);  // Wait for 0.5 seconds
            PivotDegrees(-360);  // Rotate 90 degrees counterclockwise
            wait1Msec(500);  // Wait for 0.5 seconds
        }
    }
}
