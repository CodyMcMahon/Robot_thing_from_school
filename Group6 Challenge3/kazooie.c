//Despite it being .c this file is not a C file it is a RobotC file and has
//slightly different syntax
/*
GROUP 6
secondary robot brick code for project 3
*/

//turn "black color"
#define lblack setLEDColor(ledOff)
//turn "green color"
#define lgreen setLEDColor(ledGreen)
//turn "red color"
#define lred setLEDColor(ledRed)
#define TIME_CON 100
/* packet is already ready and this function sends it through LED lights using
   a custom protocol*/
void sendPacket(int num){
		if(num & 0b011){
			displayBigTextLine(10, "GO");
			lred;
			sleep(TIME_CON);
			lgreen;
			time1[T1] = 0;
			while(time1[T1] < 100);
			if (num & 0b010)
				lred;
			else
				lgreen;
			while(time1[T1] < 250);
			if (num & 0b001)
					lred;
			else
					lgreen;
			while(time1[T1] < 410);
			lgreen;
		}
		/*light sensor only*/
		else{
			lblack;
			sleep(TIME_CON);
			lgreen;
		}
}
/*main task*/
task main()
{
	lgreen;
	int num = 0;
	int upLight = 0;
	while(1){
		/*stall for 100ms*/
		time1[T1] = 0;
		while(time1[T1] < 100);
		/*left bumper*/
		if (getTouchValue(S3)&&((num & 0b1) == 0)){
			num|=0b1;
			continue;
		}
		/*right bumper*/
		if (getTouchValue(S4)&&((num & 0b10) == 0)){
			num|=0b10;
			continue;
		}
		/*up light sensor*/
		if (getColorAmbient(S2) > 41 &&((num & 0b100) == 0)){
			num|=0b100;
			continue;
		}
			displayBigTextLine(3, "sadfaegrgrg%d",getColorAmbient(S2));
		if(num){
			sendPacket(num);
			num = 0;
		}
	}
}
