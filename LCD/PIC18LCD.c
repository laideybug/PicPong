//*****************************************************************
// PICDEM HPC Explorer 18 LCD function
// Filename : pic18lcd.c
//
// By Rockie Roper, modified by Simon Cotton, modified again by Stephen Laide
// Converted from the ASM that came with the demo board
//
// Created on 2009-06-05, modified 2011-11-25, modified again 2012-12-9
// 
//*****************************************************************
#ifndef __PIC18LCD_C
#define __PIC18LCD_C

#include <p18F8722.h>
#include "PIC18LCD.h"
unsigned char happy[8] = {0x00,0x0A,0x0A,0x00,0x1F,0x11,0x0E,0x00};
unsigned char smiley[8] = {0x00,0x0A,0x0A,0x00,0x04,0x11,0x0E,0x00};
unsigned char indif[8] = {0x00,0x0A,0x00,0x00,0x04,0x00,0x1F,0x00};
unsigned char sad[8] = {0x00,0x0A,0x00,0x04,0x00,0x0E,0x11,0x00};
unsigned char angry[8] = {0x11,0x0A,0x11,0x04,0x00,0x0E,0x11,0x00};
unsigned char arrowDown[8] = {0x00,0x04,0x04,0x04,0x15,0x0E,0x04,0x00};
unsigned char worried[8] = {0x0A,0x11,0x0A,0x00,0x04,0x00,0x0E,0x11};
unsigned char heart[8] = {0x00,0x0A,0x15,0x11,0x0A,0x04,0x00,0x00};

void buildLCDchar (unsigned char loc, unsigned char *p);

#pragma code
 
//*****************************************************************
// LCD busy delay
//*****************************************************************
void LCDBusy(void)
{
       Delay10KTCYx(50);
}
//*****************************************************************
// Write to MCP923S17 Port A
//*****************************************************************
void WritePortA(char b)
{
       LCD_CS = 0;
        
       LCD_SSPBUF = 0x40;
       while(!LCD_SPI_IF);
       LCD_SPI_IF = 0;
       
       LCD_SSPBUF = 0x12;
       while(!LCD_SPI_IF);
       LCD_SPI_IF = 0;
        
       LCD_SSPBUF = b;
       while(!LCD_SPI_IF);
       LCD_SPI_IF = 0;
        
       LCD_CS = 1;
}
//*****************************************************************
// Write to MCP923S17 Port B
//*****************************************************************
void WritePortB(char b)
{
       LCD_CS = 0;
        
       LCD_SSPBUF = 0x40;
       while(!LCD_SPI_IF);
       LCD_SPI_IF = 0;
       
       LCD_SSPBUF = 0x13;
       while(!LCD_SPI_IF);
       LCD_SPI_IF = 0;
        
       LCD_SSPBUF = b;

       while(!LCD_SPI_IF);
       LCD_SPI_IF = 0;
	   LCD_CS = 1;
        
        	
}
//*****************************************************************
// Write the data to the display
//*****************************************************************
void d_write(char b)
{
        WritePortA(0x80);
        Delay10KTCYx(20);
        WritePortB(b);
        Nop();
        Nop();
        Nop();
        Nop();
        WritePortA(0xC0);
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        WritePortA(0x00);
}

