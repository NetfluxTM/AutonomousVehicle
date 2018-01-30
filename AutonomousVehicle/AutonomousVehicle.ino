/*
 Name:		AutonomousVehicle.ino
 Author:	Justin Champagne
 
 Sections:
	Renaming Pins:					pretty self explanatory, labels are given to the pins on the Arduino UNO so when working with them in the functions, it's less confusing.
	Function Prototypes:			function definitions are below void loop() [AKA Main()], purely housekeeping so main code is closest to the top.
	void setup():					required function, automatically called when arduino turns on. Use to initialize variables, pin modes, etc.
	void loop():					required function, where the main code will be written. Once it reaches the end of the function, will loop, endlessly.
	
	Function Definitions:			below sections are where the code for function prototypes are written. 
	Motor Functions:				Functions controlling the motors. Moving forward/backward, accelerating, decelerating, turning, stopping, etc..
	Line Follower Functions:		read input from line following ir sensors (LFIRS), make decision based on the values read (ie: turning left if right LFIRS detects black, etc.)
	Obstacle Detection Functions:	read input from ultrasonic sensor, stop when sensor detects object
	Experimental Testing:			random miscellaneous functions to test features and ideas


 Comments:
	Technically the range for motor speed is 0-255, however anything below around 80 or so won't allow the motors to spin the tires, depending on the load/resistance.

 Todo:
	Check if left / right turn are actually left and right
	find max/min values for motor speeds. Check if: Max 255, Min 59 w/o load (sideways) and 80 w/ load (resting normally on ground)
	adjust max/min values for accelerating/decelerating (if speed < 80 then set speed to 80, if speed > 255 set speed to 255, if in middle run regular for loop)
	decide if motor modes need to be set for accelerate / decelerate. If mode is set to turn left or reverse when accelerate is called, will it run as expected?

*/
// --------------------------------------------------Renaming Pins-------------------------------------------------- //
// Connecting motor controller (H-Bridge L298N) pins to Arduino digital pins
// leftForward - rightReverse used to control direction of both motors, and break / coasting modes
// these names given because they are the labels in the H-Bridge tutorial (https://tronixlabs.com.au/news/tutorial-l298n-dual-motor-controller-module-2a-and-arduino)

// Left Motor
int leftMotorSpeed = 5; // variable speed control
int leftForward = 7;  // mode switch
int leftReverse = 6;

// Right Motor
int rightMotorSpeed = 10;  // variable speed control
int rightForward = 9;  
int rightReverse = 8;



// IR Sensors
int LeftLineIR = 3;			//(ASSUMING PIN SET TO 0)  Insert pin# for left IR line follower sensor      
int MidLeftLineIR = 4;		//(ASSUMING PIN SET TO 1)  Insert pin # for middle-left IR line follower sensor  
int MidRightLineIR = 11	;	//(ASSUMING PIN SET TO 1)  Insert pin # for middle-left IR line follower sensor  
int RightLineIR = 12;		//(ASSUMING PIN SET TO 2)  Insert pin # for right IR line follower sensor     

// Optointerrupter: IR Sensors attached to spokes of axle to measure distance
int LeftOptointerrupter = 0;  //(ASSUMING PIN SET TO 11) Insert pin # for left optointerrupter attached to spokes of left axle           
int RightOptointerrupter = 1;  //(ASSUMING PIN SET TO 12) Insert pin # for right optointerrupter attached to spokes of right axle         

// Ultrasonic
// -- int Ultrasonic = ?? How to set up pinout into arduino for ultrasonic sensor (wall-e eyes)






// --------------------------------------------------Function Prototypes-------------------------------------------------- //
void setup();

void forwardDrive(int speed, int time);
void reverseDrive(int speed, int time);
void leftTireForward(int speed, int time);
void rightTireForward(int speed, int time);
void leftTireBackward(int speed, int time);
void rightTireBackward(int speed, int time);

void stop();
void speedStop();
void motorOff();

int accelerate(int targetSpeed, int time);
int decelerate(int currentSpeed, int time);
int spinCW(int speed, int time);
int spinCCW(int speed, int time);

void followLine();

void negativesTest();
void speedValuesTest();
void movementTest();
void dance();

// --------------------------------------------------void setup()-------------------------------------------------- //

