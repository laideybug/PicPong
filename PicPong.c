//FILE: FinalPong.c
//AUTH: Stephen Laide (40089963)
//DATE: 11/12/2012
//YEAR: SESE Stage 1
//NOTE: ECS1001 Mini-project
//VERS: 0.9 (Beta)
//LOGO:
// .----------------.  .----------------.  .----------------.  .----------------.  .----------------.  .----------------.  .-----------------. .----------------. 
//| .--------------. || .--------------. || .--------------. || .--------------. || .--------------. || .--------------. || .--------------. || .--------------. |
//| |   ______     | || |     _____    | || |     ______   | || |              | || |   ______     | || |     ____     | || | ____  _____  | || |    ______    | |
//| |  |_   __ \   | || |    |_   _|   | || |   .' ___  |  | || |              | || |  |_   __ \   | || |   .'    `.   | || ||_   \|_   _| | || |  .' ___  |   | |
//| |    | |__) |  | || |      | |     | || |  / .'   \_|  | || |    ______    | || |    | |__) |  | || |  /  .--.  \  | || |  |   \ | |   | || | / .'   \_|   | |
//| |    |  ___/   | || |      | |     | || |  | |         | || |   |______|   | || |    |  ___/   | || |  | |    | |  | || |  | |\ \| |   | || | | |    ____  | |
//| |   _| |_      | || |     _| |_    | || |  \ `.___.'\  | || |              | || |   _| |_      | || |  \  `--'  /  | || | _| |_\   |_  | || | \ `.___]  _| | |
//| |  |_____|     | || |    |_____|   | || |   `._____.'  | || |              | || |  |_____|     | || |   `.____.'   | || ||_____|\____| | || |  `._____.'   | |
//| |              | || |              | || |              | || |              | || |              | || |              | || |              | || |              | |
//| '--------------' || '--------------' || '--------------' || '--------------' || '--------------' || '--------------' || '--------------' || '--------------' |
// '----------------'  '----------------'  '----------------'  '----------------'  '----------------'  '----------------'  '----------------'  '----------------' 



//******************************************************************
// Header files
//******************************************************************

#include <p18f8722.h>
#include <delays.h>
#include "PIC18LCD.h"

//******************************************************************
// Config settings
//******************************************************************

#pragma config OSC = HSPLL
#pragma config FCMEN = OFF
#pragma config IESO = OFF
#pragma config PWRT = OFF
#pragma config BOREN = OFF
#pragma config WDT = OFF
#pragma config MCLRE = ON
#pragma config LVP = OFF
#pragma config XINST = OFF

//******************************************************************
// Variable definitions
//******************************************************************

#define LED0 PORTDbits.RD0
#define LED7 PORTDbits.RD7

//******************************************************************
// Universal constant declarations and definitions
//******************************************************************

unsigned char mode = 0;
unsigned char operation;
unsigned char count;
unsigned char speed1 = 60;
unsigned char speed2 = 125;
unsigned char P1score = 5;
unsigned char P2score = 5;
unsigned char P1scoreCounter = 0;
unsigned char P2scoreCounter = 0;

//******************************************************************
// Array declarations
//******************************************************************

char heartScore[6];

//******************************************************************
// LCD text arrays
//******************************************************************

char lineClear[] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\n'};
char welcome[] = {' ', ' ', ' ', 'W', 'E', 'L', 'C', 'O', 'M', 'E', ' ', 'T', 'O', '\n'};						//WELCOME TO
char picPong[] = {0x05, 0x05, 0x05, ' ', 'P', 'I', 'C', '-','P', 'O', 'N', 'G', ' ', 0x00, 0x00, 0x00, '\n'};	//:) PIC-PONG :)
char select1[] = {' ', ' ', 'S', 'e', 'l', 'e', 'c', 't', ' ', 'm', 'o', 'd', 'e',':', '\n'};					//Select mode:
char select2[] = {0x05, ' ', 'E', 'a', 's', 'y', 0x06, ' ', ' ', 0x06, 'H', 'a', 'r', 'd', ' ', 0x00, '\n'};	//:) Easy v v Hard >:(
char easyMode[] = {' ', 0x05, ' ', 'E', 'a', 's', 'y', ' ', 'm', 'o', 'd', 'e', ' ', 0x05, '\n'};				//Easy mode
char hardMode[] = {' ', 0x00, ' ', 'H', 'a', 'r', 'd', ' ', 'm', 'o', 'd', 'e', ' ', 0x00, '\n'};				//Hard mode
char selected[] = {' ', ' ', ' ', 's', 'e', 'l', 'e', 'c', 't', 'e', 'd', '!', '\n'};							//selected!
char startWhen[] = {' ', 0x06, ' ', 'S', 't', 'a', 'r', 't', ' ', 'w', 'h', 'e', 'n', ' ', 0x06, '\n'};			//Start when
char ready[] = {' ', 0x06, ' ', ' ', ' ', 'r', 'e', 'a', 'd', 'y', '!', ' ', ' ', ' ', 0x06, '\n'};				//ready!
char player1[] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'P', 'l', 'a', 'y', 'e', 'r', ' ', '1', '\n'};		//Player 1
char player2[] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'P', 'l', 'a', 'y', 'e', 'r', ' ', '2', '\n'};		//Player 2
char clearScore[] = {' ',' ', ' ', ' ', ' ', ' ', '\n'};
char P1win[] = {0x05, 'P', 'l', 'a', 'y', 'e', 'r', ' ', '1', ' ', 'w', 'i', 'n', 's', '!', 0x05, '\n'};		//Player 1 wins!
char P2win[] = {0x05, 'P', 'l', 'a', 'y', 'e', 'r', ' ', '2', ' ', 'w', 'i', 'n', 's', '!', 0x05, '\n'};		//Player 2 wins!