//*****************************************************************
// Write the line to the display
//*****************************************************************
void d_write_line(char* b)
{
		
	WritePortA(0x80);
    LCDBusy();
		
	while( *b != '\n' ){

       	WritePortB(*b++);
       	Nop();
       	Nop();
       	Nop();
   	 	Nop();
       	WritePortA(0xC0);
       	Nop();
       	Nop();
       	Nop();
       	Nop();
   	 	Nop();
       	Nop();
		
       	WritePortA(0x00);		
   }

}
//*****************************************************************
// Write the line to the display with delay between characters
//*****************************************************************
void d_write_line_delay(char* b, unsigned char d)
{
	if( d < 20 ) d = 20; 
	WritePortA(0x80);
    LCDBusy();
		
	while( *b != '\n' ){

       	WritePortB(*b++);
       	Nop();
       	Nop();
       	Nop();
   	 	Nop();
       	WritePortA(0xC0);
       	Nop();
       	Nop();
       	Nop();
       	Nop();
  	 	Nop();
       	Nop();
		
       	WritePortA(0x00);
       	Delay10KTCYx(d);		
   }
}
//*****************************************************************
// Send a instruction to the display
//*****************************************************************
void i_write(char b)
{
    WritePortA(0x00);
    LCDBusy();
    WritePortB(b);
    Nop();
    Nop();
    Nop();
    Nop();
    WritePortA(0x40);
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    WritePortA(0x00);
}
//*****************************************************************
// Write to line 1 of the display
//*****************************************************************
void LCDLine_1(void)
{
     i_write(0x80);
}
//*****************************************************************
// Write to line 2 of the display
//*****************************************************************
void LCDLine_2(void)
{
     i_write(0xC0);
}
//*****************************************************************
// To clear the display
//*****************************************************************
void LCDClear(void)
{
     i_write(0x01);
}
//******************************************************************
// Function to write to the PORT
//******************************************************************
void InitWrite(char b)
{
     WritePortA(0);
     WritePortB(b);
     Nop();
     Nop();
     Nop();
     WritePortA(0x40);
     Nop();
     Nop();
     Nop();
     Nop();
     Nop();
     Nop();
     Nop();
     Nop();
     Nop();
     WritePortA(0);
}
//*****************************************************************
// Initialize MCP923S17 Port A
//*****************************************************************
void InitPortA_SPI(char b)
{
     LCD_CS = 0;
     LCD_SSPBUF = 0x40;
     while(!LCD_SPI_IF);
     LCD_SPI_IF = 0;
        
     LCD_SSPBUF = 0x00;
     while(!LCD_SPI_IF);
     LCD_SPI_IF = 0;
        
     LCD_SSPBUF = b;
     while(!LCD_SPI_IF);
     LCD_SPI_IF = 0;
        
     LCD_CS = 1;
}
//*****************************************************************
// Initialize MCP923S17 Port B
//*****************************************************************
void InitPortB_SPI(char b)
{
     LCD_CS = 0;
     LCD_SSPBUF = 0x40;
     while(!LCD_SPI_IF);
     LCD_SPI_IF = 0;
        
     LCD_SSPBUF = 0x01;
     while(!LCD_SPI_IF);
     LCD_SPI_IF = 0;
        
     LCD_SSPBUF = b;
     while(!LCD_SPI_IF);
     LCD_SPI_IF = 0;
        
     LCD_CS = 1;
}
//*****************************************************************
// Initialize MCP923S17 SPI
//*****************************************************************
void InitSPI(void)
{
     LCD_SCK_TRIS = 0;
     LCD_SDO_TRIS = 0;
        
     LCD_SPICON1 = 0x22;
     LCD_SPISTATbits.CKE = 1;
     //LCD_SPISTATbits.SMP = 0;
     LCD_SPI_IF = 0;
}
//******************************************************************
// LCD Initialization function
//******************************************************************
void LCDInit(void)
{
	LCD_CS_TRIS = 0;
	LCD_CS = 1;
	Delay10KTCYx(50);
        
	LCD_RST_TRIS = 0;
	LCD_RST = 0;
	Delay10KTCYx(20);
	LCD_RST = 1;
        
	InitSPI();
	InitPortA_SPI(0);
	InitPortB_SPI(0);
        
	WritePortA(0);
        
    Delay10KTCYx(20);
	InitWrite(0x3C);                               					//0011NFxx
        
	Delay10KTCYx(20);
	InitWrite(0x0C);                              				  	//Display Off
        
	Delay10KTCYx(20);
	InitWrite(0x01);                              				 	//Display Clear
        
	Delay10KTCYx(20);
	InitWrite(0x06);                              				 	//Entry mode

	buildLCDchar(0, angry);											//write "angry" character to CGRAM
	buildLCDchar(1, worried);										//write "worried" character to CGRAM
	buildLCDchar(2, sad);											//write "sad" character to CGRAM
	buildLCDchar(3, indif);											//write "indif" character to CGRAM
	buildLCDchar(4, smiley);										//write "smiley" character to CGRAM
	buildLCDchar(5, happy);											//write "happy" character to CGRAM
	buildLCDchar(6, arrowDown);										//write "arrowDown" character to CGRAM
	buildLCDchar(7, heart);											//write "heart" character to CGRAM
}

//******************************************************************
// buildLCDchar function
//******************************************************************											
//Example:																	
//		buildLCDchar(1, pattern);											

void buildLCDchar (unsigned char loc, unsigned char *p){			//loc - location in CGRAM where characters are to be stored - 0,1,2,3,4,5,6,7
																	//p: pointer to custom characters' binary pattern data
	unsigned char i;

	if (loc < 8){													//if location is within CGRAM memory parameters (i.e. up to 8 locations)

		i_write(0x40 + (loc*8));									//0x40 points to CGRAM at position 0x00, "+ (loc*8)" increments the pointer to the required location
		for(i = 0; i < 8; i ++){
			d_write(p[i]);											//write custom character to relevant CGRAM location
		}
	}
	i_write(0x80);													//0x80 points back to DDRAM at location 0x00
}