void setup()
{
	// put your setup code here, to run once:
	Serial.begin(9600);
	//delay(3000);


	// setting motor controls to output
	pinMode(leftMotorSpeed, OUTPUT);
	pinMode(rightMotorSpeed, OUTPUT);
	pinMode(leftForward, OUTPUT);
	pinMode(leftReverse, OUTPUT);
	pinMode(rightForward, OUTPUT);
	pinMode(rightReverse, OUTPUT);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------ //
// ----------------------------------------------------------------------void loop()--------------------------------------------------------------------- //
// ------------------------------------------------------------------------------------------------------------------------------------------------------ //
void loop()
{	// put your main code here, to run repeatedly:
	int speed = 255; // range 70-255
	int time = 0; // length of time in milliseconds


	//for (int i = 47; i < 256; i++) {
	//	Serial.println(i);
	//	forwardDrive(i);
	//	delay(3000);
	//}
	
	spinCW(250, 0);
	//delay(1000);
	//spinCCW(250, 1000);
	//delay(1000);
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------ //
// -----------------------------------------------------------------Function Definitions----------------------------------------------------------------- //
// ------------------------------------------------------------------------------------------------------------------------------------------------------ //


// ----------------------------------------Motor Functions-------------------------------------------- //

// move forward at the given speed for the given amount of time
// speed: range 70-255
// time: in milliseconds. Will stop afterwards. Using 0 will return without stopping
void forwardDrive(int speed, int time)
{
	// enable mode for left motor
	digitalWrite(leftForward, HIGH);
	digitalWrite(leftReverse, LOW);
	// enable mode for right motor
	digitalWrite(rightForward, HIGH);
	digitalWrite(rightReverse, LOW);
	// set speed out of possible range of 0-255 for left motor
	analogWrite(leftMotorSpeed, speed);
	// set speed out of possible range of 0-255 for right motor
	analogWrite(rightMotorSpeed, speed);

	// stops moving after the defined amount of time
	// if time is set to 0, will not stop
	if (time != 0)
	{
		delay(time);
		stop();
	}
}

// move backward at the given speed for the given amount of time
// speed: range 70-255
// time: in milliseconds. Will stop afterwards. Using 0 will return without stopping
void reverseDrive(int speed, int time)
{
	// enable mode for left motor
	digitalWrite(leftForward, LOW);
	digitalWrite(leftReverse, HIGH);
	// enable mode for right motor
	digitalWrite(rightForward, LOW);
	digitalWrite(rightReverse, HIGH);
	// set speed out of possible range of 0-255 for left motor
	analogWrite(leftMotorSpeed, speed);
	// set speed out of possible range of 0-255 for right motor
	analogWrite(rightMotorSpeed, speed);

	// stops moving after the defined amount of time
	// if time is set to 0, will not stop
	if (time != 0)
	{
		delay(time);
		stop();
	}
}

// moves only the left tire forward at the given speed for the given amount of time
// speed: range 70-255
// time: in milliseconds. Will stop afterwards. Using 0 will return without stopping
void leftTireForward(int speed, int time)
{
	// enable mode for left motor
	digitalWrite(leftForward, HIGH);
	digitalWrite(leftReverse, LOW);
	// set speed out of possible range of 0-255 for left motor
	analogWrite(leftMotorSpeed, speed);
	// disable mode for right motor
	digitalWrite(rightForward, LOW);
	digitalWrite(rightReverse, LOW);
	// set speed to 0 out of possible range of 0-255 for right motor
	analogWrite(rightMotorSpeed, 0);

	// stops moving after the defined amount of time
	// if time is set to 0, will not stop
	if (time != 0)
	{
		delay(time);
		stop();
	}
}

// moves only the right tire forward at the given speed for the given amount of time
// speed: range 70-255
// time: in milliseconds. Will stop afterwards. Using 0 will return without stopping
void rightTireForward(int speed, int time)
{
	// disable mode for left motor
	digitalWrite(leftForward, LOW);
	digitalWrite(leftReverse, LOW);
	// set speed to 0 out of possible range of 0-255 for left motor
	analogWrite(leftMotorSpeed, 0);
	// enable mode for right motor
	digitalWrite(rightForward, HIGH);
	digitalWrite(rightReverse, LOW);
	// set speed out of possible range of 0-255 for right motor
	analogWrite(rightMotorSpeed, speed);

	// stops moving after the defined amount of time
	// if time is set to 0, will not stop
	if (time != 0)
	{
		delay(time);
		stop();
	}
}

// moves only the left tire backward at the given speed for the given amount of time
// speed: range 70-255
// time: in milliseconds. Will stop afterwards. Using 0 will return without stopping
void leftTireBackward(int speed, int time)
{
	// enable mode for left motor
	digitalWrite(leftForward, LOW);
	digitalWrite(leftReverse, HIGH);
	// set speed out of possible range of 0-255 for left motor
	analogWrite(leftMotorSpeed, speed);
	// disable mode for right motor
	digitalWrite(rightForward, LOW);
	digitalWrite(rightReverse, LOW);
	// set speed to 0 out of possible range of 0-255 for right motor
	analogWrite(rightMotorSpeed, 0);

	// stops moving after the defined amount of time
	// if time is set to 0, will not stop
	if (time != 0)
	{
		delay(time);
		stop();
	}
}

// moves only the right tire backward at the given speed for the given amount of time
// speed: range 70-255
// time: in milliseconds. Will stop afterwards. Using 0 will return without stopping
void rightTireBackward(int speed, int time)
{
	// disable mode for left motor
	digitalWrite(leftForward, LOW);
	digitalWrite(leftReverse, LOW);
	// set speed to 0 out of possible range of 0-255 for left motor
	analogWrite(leftMotorSpeed, 0);
	// enable mode for right motor
	digitalWrite(rightForward, LOW);
	digitalWrite(rightReverse, HIGH);
	// set speed out of possible range of 0-255 for right motor
	analogWrite(rightMotorSpeed, speed);

	// stops moving after the defined amount of time
	// if time is set to 0, will not stop
	if (time != 0)
	{
		delay(time);
		stop();
	}
}




// disables and sets speed to 0 for both motors
void stop()
{
	// set speed to 0 for both motors
	analogWrite(leftMotorSpeed, 0);
	analogWrite(rightMotorSpeed, 0);
	//disable mode for both motors
	digitalWrite(leftForward, LOW);
	digitalWrite(leftReverse, LOW);
	digitalWrite(rightForward, LOW);
	digitalWrite(rightReverse, LOW);
}

// sets motor speeds to 0, but leaves mode in previous setting. this may cause issues
void speedStop()
{
	analogWrite(leftMotorSpeed, 0);
	analogWrite(rightMotorSpeed, 0);
}

// sets mode for both motors to off, but leaves speed at previous setting. This may cause issues
void motorOff()
{
	digitalWrite(leftForward, LOW);
	digitalWrite(leftReverse, LOW);
	digitalWrite(rightForward, LOW);
	digitalWrite(rightReverse, LOW);
}



// time defines intervals in milliseconds between incrementing the speed. Returns current speed
int accelerate(int targetSpeed, int time)
{
	for(int i = 0; i <= targetSpeed; i++)
	{
		analogWrite(leftMotorSpeed, i);
		analogWrite(rightMotorSpeed, i);
		delay(time);
	}
	return targetSpeed; // this is now the current speed
}

// current speed set to make sure movement isn't jumpy. Time defines intervals in milliseconds between incrementing the speed
int decelerate(int currentSpeed, int time)
{
	for (int i = currentSpeed; i >= 0; --i)
	{
		analogWrite(leftMotorSpeed, i);
		analogWrite(rightMotorSpeed, i);
		delay(time);
	}

	motorOff();
	return 0; // this is now the current speed, 0, we are assuming nothing will go wrong
}

// spins clockwise at the given strength for the given amount of time
int spinCW(int speed, int time)
{
	// enable mode for left motor
	digitalWrite(leftForward, HIGH);
	digitalWrite(leftReverse, LOW);
	// set speed out of possible range of 0-255 for left motor
	analogWrite(leftMotorSpeed, speed);
	// enable mode for right motor
	digitalWrite(rightForward, LOW);
	digitalWrite(rightReverse, HIGH);
	// set speed out of possible range of 0-255 for right motor
	analogWrite(rightMotorSpeed, speed);

	// stops moving after the defined amount of time
	// if time is set to 0, will not stop
	if (time != 0)
	{
		delay(time);
		stop();
	}
}

// spins counterclockwise at the given strength for the given amount of time
int spinCCW(int speed, int time)
{
	// enable mode for left motor
	digitalWrite(leftForward, LOW);
	digitalWrite(leftReverse, HIGH);
	// set speed out of possible range of 0-255 for left motor
	analogWrite(leftMotorSpeed, speed);
	// enable mode for right motor
	digitalWrite(rightForward, HIGH);
	digitalWrite(rightReverse, LOW);
	// set speed out of possible range of 0-255 for right motor
	analogWrite(rightMotorSpeed, speed);

	// stops moving after the defined amount of time
	// if time is set to 0, will not stop
	if (time != 0)
	{
		delay(time);
		stop();
	}
}

// ------------------------------------- Line Follower Functions --------------------------------------------- //

void followLine()
{
	if (digitalRead(LeftLineIR) == HIGH) {
		Serial.println("LeftLineIR is HIGH");
	}
	else if (digitalRead(LeftLineIR) == LOW) {
		Serial.println("LeftLineIR is LOW");
	}


	if (digitalRead(MidLeftLineIR) == HIGH) {
		Serial.println("MidLeftLineIR is HIGH");
	}
	else if (digitalRead(MidLeftLineIR) == LOW) {
		Serial.println("MidLeftLineIR is LOW");
	}


	if (digitalRead(MidRightLineIR) == HIGH) {
		Serial.println("MidRightLineIR is HIGH");
	}
	else if (digitalRead(MidRightLineIR) == LOW) {
		Serial.println("MidRightLineIR is LOW");
	}


	if (digitalRead(RightLineIR) == HIGH) {
		Serial.println("RightLineIR is HIGH");
	}
	else if (digitalRead(RightLineIR) == LOW) {
		Serial.println("RightLineIR is LOW");
	}

}

// ------------------------------------- Obstacle Detection Functions --------------------------------------------- //






// ------------------------------------- Experimental Testing --------------------------------------------- //

// Testing to see what happens if negative values are passed in for speed
void negativesTest()
{
	reverseDrive(-90, 0);
	delay(250);
	stop();
	delay(500);
	reverseDrive(90, 0);
	delay(250);
	stop();
	delay(500);

	forwardDrive(-90, 0);
	delay(250);
	stop();
	delay(500);
	forwardDrive(90, 0);
	delay(250);
	stop();
	
	delay(1000);

	reverseDrive(-160, 0);
	delay(250);
	stop();
	delay(500);
	reverseDrive(160, 0);
	delay(250);
	stop();
	delay(500);

	forwardDrive(-160, 0);
	delay(250);
	stop();
	delay(500);
	forwardDrive(160, 0);
	delay(250);
	stop();
	delay(500);
}

// testing what effect different values of speed will have
void speedValuesTest()
{
	reverseDrive(70, 0);
	delay(250);
	stop();
	delay(500);
	reverseDrive(255, 0);
	delay(250);
	stop();
	delay(500);

	forwardDrive(70, 0);
	delay(250);
	stop();
	delay(500);
	forwardDrive(255, 0);
	delay(250);
	stop();

	delay(1000);

	reverseDrive(160, 0);
	delay(250);
	stop();
	delay(500);
	reverseDrive(160, 0);
	delay(250);
	stop();
	delay(500);

	forwardDrive(160, 0);
	delay(250);
	stop();
	delay(500);
	forwardDrive(160, 0);
	delay(250);
	stop();
	delay(500);
}

// testing all the movement functions to make sure they work properly
void movementTest()
{
	forwardDrive(255, 1000);
	delay(1000);

	reverseDrive(255, 1000);
	delay(1000);

	leftTireForward(255, 1000);
	delay(1000);

	rightTireForward(255, 1000);
	delay(1000);

	leftTireBackward(255, 1000);
	delay(1000);

	rightTireBackward(255, 1000);
	delay(1000);

	spinCW(255, 1000);
	delay(1000);

	spinCCW(255, 1000);
	delay(1000);
}

void dance()
{
	for (int i = 0; i < 25; i++) {
	
		for (int j = 0; j < 3; j++) {
			leftTireForward(255, 100);
			rightTireBackward(255, 100);
			leftTireBackward(255, 150);
			rightTireForward(255, 150);
		}
		spinCW(255, 1000);
		spinCCW(255, 1000);

	}

}