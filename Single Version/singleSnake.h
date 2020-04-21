#ifndef _SNAKE_H_
#define _SNAKE_H_

typedef unsigned char boolean;
#define TRUE	1
#define FALSE	0

#define UP	  				0x4800
#define DOWN 		 		0x5000
#define LEFT 				0x4b00
#define RIGHT 				0x4d00
#define ESC	  				0x11b
#define PGUP  				0x4900
#define PGDN  				0x5100

#define MAX_COUNT 			20000
#define MIN_COUNT 			125
#define DEFAULT_COUNT 		5000

#define MAX_X   			80
#define MAX_Y   			25
#define DEFAULT_X 			40
#define DEFAULT_Y 			12
#define Full_Screen_COUNT	2000

#define HEAD_RIGHT_INDEX 	62

#define BLOCK  		 		0
#define USUAL_FOOD   		2
#define SUPER_FOOD	 		1
#define BODY  				-1
#define WALL  				-2

#define MAX_LEN 			1000

typedef struct DELTA_MOVE{
	int deltRow;
	int deltCol;
}DELTA_MOVE;

typedef struct SNAKE_BODY{
	int xPostion;
	int yPostion;
}SNAKE_BODY;

typedef struct SNAKE{
	int head;
	int len;
	int curLen;
	int direct;
	SNAKE_BODY sb;
}SNAKE;

void dealFood();
void dealFoodIndex();
void showFood();
void creatFoodNum();
void move(int headXPos, int headYPos, DELTA_MOVE delta, char headType, SNAKE snake);
boolean dealEat(SNAKE snake);
char getHeadType(int snakeHeadIndex);
int readValidOrder(int key, int tempCount, SNAKE snake);
boolean isFinished(int x, int y);
void showBorder();
void hideCursor();

#endif