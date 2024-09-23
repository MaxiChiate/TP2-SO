// frontSnake.c
#include <frontSnake.h>
#include <snake.h>
#include <userlib.h>

void drawMap()  {
    clear();
    for(int i=0; i<=COLUMNS; ++i)    {
        drawWhiteSquare(i, 0);
        drawWhiteSquare(i, ROWS);
    }
    for(int i=0; i<=ROWS; ++i)    {
        drawWhiteSquare(0, i);
        drawWhiteSquare(COLUMNS, i);
    }

    print("Press p for pause.\nPlease, consider use pause before checking registers.\n");
}


void drawSnake(tSnake snake)  {
    if(snake->id == 1)
        drawGreenSquare(snake->body[snake->headPos-1].x, snake->body[snake->headPos-1].y);

    else    
        drawYellowSquare(snake->body[snake->headPos-1].x, snake->body[snake->headPos-1].y);
    

    int headModel;
    switch(snake->body[snake->headPos].direction)    {
        
        case LEFT :    {
            headModel = 0;
            break;
        }
        case RIGHT :    {
            headModel = 1;
            break;
        }
        case UP :    {
            headModel = 3;
            break;
        }
        case DOWN :    {
            headModel = 2;
            break;
        }
    }
    if(snake->id == 1)
        
        draw(headModel, snake->body[snake->headPos].x, snake->body[snake->headPos].y, 0x01);

    else
        
        draw(headModel, snake->body[snake->headPos].x, snake->body[snake->headPos].y, 0x02);

}


void drawApple(tApple apple)    {
    draw(0x05, apple->x, apple->y, 0x00);
}



static void printDirection(tDirection direction) {

    switch(direction) {
        
        case RIGHT: {
            print("RIGHT");
            break;
        }
        case LEFT: {
            print("LEFT");
            break;
        }
        case DOWN: {
            print("DOWN");
            break;
        }
        case UP: {
            print("UP");
            break;
        }
        
        default:    {
            print("Direction not defined");
            break;
        }
    }
}



static void printSnakeBody(struct snakeBody body)    {

    putEnter();

    print("x: ");   printUinteger(body.x);
    putEnter();
    print("y: ");   printUinteger(body.y);
    putEnter();
    print("Direction: ");   printDirection(body.direction);

    putEnter();
}


void printSnakeInfo(tSnake snake)   {

    print("Snake"); printUinteger(snake->id);   putEnter();
    print("Snake length: ");   printUinteger(snake->headPos+1);
    putEnter();
    print("Snake is");
    snake->eating ? print(" ") : print(" not ");
    print("eating");  
    putnEnters(2);

    print("Head info: ");
    printSnakeBody(snake->body[snake->headPos]);

}

void printSnakeInfo2(tSnake snake) {
    
    print("id:");
    printUinteger(snake->id);       putChar(' ');
    print("l:");
    printUinteger(snake->headPos+1);putChar(' ');
    print("e?");
    printUinteger(snake->eating);   putChar(' ');
    print("x:");
    printUinteger(snake->body[snake->headPos].x);   putChar(' ');
    print("y:");
    printUinteger(snake->body[snake->headPos].y);   putChar(' ');
    

}

void cleanSankeText()  {
    for(int i=0; i<200; ++i) {
        putBackSpace();
    }
}

void printSnakeScore(tSnake snake)  {


    print("Snake ");  printUinteger(snake->id);
    print(": ");    printUinteger(snake->headPos);

}

void printAppleInfo(tApple apple)    {

    print("Apple: ");

    putEnter();

    print("x: ");   printUinteger(apple->x);
    putEnter();
    print("y: ");   printUinteger(apple->y);

    putEnter();


}