//******************************************************************
// Function declarations
//******************************************************************

void displayScore1(void);
void displayScore2(void);
void gameOver(void);

//******************************************************************
// Interrupt declarations
//******************************************************************

void game_ISR(void);

//******************************************************************
// Main function
//******************************************************************

int main() {

	INTCONbits.INT0IF = 0;										//clear RB0 interrupt flag
	INTCONbits.INT0IE = 1;										//enable RB0 as interrupt source
	INTCON2bits.INTEDG0 = 0;									//make RB0 negative edge

	INTCON3bits.INT2IF = 0;										//clear RB2 interrupt flag
	INTCON3bits.INT2IE = 1;										//enable RB2 as interrupt source
	INTCON2bits.INTEDG2 = 0;									//make RB2 negative edge

	INTCONbits.GIE = 1;											//enable all interrupts globally


	ADCON1bits.PCFG0 = 1;										//set up buttonA (RA5) as an input
	ADCON1bits.PCFG1 = 1;
	ADCON1bits.PCFG2 = 1;
	ADCON1bits.PCFG3 = 1;

	TRISAbits.RA5 = 1;											//set up buttonA (RA5) as an input
	TRISBbits.RB0 = 1;											//set up player 1's (RA5) button as an input

	TRISD = 0x00;												//set up PORTD
	PORTD = 0x00;

	heartScore[0] = (0x07);										//heartScore array 
	heartScore[1] = (0x07);
	heartScore[2] = (0x07);
	heartScore[3] = (0x07);
	heartScore[4] = (0x07);
	heartScore[5] = (' ');


	LCDInit();													//LCD initialisation and welcome message
	LCDClear();
	LCDLine_1();
	d_write_line_delay(welcome, 50);
	LCDLine_2();
	d_write_line_delay(picPong, 50);
	
	for (count = 0; count < 10; count ++) {
		Delay10KTCYx(250);
	}
	
	LCDLine_1();												//difficulty mode select screen
	d_write_line_delay(lineClear, 20);
	LCDLine_2();
	d_write_line_delay(lineClear, 20);
	LCDLine_1();
	d_write_line_delay(select1, 20);
	LCDLine_2();
	d_write_line_delay(select2, 20);

	while(1) {													//loop to wait for mode selection by user
		if (mode == 1) {											//confirmation of "Easy" mode selection
			LCDLine_1();
			d_write_line_delay(lineClear, 20);
			LCDLine_2();
			d_write_line_delay(lineClear, 20);
			LCDLine_1();
			d_write_line_delay(easyMode, 20);
			LCDLine_2();
			d_write_line_delay(selected, 20);
			
			for (count = 0; count < 10; count ++) {
				Delay10KTCYx(250);
			}
			
			LCDLine_1();
			d_write_line_delay(lineClear, 20);
			LCDLine_2();
			d_write_line_delay(lineClear, 20);
			LCDLine_1();
			d_write_line_delay(startWhen, 20);
			LCDLine_2();
			d_write_line_delay(ready, 20);
			
			for (count = 0; count < 10; count ++) {
				Delay10KTCYx(250);
			}
			
			break;
		}
		
		if (mode == 2) {											//confirmation of "Hard" mode selection
			LCDLine_1();
			d_write_line_delay(lineClear, 20);
			LCDLine_2();
			d_write_line_delay(lineClear, 20);
			LCDLine_1();
			d_write_line_delay(hardMode, 20);
			LCDLine_2();
			d_write_line_delay(selected, 20);
			
			for (count = 0; count < 10; count ++) {
				Delay10KTCYx(250);
			}
			
			LCDLine_1();
			d_write_line_delay(lineClear, 20);
			LCDLine_2();
			d_write_line_delay(lineClear, 20);
			LCDLine_1();
			d_write_line_delay(startWhen, 20);
			LCDLine_2();
			d_write_line_delay(ready, 20);
			
			for (count = 0; count < 10; count ++) {
				Delay10KTCYx(250);
			}
			
			break;
		}
	}

	LCDClear();													//scoreboard setup
	LCDLine_1();
	d_write_line_delay(player1, 20);
	LCDLine_2();
	d_write_line_delay(player2, 20);	
	displayScore1();
	displayScore2();

	operation = 0;												//set starting condition at 0

	while(1) {
		if (operation == 0) PORTD = 0;							//game start/round start - wait for button press to initialise action
		
		if (operation == 1) {									//initiate LED shift direction left << for Player 1 (RA5)
			PORTD = 0b00000001;
			Delay10KTCYx(speed1);
			
			while ((operation == 1) && (PORTD != 0b10000000)) {  //check for button press that has resulted in an operation change
				PORTD = PORTD << 1;
				Delay10KTCYx(speed2);
			}
			
			if (operation == 1) operation = 4;					//if "ball" reaches other side without a reaction, Player 1 scores
		}
		
		if (operation == 2 ) {									//initiate LED shift direction right >> for Player 2 (RB0)
			PORTD = 0b10000000;
			Delay10KTCYx(speed1);
			
			while ((operation == 2) && (PORTD != 0b00000001)) {	//check for button press that has resulted in an operation change
				PORTD = PORTD >> 1;
				Delay10KTCYx(speed2);
			}
			
			if (operation == 2) operation = 3;					//if "ball" reaches other side without a reaction, Player 2 scores
		}
		
		if (operation == 3) {									//if Player 1 performed a miss-hit or if player 2 scored
			PORTD = 0;											//reset LEDs
			P1score = P1score - 1;								//decrement Player 1's "hearts"/"lives"
			P1scoreCounter = P1scoreCounter + 1;				//increment Player 1 score tracker
			speed1 = 60;										//reset speed1
			speed2 = 125;										//reset speed2
			displayScore1();									//change LCD score accordingly
			
			for (count = 0; count < 10; count ++) {				//LED7 flash x5 to acknowledge Player 2 score
				Delay10KTCYx(125);
				LED7 ^= 1;
			}
			
			if (P1score == 0) gameOver();						//if Player 1 has no more lives, game is over
			else {
				operation = 0;									//if Player 1 still has lives, restart round and continue game
			}
		}
		
		if (operation == 4) {									//if Player 2 performed a miss-hit or if player 1 scored
			PORTD = 0;											//reset LEDs
			P2score = P2score - 1;								//decrement Player 2's "hearts"/"lives"
			P2scoreCounter = P2scoreCounter + 1;				//increment Player 2 score tracker
			speed1 = 60;										//reset speed1
			speed2 = 125;										//reset speed2
			displayScore2();									//change LCD score accordingly
			
			for (count = 0; count < 10; count ++) {				//LED0 flash x5 to acknowledge Player 1 score
				Delay10KTCYx(125);
				LED0 ^= 1;
			}
			
			if (P2score == 0) gameOver();						//if Player 2 has no more lives, game is over
			else {
				operation = 0;									//if Player 2 still has lives, restart round and continue game
			}
		}
		
		if (operation == 5) {									//operation 5 resets all variables and scores so the game can be replayed once a player has won
			operation = 0;
			P1score = 5;
			P2score = 5;
			P1scoreCounter = 0;
			P2scoreCounter = 0;
			LCDClear();											//reset LCD
			LCDLine_1();
			d_write_line_delay(player1, 20);
			LCDLine_2();
			d_write_line_delay(player2, 20);	
			displayScore1();
			displayScore2();
		}
	}
	return 0;
}


