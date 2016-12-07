//Despite it being .c this file is not a C file it is a RobotC file and has
//slightly different syntax
/*
GROUP 6
primary robot brick code for project 3
*/

#pragma config(Sensor, S1,     readerSensor,   sensorEV3_Color, modeEV3Color_Ambient)
#pragma config(Sensor, S2,     sonarSensor,    sensorEV3_Ultrasonic)
#pragma config(Sensor, S3,     leftDownLightSensor, sensorEV3_Color)
#pragma config(Sensor, S4,     rightDownLightSensor, sensorEV3_Color)
#pragma config(Motor,  motorA,          rightWheel,    tmotorEV3_Large, PIDControl, driveLeft, encoder)
#pragma config(Motor,  motorD,          leftWheel,     tmotorEV3_Large, PIDControl, driveRight, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#define IF_DEATH hunger < 0
#define IF_BACKUP bumperValue
#define IF_ESCAPE (upLight) && hunger > 25 && fear > 0
#define IF_SONAR (distance < MAX_DISTANCE && hunger > 25) || isTurning
#define IF_FEED (hunger < 50 && (leftDownLight || rightDownLight)) || foodFound
#define IF_WANDER 1

//constants
#define DEFAULT_PRIORITY 7
#define FEAR_MAX 100
#define ALPHA 0.3
#define MAX_DISTANCE 92
#define DEFAULT_SPEED 30
#define MAX_SPEED 80
#define STOPPING_DISTANCE 10
#define TIME_CON 100
//end constants

//states
enum states{
		DEATH = 0,
		BACKUP = 1,
		ESCAPE = 2,
		SONAR = 3,
		FEED = 4,
		WANDER = 5
};
//end states

//values
int feeding;
int hunger = 100;
int fear;
int gotScared;
int bumperValue = 0;
int upLight = 0;
int flag = 0;
int distance;
int isTurning = 0;
//end values

// Default weight co-efficient for light weighted averages
#define WEIGHT .8


//Globals for use with Feeding/Gradient
bool leftDownLight = false;
bool rightDownLight = false;
bool lastTurnLeft = false;
bool onFood = false;
bool foodFound = false;

float avgLightLeft = 0;
float avgLightRight = 0;
//End Feeding/Gradient Globals

//the current state
states state;
//end the current state

//prototypes
void switchTask(states newState);
void killProgram();
void approachObject();
void reverseAndTurn();
void pivot(int dir);
bool checkForObject();
void runAway();

//behavior based tasks
task death(){
	onFood = false;
	playTone(1000, 50);
	killProgram();
	while(1);
}

