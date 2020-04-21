#ifndef _FINAL_VERSION_H_
#define _FINAL_VERSION_H_

typedef unsigned char boolean;
#define TRUE	1
#define FALSE	0

#define W	  				0x1177
#define S     				0x1f73
#define A     				0X1e61
#define D     				0x2064
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
#define SNAKE_HEAD_UP 		30	/*下四行是第二条蛇的蛇头图形，方便我们区分两条蛇*/
#define SNAKE_HEAD_DOWN 	31
#define SNAKE_HEAD_LEFT 	17
#define SNAKE_HEAD_RIGHT 	16

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
	SNAKE_BODY *sb;
}SNAKE;

void hideCursor();
boolean init();
boolean readPlayerNum();
void showBorder();
boolean isFinished(int headX, int headY);
int readValidOrder(int key, SNAKE *snake);
char getHeadType1(int snakeHeadIndex);
int getHeadType2(int snakeHeadIndex);
boolean dealEat(SNAKE *snake);
void dealmove(int *headXPos, int *headYPos, DELTA_MOVE *delta, char headType, SNAKE *snake);
void dealFoodIndex();
void showFood();
void creatFoodNum();
void dealFood();
void dealOnePlayer();
void dealTwoPlayer();

#endif