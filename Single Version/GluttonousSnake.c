#include <stdlib.h>
#include <conio.h>
#include <stdio.h>
#include <BIOS.H>
#include <time.h>
#include <dos.h>

#include "singleSnake.h"

int foodNum = 0;
boolean eatUpFood = TRUE;
char snakeHead[4] = {'^', 'v', '<', '>'};
int screenPoint[Full_Screen_COUNT] = {0};

/*处理前进吃食问题*/
boolean dealEat(SNAKE *snake) {
    int i;
    int k;
    int tmp;
    int tempHeadX;
    int tempHeadY;
    
    tempHeadX = snake->sb[snake->head].xPostion;
    tempHeadY = snake->sb[snake->head].yPostion;
    tmp = (tempHeadY - 1) * MAX_X +  tempHeadX - 1;
    if((screenPoint[tmp] == USUAL_FOOD) || (screenPoint[tmp] == SUPER_FOOD)) {	/*吃到食物*/
        foodNum--;
        screenPoint[tmp] = 0;
        (snake->len) += (screenPoint[tmp] == USUAL_FOOD) ? 1 : 2;
    }

    if(screenPoint[tmp] == BODY) {	/*吃到自身*/
        clrscr();
        gotoxy(40, 12);
        printf("Game over!");
        getch();

        return TRUE;
    }

    screenPoint[(tempHeadY - 1) * MAX_X +  tempHeadX - 1] = BODY;	/*将这个点转换为蛇身*/
    if(foodNum <= 0) {
        eatUpFood = TRUE;
        creatFoodNum();
    }
    return FALSE;
}

void creatFoodNum() {
    foodNum = rand()%2 ? 3 : 2;
}

void dealFood() {
    dealFoodIndex();
    showFood();
}

/*显示食物*/
void showFood() {
    int i;
    int x;
    int y;

    for(i = 0; i < Full_Screen_COUNT; i++) {
        if(screenPoint[i] == USUAL_FOOD || screenPoint[i] == SUPER_FOOD) {
            x = (i + 1) % MAX_X;
            y = (i + 1) / MAX_X + 1;
            gotoxy(x, y);
            printf((screenPoint[i] == USUAL_FOOD) ? "#" : "$");
        }
    }
    eatUpFood = FALSE;
}

/*放置食物*/
void dealFoodIndex() {
    int randNum;
    int i;
    int count = 0;
    int index[Full_Screen_COUNT] = {0};	/*这个数组是为我们之后的“发牌算法”的使用做准备*/
    int j = Full_Screen_COUNT-1;

    for(i = 0; i < Full_Screen_COUNT; i++) {
        if(screenPoint[i] == BLOCK) {
            index[count] = i;
            count++;
        }
    }

    for(i = 0; i < foodNum; i++) {
        srand(time(0) + i);
        randNum = rand() % count;

        screenPoint[index[randNum]] = rand()%2 ? USUAL_FOOD : SUPER_FOOD;
        index[randNum] = index[j--];
        count--;
    }
}
/*根据方向得到蛇头的图形*/
char getHeadType(int snakeHeadIndex) {
    return snakeHead[snakeHeadIndex];
}

/*使得蛇能够向前移动*/
void move(int *headXPos, int *headYPos, DELTA_MOVE *delta, char headType, SNAKE *snake) {
    int tailRow;
    int tailCol;
    int tail;
    int tempX;
    int tempY;

    tempX = snake->sb[snake->head].xPostion;
    tempY = snake->sb[snake->head].yPostion;
    gotoxy(tempX, tempY);
    printf("*");
    tempX = tempX + delta->deltRow;
    tempY = tempY + delta->deltCol;
    gotoxy(tempX, tempY);
    printf("%c", headType);
    snake->head  = (snake->head + 1) % MAX_LEN;
    snake->sb[snake->head].xPostion = tempX;
    snake->sb[snake->head].yPostion = tempY;
    *headXPos = tempX;
    *headYPos = tempY;
    if(snake->curLen < snake->len) {
        (snake->curLen)++;
        return;
    }
        
    tail = (snake->head - snake->len + MAX_LEN) % MAX_LEN;
    tailRow = snake->sb[tail].xPostion;
    tailCol = snake->sb[tail].yPostion;
    screenPoint[(tailCol - 1) * MAX_X +  tailRow - 1] = 0;
    gotoxy(tailRow, tailCol);
    printf(" ");    
}