//******************************************************************
// Player 1 score display function
//******************************************************************

void displayScore1(void) {

	LCDLine_1();
	d_write_line(clearScore);									//clear previous score displayed on LCD so the current score can be displayed
	LCDLine_1();

	switch (P1score){											//if Player 1's score is equal to...
		case 0:
			d_write(0x00);										//display "angry" character
			break;
		case 1:
			d_write(0x01);										//display "worried" character
			break;		
		case 2:
			d_write(0x02);										//display "sad" character
			break;
		case 3:
			d_write(0x03);										//display "indifferent" character
			break;
		case 4:
			d_write(0x04);										//display "smiley" character
			break;
		case 5:
			d_write(0x05);										//display "happy" character
			break;
		default:
			break;
	}
	
	for (count = 0; count < P1score; count ++) {
		d_write(heartScore[P1scoreCounter]);					//display relevant number of lives (this is inversely proportional to the scoreCounter)
	}
}

//******************************************************************
// Player 2 score display function
//******************************************************************

void displayScore2(void) {

	LCDLine_2();
	d_write_line(clearScore);									//clear previous score displayed on LCD so the current score can be displayed
	LCDLine_2();

	switch (P2score) {											//if Player 1's score is equal to...
		case 0:
			d_write(0x00);										//display "angry" character
			break;
		case 1:
			d_write(0x01);										//display "worried" character
			break;		
		case 2:
			d_write(0x02);										//display "sad" character
			break;
		case 3:
			d_write(0x03);										//display "indifferent" character
			break;
		case 4:
			d_write(0x04);										//display "smiley" character
			break;
		case 5:
			d_write(0x05);										//display "happy" character
			break;
		default:
			break;
	}
	
	for (count = 0; count < P2score; count ++) {
		d_write(heartScore[P2scoreCounter]);					//display relevant number of lives (this is inversely proportional to the scoreCounter)
	}
}

