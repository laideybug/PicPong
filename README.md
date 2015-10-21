#PicPong
PicPong is a simple Pong-style game made to run on a PIC18F8722 explorer board. The explorer board features a PIC18F8722 microcontroller, two buttons, a strip of eight LEDs and an LCD display.

The project was developed using the MPLAB C18 compiler of Microchip’s MPLAB Integrated Development Environment.

## Gameplay
The game is initialised by a player pressing their button and this starts the LED "ball" shifting towards the other player. If the other player presses their button when the "ball" reaches the LED space closest to them, the "ball" is returned. Otherwise, they lose a life. Each player has 5 lives and the game continues until one player loses all of their lives.

## Game Modes
Alongside the basic ping pong game there is the option to choose a difficulty mode via the LCD display when the game is started. These are “Easy Mode” and “Hard Mode”. “Easy Mode” refers to the basic game mode outlined above. If “Hard Mode” is selected, the speed of the "ball" will increase every time it is returned by a player. If a player scores, the speed is reset to its original value and the round restarts. 

## Program Flow
The main game logic is found in PicPong.c and the program flow can be seen below:

![](http://i.imgur.com/h29CwRj.jpg)

## LCD Display
The HD44780 LCD display module contains 64 bytes of writeable CGRAM and enables the user to create up to eight 5x8 pixel character patterns for use on the display. Each row of the character pattern is represented a binary number however, as the LCD’s data bus is 8 bits wide, the upper 3 bits will not be seen and are required to be set to 0. The 5 lower bits of each row represent the data for each specific line of the image. The 1’s represent a black pixel on the display while the 0’s represent white pixels. As the game is being designed with children as the end users in mind, 8 suitable character patterns were designed for use on the LCD.

![](http://i.imgur.com/RWs5wPb.jpg)