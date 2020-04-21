#include <stdlib.h>
#include <conio.h>
#include <stdio.h>
#include <BIOS.H>
#include <time.h>
#include <dos.h>

#include "duoSnake.h"

int foodNum = 0;
boolean eatUpFood = TRUE;
int cdTime = DEFAULT_COUNT;
char snakeHead[4] = {'^', 'v', '<', '>'};
int screenPoint[Full_Screen_COUNT] = {0};
DELTA_MOVE delta[4] = {
        {0, -1},             /*上*/  
        {0, 1},             /*下*/              
        {-1, 0},            /*左*/
        {1, 0}};            /*右*/

void dealTwoPlayer() {
    int i = 0;
    int headXPos = DEFAULT_X;
    int headYPos = DEFAULT_Y;
    int key;
    int tempIndex1;
    int tempIndex2;
    int newKey;
    int snakeHeadType1 = HEAD_RIGHT_INDEX;
    int snakeHeadType2 = SNAKE_HEAD_RIGHT;

    boolean finished1 = FALSE;
    boolean finished2 = FALSE;
    SNAKE player1 = {0, 5, 1, 3, NULL};
    SNAKE player2 = {0, 5, 1, 3, NULL};

    SNAKE_BODY snakeBody1[MAX_LEN] = {{0,0}};
    SNAKE_BODY snakeBody2[MAX_LEN] = {{0,0}};

    snakeBody1[0].xPostion = headXPos + 20;
    snakeBody1[0].yPostion = headYPos;
    player1.sb = snakeBody1;
    snakeBody2[0].xPostion = headXPos - 20;
    snakeBody2[0].yPostion = headYPos;
    player2.sb = snakeBody2;

    showBorder();
    while((!isFinished(headXPos, headYPos))) {
        i++;
        key = bioskey(1);
        if(key != 0) {
            newKey = bioskey(0);
            if(newKey == ESC) {
                break;
            }
            tempIndex1 = readValidOrder(newKey, &player1);
            if(tempIndex1 >= 0 && tempIndex1 <= 3) {
                player1.direct = tempIndex1;
                snakeHeadType1 = getHeadType1(player1.direct);
            }
            tempIndex2 = readValidOrder(newKey, &player2);
            if(tempIndex2 >= 4 && tempIndex2 <= 7) {
                player2.direct = tempIndex2 % 4;    /*因为本人设定：第二条蛇的方向的下标时从4到7的，所以这里要对4取余*/
                snakeHeadType2 = getHeadType2(player2.direct);
            }
        }
        if(i > cdTime) {
            finished1 = dealEat(&player1);
            if(finished1 == TRUE) {
                break;
            }
            dealmove(&headXPos, &headYPos, delta + player1.direct, snakeHeadType1, &player1);
            if((finished1 = isFinished(headXPos, headYPos)) == TRUE) {
                break;
            }
            finished2 = dealEat(&player2);
            dealmove(&headXPos, &headYPos, delta + player2.direct, snakeHeadType2, &player2);
            if((finished2 = isFinished(headXPos, headYPos)) == TRUE) {
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
    printf(finished1 == finished2 ? "Game over!" : (finished2 ? "Winer : player1 !" : "Winer : player2 !"));
    getch();
}

void dealOnePlayer() {
    int i = 0;
    int headXPos = DEFAULT_X;
    int headYPos = DEFAULT_Y;
    int key;
    int tempIndex;
    int newKey;
    int snakeHeadType = HEAD_RIGHT_INDEX;

    boolean finished = FALSE;
    SNAKE player = {0, 5, 1, 3, NULL};
    SNAKE_BODY snakeBody[MAX_LEN] = {{0,0}};

    snakeBody[0].xPostion = headXPos;
    snakeBody[0].yPostion = headYPos;
    player.sb = snakeBody;

    showBorder();
    while((!isFinished(headXPos, headYPos))) {
        i++;
        key = bioskey(1);
        if(key != 0){
            newKey = bioskey(0);
            if(newKey == ESC) {
                break;
            }
            tempIndex = readValidOrder(newKey, &player);
            if(tempIndex >= 0 && tempIndex <= 3) {
                player.direct = tempIndex;
                snakeHeadType = getHeadType1(player.direct);
            }
        }
        if(i > cdTime) {
            finished = dealEat(&player);
            if(finished == TRUE) {
                break;
            }
            dealmove(&headXPos, &headYPos, delta + player.direct, snakeHeadType, &player);
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
char getHeadType1(int snakeHeadIndex) {
    return snakeHead[snakeHeadIndex];
}

int getHeadType2(int snakeHeadIndex) {
    switch(snakeHeadIndex) {
        case 0:     return SNAKE_HEAD_UP;     /*上*/
        case 1:     return SNAKE_HEAD_DOWN;  /*下*/
        case 2:     return SNAKE_HEAD_LEFT;  /*左*/
        case 3:     return SNAKE_HEAD_RIGHT; /*右*/
    }
}

/*使得蛇能够向前移动*/
void dealmove(int *headXPos, int *headYPos, DELTA_MOVE *delta, char headType, SNAKE *snake) {
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
int readValidOrder(int key, SNAKE *snake) {
    int tmp;

    tmp = snake->direct;
    switch(key) {
        case UP:    return tmp == 1 ? 1 : 0;
        case DOWN:  return tmp == 0 ? 0 : 1;
        case LEFT:  return tmp == 3 ? 3 : 2;
        case RIGHT: return tmp == 2 ? 2 : 3;    /*上面的返回值分别对应“上”、“下”、“左”、“右”在“蛇头类型数组”中的下标*/
        case W:     return tmp == 5 ? 5 : 4;
        case S:     return tmp == 4 ? 4 : 5;
        case A:     return tmp == 7 ? 7 : 6;
        case D:     return tmp == 6 ? 6 : 7;

        case PGUP:  
            if(cdTime > MIN_COUNT) {
                cdTime = cdTime / 2;
            }
            return -1;
        case PGDN:  
            if(cdTime < MAX_COUNT) {
                cdTime = cdTime * 2;
            }
            return -1;
    }
}

/*判断蛇头是否撞墙*/
boolean isFinished(int headX, int headY) {
    if(WALL != screenPoint[(headY-1)*MAX_X + headX-1]) {
        return FALSE;
    }
        return TRUE;
}

/*显示“墙壁”*/
void showBorder() {
    int i;

    clrscr();
    gotoxy(22, 12); /*这里的行、列值完全是一个点一个点测出来的，为了使下面的话能够在屏幕正中央显示*/
    printf("Enter any char to start the game!");
    getch();

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
boolean init() {
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
    
    return readPlayerNum();
}

/*游戏模式选择*/
boolean readPlayerNum() {
    int result;

    clrscr();
    gotoxy(12, 9);
    printf("******************************************************");
    gotoxy(12, 10);
    printf("*Enter the number '1' to start the single player mode*");
    gotoxy(12, 11);
    printf("*Enter the number '2' to start the   two-person  mode*");
    gotoxy(12, 12);
    printf("******************************************************");
    gotoxy(23, 14);
    printf("your choose is : ");
    scanf("%d", &result);
    while((result != 1) && (result != 2)) {
        gotoxy(12, 14);
        printf("\nerror enter : Please follow the prompts to enter: ");
        scanf("%d", &result); 
    }
    return result == 1 ? TRUE : FALSE;
}

/*隐藏光标*/
void hideCursor() { /*这个函数无需理解，因为本人想要做的比较美观，才在网上查询得此代码段使得光标被隐藏*/
    union REGS r;
    r.h.ah = 1;
    r.h.ch = 15;
    r.h.cl = 0;
    int86(0x10,&r,&r);
}

int main() {
    boolean modelChoose = FALSE;

    hideCursor();
    modelChoose = init();
    if(modelChoose) {
        dealOnePlayer();
    } else {
        dealTwoPlayer();
    }

    return 0;
}