//******************************************************************
// Game Over function
//******************************************************************

void gameOver(void) {

	if (P1score == 0) {											//if function was called because P1 has run out of lives...
		LCDLine_2();
		d_write_line_delay(lineClear, 20);						//clear Player 2's line
		LCDLine_2();	
		d_write_line_delay(P2win, 20);							//declare Player 2 the winner
		
		for(count = 0; count < 10; count ++) {
			Delay10KTCYx(250);
		}
		
		operation = 5;											//reset all values
	}
	
	if (P2score == 0) {											//if function was called because P2 has run out of lives...
		LCDLine_1();
		d_write_line_delay(lineClear, 20);						//clear Player 1's line
		LCDLine_1();
		d_write_line_delay(P1win, 20);							//declare Player 1 the winner
		
		for (count = 0; count < 10; count ++) {
			Delay10KTCYx(250);	
		}
		
		operation = 5;											//reset all values
	}
}

//******************************************************************
// Interrupt redirection
//******************************************************************

#pragma code highPriorityInt=0x08
void highPriorityInt(void){
	_asm
	GOTO game_ISR
	_endasm
}

//******************************************************************
// Button interrupt function
//******************************************************************

#pragma interrupt game_ISR
void game_ISR(void) {

	if (INTCON3bits.INT2IF == 1) {							//if RA5 was pressed...		
		INTCON3bits.INT2IF = 0;								//clear RB2(RA5) interrupt flag
		
		if (mode == 0) mode = 2;							//if in mode select loop, select mode 2 ("hard")
		
		switch (operation) {
			case 0:
				operation = 1;								//launch ball from Player 1's side if game/round is starting
				break;
			case 1:
				operation = 3;								//if a button press occurs while LED is shifting left (moving away), decrement a life
				break;
			case 2:
				if (PORTD == 0b00000001) {					//if button press is valid, register a "hit"
					operation = 1;
					
					if (mode == 2) {
						speed1 = speed1*0.95;				//if "hard" difficulty setting was chosen, increase speed1(LED initialisation delay) by 5% each hit
						speed2 = speed2*0.80;				//if "hard" difficulty setting was chosen, increase speed2(LED shift delay) by 20% each hit
					}
				} else{
					operation = 3;							//if button press is too early while LED is shifting right (returning), decrement a life 
				}
				break;
			default:
				break;
		}
	}

	if (INTCONbits.INT0IF == 1) {							//if RA5 was pressed...	
		INTCONbits.INT0IF = 0;								//clear RB0 interrupt flag
		
		if (mode == 0) mode = 1;							//if in mode select loop, select mode 1 ("easy")
		
		switch (operation) {
			case 0:
				operation = 2;								//launch ball from Player 2's side if game/round is starting
				break;
			case 1:
				if (PORTD == 0b10000000) {					//if button press is valid, register a "hit"
					operation = 2;
					
					if (mode == 2) {
						speed1 = speed1*0.95;				//if "hard" difficulty setting was chosen, increase speed1(LED initialisation delay) by 5% each hit
						speed2 = speed2*0.80;				//if "hard" difficulty setting was chosen, increase speed2(LED shift delay) by 20% each hit
					}	
				} else{
					operation = 4;							//if button press is too early while LED is shifting right (returning), decrement a life 
				}
				break;
			case 2:
				operation = 4;								//if a button press occurs while LED is shifting left (moving away), decrement a life
				break;
			default:
				break;
		}
	}
}