/*通过判断按键的键值来得到不同的响应*/
int readValidOrder(int key, int *cdTime, SNAKE *snake) {
    int tmp;

    tmp = snake->direct;
    switch(key) {
        case RIGHT: return tmp == 2 ? 2 : 3;
        case LEFT:  return tmp == 3 ? 3 : 2;
        case DOWN:  return tmp == 0 ? 0 : 1;
        case UP:    return tmp == 1 ? 1 : 0;    /*上面的返回值分别对应“上”、“下”、“左”、“右”在“蛇头类型数组”中的下标*/

        case PGUP:  if(*cdTime > MIN_COUNT) {
                        *cdTime = *cdTime / 2;
                    }
                    return -1;
        case PGDN:  if(*cdTime < MAX_COUNT) {
                        *cdTime = *cdTime * 2;
                    }
                    return -1;
    }
}

/*隐藏光标*/
void hideCursor() { /*这个函数无需理解，因为本人想要做的比较美观，才在网上查询得此代码段使得光标被隐藏*/
    union REGS reg;
    reg.h.ah = 1;
    reg.h.ch = 15;
    reg.h.cl = 0;
    int86(0x10,&reg,&reg);
}

/*判断蛇头是否撞墙*/
boolean isFinished(int x, int y) {
    if(WALL != screenPoint[(y-1)*MAX_X + x-1]) {
        return FALSE;
    }
        return TRUE;
}

/*显示“墙壁”*/
void showBorder() {
    int i;

    clrscr();
    for(i = 2; i < MAX_X; i++) {    /*将“上下墙壁”显示出来*/
        gotoxy(i, 2);
        putch(223);
        gotoxy(i, 25);
        putch(220);
    }
    for(i = 2; i <= MAX_Y; i++) {    /*将“左右墙壁”显示出来*/
        gotoxy(1, i);
        putch(219);
        gotoxy(80, i);
        putch(219);
    }
}

/*将“墙壁”都在屏幕点数组中标记出来*/
void init() {
    int i;
    int j;
    int tmp;

    for(i = 1; i <= 80; i++) {
        for(j = 1; j <= 25; j++) {
            if(!(i > 1 && i < 80 && j > 1 && j < 25)) {
                tmp = (j - 1) * 80 +  i - 1;
                screenPoint[tmp] = WALL;
            }
        }
    }
    clrscr();
    gotoxy(22, 12); /*这里的行、列值完全是一个点一个点测出来的，为了使下面的话能够在屏幕正中央显示*/
    printf("Enter any char to start the game!");
}

void main() {
    int i = 0;
    int headXPos = DEFAULT_X;
    int headYPos = DEFAULT_Y;
    int key;
    int tempIndex;
    int newKey;
    int cdTime = DEFAULT_COUNT;
    int snakeHeadType = HEAD_RIGHT_INDEX;
    boolean finished = FALSE;
    SNAKE player = {0, 5, 1, 3, NULL};

    DELTA_MOVE delta[4] = {
    	{0, -1},			 /*上*/  
        {0, 1},             /*下*/              
        {-1, 0},            /*左*/
        {1, 0}};            /*右*/

    SNAKE_BODY snakeBody[MAX_LEN] = {{0,0}};

    hideCursor();
    
    init();
    getch();
    snakeBody[0].xPostion = headXPos;
    snakeBody[0].yPostion = headYPos;
    player.sb = snakeBody;

    clrscr();
    showBorder();
    while((!isFinished(headXPos, headYPos))) {
        i++;
        key = bioskey(1);
        if(key != 0){
            newKey = bioskey(0);
            if(newKey == ESC) {
                break;
            }
            tempIndex = readValidOrder(newKey, &cdTime, &player);
            if(tempIndex >= 0 && tempIndex <= 3) {
                player.direct = tempIndex;
                snakeHeadType = getHeadType(player.direct);
            }
        }
        if(i > cdTime) {
            finished = dealEat(&player);
            if(finished == TRUE) {
                break;
            }
            move(&headXPos, &headYPos, delta + player.direct, snakeHeadType, &player);
            if(isFinished(headXPos, headYPos) == TRUE) {
                break;
            }

            if(eatUpFood == TRUE) {
                dealFood();
            }
        	i = 0;
        }
    }
    clrscr();
    gotoxy(40, 12);
    printf("Game over!");
    getch();
}