// Bumper detection
task backup(){
	onFood = false;
	foodFound = false;
	upLight = 0;
	isTurning = 0;
	flag = 0;
	//backup a bit
	setMotorSpeed(motorA, -40);
	setMotorSpeed(motorD, -40);
	sleep(600);
	if(bumperValue == 1){
		//turn left
		setMotorSpeed(motorA, 0);
		setMotorSpeed(motorD, DEFAULT_SPEED);
		sleep(200);
	}
	else if (bumperValue == 2) {
		//turn right
		setMotorSpeed(motorA, DEFAULT_SPEED);
		setMotorSpeed(motorD, 0);
		sleep(200);
	}
	else {
		setMotorSync(leftWheel, rightWheel, 0, -40;
		sleep(1000);
		setMotorSync(leftWheel, rightWheel, 0,0;
		playImmediateTone(500, 50);
		sleep(1000);
		pivot(rand()%2);
	}
	sleep(500);
	sleep(500);
	bumperValue = 0;
	while(1);
}

//Task that runs when the upper light sensor is triggered
task escape(){
	onFood = false;
	foodFound = false;
	isTurning = 0;
	gotScared = 25;
	playImmediateTone(500, 50);
	if(upLight){
		flag = 1;

	}
	reverseAndTurn();
	runAway();
	fear -= 25;
	while(1)
	upLight = 0;
}

//Detects if an object if withing three feet of front
task sonar(){
	onFood = false;
	foodFound = false;
	isTurning = 0;
	repeat(forever){
		if(checkForObject()){
			approachObject();
		}
		sleep(10);
	}

	isTurning = 0;
}

//Gets weighted average for gradient following
int getWeightedAverage(bool left) {
  if (left) {
  	int newInput = SensorValue[leftDownLightSensor];
	  avgLightLeft = newInput + WEIGHT * (avgLightLeft - newInput);
	  return avgLightLeft;
  }else {
  	int newInput = SensorValue[rightDownLightSensor];
    avgLightRight = newInput + WEIGHT * (avgLightRight - newInput);
    return avgLightRight;
  }
}

//Sets the motor speeds
void setSpeeds(int leftSpeed, int rightSpeed) {
  motor[leftWheel] = leftSpeed;
  motor[rightWheel] = rightSpeed;
}

//Task for feeding
task feed(){
	onFood = false;
	bool finished = false;
	time1[T3] = 0
	isTurning = 0;
	foodFound = false;
	while (!foodFound) {

		if (leftDownLight && rightDownLight) {
			foodFound = true;
		}
		sleep(1);
	}
	playTone(440, 50);
	int timeslice = 500001;
	int leftWander = 0;
	int rightWander = 0;
	while (hunger < 100) {
			if (!lastTurnLeft) {
				setLEDColor(ledGreen);
				leftWander = DEFAULT_SPEED / 4
				rightWander = (DEFAULT_SPEED + 10) / 4
			} else {
			setLEDColor(ledOrange);
				rightWander = DEFAULT_SPEED / 4
				leftWander = (DEFAULT_SPEED + 10) / 4
			}

	  int leftSpeed = 0;
	  int rightSpeed = 0;

	  bool initState = onFood;

	  if (leftDownLight && rightDownLight) {

	    onFood = true;
	  } else {

	    onFood = false;
	  }

	  if (onFood != initState) {
	    time1[T3] = 0;
	  }

	  if (onFood && hunger < 100) {
	    setSpeeds(leftWander, rightWander);

	  } else if (!onFood && time1[T3] > 500) {
	    //TURN 180
	  	timeslice = 50001;

	  	setLEDColor(ledRed);
	  	if (lastTurnLeft) {
	  		setSpeeds(10, -10);
	  	} else {
	  		setSpeeds(-10, 10);
	  	}
	    sleep(600);
	  } else if (!onFood && (leftDownLight || rightDownLight)) {
	  	//TURN TO GRADIENT
	  	setLEDColor(ledOrange);
	  	leftSpeed = 0;
	  	rightSpeed = 0;
	  	if (leftDownLight) {
	  		rightSpeed = 50;
	  		lastTurnLeft = false;
	  	}
	  	if (rightDownLight) {
	  		lastTurnLeft = true;
	  		leftSpeed = 50;
	  	}
	    setSpeeds(leftSpeed, rightSpeed);
	  }
	  if (onFood && hunger >= 98 && !finished) {
	  	playTone(220, 240);
	  	finished = true;
		}
	  timeslice++;
	  sleep(10);
	}
	foodFound = false;
	onFood = false;
}

task wander(){
	onFood = false;

	isTurning = 0;
	setMotorSync(rightWheel, leftWheel, 0, DEFAULT_SPEED);
	int time, speed, wheel;
	while(1){
		time = rand()%3;
		speed = rand()%15 + 2;
		wheel = rand()%2;
		switch(wheel){
			case 0:

					setMotorSpeed(rightWheel, DEFAULT_SPEED);
					setMotorSpeed(leftWheel, DEFAULT_SPEED+speed);

				break;
			case 1:

					setMotorSpeed(leftWheel, DEFAULT_SPEED);
					setMotorSpeed(rightWheel, DEFAULT_SPEED+speed);

				break;
			default:
				break;
		}
		sleep(500*time);
	}
}
//end behavior based tasks

//Task for the arbiter
task arbiter(){
			displayTextLine(5, "arbiter: %ld",arbiter);
	while(1){
		sleep(50);
		if(IF_DEATH){
			switchTask(DEATH);
			displayTextLine(7, "death");
		}
		else if(IF_BACKUP){
			if(state != BACKUP){
				switchTask(BACKUP);
			}
			displayTextLine(7, "backup");
		}
		else if(IF_ESCAPE){
			if(state != ESCAPE){
				switchTask(ESCAPE);
			}
			displayTextLine(7, "escape");
		}
		else if(IF_SONAR){
			if(state != SONAR){
				switchTask(SONAR);
			}
			displayTextLine(7, "sonar");
		}
		else if(IF_FEED){
			if(state != FEED){
				switchTask(FEED);
			}
			displayTextLine(7, "feed");
		}
		else {
			if(state != WANDER){
				switchTask(WANDER);
			}
			displayTextLine(7, "wander");
		}
	}
}
//end arbiter


//sensor tasks
void recievePacket(){

}

//Task for interpretting the incoming signals
//from the second brick
task communication(){
	sleep(20);
	int reading;
	while(1){
		reading = getColorAmbient(S1);
		if(reading < 10){
			upLight = 1;
			sleep(TIME_CON);
		}
		else if(getColorAmbient(S1) < 25){
			sleep(TIME_CON/2);
		}
		else{
			hogCPU();
			setMotorSpeed(motorA, -40);
	    setMotorSpeed(motorD, -40);
			while(getColorAmbient(S1) > 25);//could be off by 5ms+-light delaytime
			time1[T1] = 0;//about 5 to 15 seconds off
			while(time1[T1] < 175);
			if (getColorAmbient(S1) > 25)
				bumperValue |= 0b10;
			else
				bumperValue &= 0b01;
			while(time1[T1] < 325);
			if (getColorAmbient(S1) > 25)
				bumperValue |= 0b01;
			else
				bumperValue &= 0b10;
			stopTask(arbiter);
			startTask(arbiter);
			switchTask(backup);
			while(time1[T1] < 675);
			releaseCPU();
		}
			sleep(20);

			displayBigTextLine(13, "%d",bumperValue);

			sleep(50);
	}
}

//Task that gets the distance to an object in front of the robot
task sonarReading(){
	int g = 0;
	while(true) {
		distance = getUSDistance(sonarSensor) + ((distance - getUSDistance(sonarSensor)) * 50) / 100;
		sleep(20);
		displayTextLine(10, "Distance %d # %d", distance,g++);
	}
}

//Task for determing if the robot is on a food patch
task lightReading(){
	while(true) {
		if (getWeightedAverage(true) >= 20)
			leftDownLight = true;
		else
			leftDownLight = false;

		if (getWeightedAverage(false) >= 20)
			rightDownLight = true;
		else
			rightDownLight = false;

	sleep(10);
  }
}

//Task that handles the incrementing and decrementing
// of the hunger and fear of the robot
task aging(){
	time1[T2] = 0;
	while(1){
		if(time1[T2] > 2400){
				time1[T2] = 0;//time1[T2] - 240;
			if(onFood){
				hunger+=2;
				if(hunger > 100){hunger = 100;}
			}
			else{
				hunger--;
			}
			if(gotScared){
				gotScared--;
			}
			else if(fear < FEAR_MAX){
				fear++;
			}
		}
			displayTextLine(9, "hunger %d", hunger);
			displayTextLine(1, "fear %d", fear);
	sleep(100);
	}
}
//end sensor tasks



/**********************************************
*************** MAIN **************************
**********************************************/

//main
task main()
{
	gotScared = 0;
	isTurning = 0;
	displayTextLine(10, "Distance %d", distance);
	playTone(100, 50);
	//initialize values
	state = WANDER;
	bumperValue = 0;
	feeding = 0;
	hunger = 100;
	fear = 100;
	//start tasks
	nSchedulePriority = 20;
	startTask(communication,DEFAULT_PRIORITY+3);
	startTask(sonarReading,DEFAULT_PRIORITY+2);
	startTask(lightReading,DEFAULT_PRIORITY+2);
	startTask(aging,DEFAULT_PRIORITY+2);
	startTask(arbiter,DEFAULT_PRIORITY+1);
	startTask(wander,DEFAULT_PRIORITY);
	nSchedulePriority = DEFAULT_PRIORITY-2;

	while(1)(sleep(9999));
}

//task switching
void switchTask(states newState){
	hogCPU();
		if(state == newState){
			releaseCPU();
	  	return;
		}
 		switch(state){
  		case DEATH:stopTask(death);break;
			case BACKUP:stopTask(backup);break;
  		case ESCAPE:stopTask(escape);break;
			case SONAR:stopTask(sonar);break;
			case FEED:stopTask(feed);break;
			case WANDER:stopTask(wander);break;
		}
		switch(newState){
			case DEATH: startTask(death);break;
  		case BACKUP:startTask(backup);break;
			case ESCAPE:startTask(escape);break;
  		case SONAR:startTask(sonar);break;
			case FEED:startTask(feed);break;
			case WANDER:startTask(wander);break;
		}
  		displayTextLine(11, "new state: %d", newState);
	  state = newState;
	releaseCPU();
}

//Kills the program when the robot dies
void killProgram(){
			displayTextLine(8, "I DEAD");
			setMotorSync(rightWheel, leftWheel, 0, 0);
			stopTask(arbiter);
			stopTask(communication);
			//stopAllTasks();
}

/*
* Reverses the robot then turns left or right
*/
void reverseAndTurn(){
	int direction = rand() % 2;
	//back up
	setMotorSync(rightWheel, leftWheel, 0, -DEFAULT_SPEED);
	sleep(2000);
	// turn away from object
	pivot(direction);
	isTurning = 0;
}

/*
* Causes the robot to approach an object it sees with the sonic sensor
* Stops at the object and plays a tone.
*/
void approachObject(){
	isTurning = 0;
	while(distance > STOPPING_DISTANCE){
		sleep(5);
		isTurning = 0;
		if(!checkForObject()){//check if object still there
		isTurning = 0;
			return;
		}
		setMotorSync(rightWheel, leftWheel, 0, distance);
	isTurning = 0;
		}
	isTurning = 0;
	//stop
	setMotorSync(rightWheel, leftWheel, 0, 0);
	isTurning = 0;
	sleep(2000);
	isTurning = 0;
	isTurning = 1;
	reverseAndTurn();
	isTurning = 0;
}

/*
* Function to pivot the robot, takes a direction param and pivots left or right
*/
void pivot(int dir){
	if (dir) {// left
		setMotorSpeed(leftWheel, 0);
		setMotorSpeed(rightWheel, DEFAULT_SPEED);
		} else { //right
		setMotorSpeed(rightWheel, 0);
		setMotorSpeed(leftWheel, DEFAULT_SPEED);
	}
	sleep(1250);
	isTurning = 0;
}

/*
* Determines if there is an object in range
* @return true if there is an oject within 3 feet, false otherwise
*/
bool checkForObject(){
	return distance < MAX_DISTANCE;
}

/*
* Determines how long and how fast to run away, then runs.
*/
void runAway() {
	int speed = MAX_SPEED * (fear / 100.0);
	if (speed < 30) speed = 30;
	setMotorSpeed(leftWheel, speed);
	setMotorSpeed(rightWheel, speed);
	sleep(40 * fear);
}