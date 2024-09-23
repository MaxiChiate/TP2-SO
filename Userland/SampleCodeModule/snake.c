#include <userlib.h>
#include <snake.h>
#include <frontSnake.h>
#include <defs.h>

#define TICKS_UNTIL_PRINT 300000

void startSnake(unsigned int players){

    setPlayers(players);

    if(incorrectplayers()){
        print("Error en la cantidad de jugadores\n");
        return;
    }

    tSnake snake1;
    tSnake snake2;
    tApple myApple; 

    spawnSnake(snake1,1);
    if(getPlayers()==2) {
        spawnSnake(snake2, 2);
    }

	spawnApple(myApple, snake1, snake2);

    drawMap();
    drawApple(myApple);

    unsigned char key;
    int ticks = TICKS_UNTIL_PRINT;

    while(1)    {

        if(ticks++ == TICKS_UNTIL_PRINT)   {
            
            ticks = 0;        

            if(creep(snake1, myApple, snake2->body[snake2->headPos]))
                return;

            if(snake1->eating)   {
                cleanSankeText();
                printSnakeScore(snake1);
                spawnApple(myApple, snake1, snake2);         
                drawApple(myApple);
                
            }
    
            if(getPlayers()==2) {
                
                if(creep(snake2, myApple, snake1->body[snake1->headPos]))
                    return;
            
                if(snake2->eating)   {
                    cleanSankeText();
                    printSnakeScore(snake2);
                    spawnApple(myApple, snake1, snake2);
                    drawApple(myApple);
                }
            }

            drawSnake(snake1);
            if(getPlayers()==2)
                drawSnake(snake2);



        }

        key = getChar();

        if(isSnakeKey(key))  {

            if(isUpper(key))    {
                key += 32;  // Lo paso a minusc.
        }
            if( key == 'p')     {

                cleanSankeText();
                clear();
                print("PAUSED");
            // Espera:
                while(getChar()!='p');

                cleanSankeText();
                printSnakeScore(snake1);
                if(getPlayers()==2)
                    printSnakeScore(snake2);


                drawMap();
                drawApple(myApple);
                
        }    
            else if(!useKey(snake1, key, keysSnake1) && getPlayers()==2)    {

                useKey(snake2, key, keysSnake2);
        }            
        
        }

    }

}

int creep(tSnake mySnake, tApple myApple, struct snakeBody otherSnakeHead)   {

    char looser;

    if( (looser = moveSnake(mySnake, otherSnakeHead)) == 0) {
    // Sin colision.
        
                
            feedSnake(myApple, mySnake);
            return 0;
    }
    else if (looser != 3)   {

            play_song(4);

            clear();
            print("Snake ");    
            if(looser == 1)
                printUinteger(mySnake->id);
            else
                printUinteger(mySnake->id==1? 2 : 1);
                
            print(" just crashed\n");
            while(getChar()!='\n');

            print("Snake ");   
            if(looser == 1)
                printUinteger(mySnake->id);
            else
                printUinteger(mySnake->id==1? 2 : 1);
            print(" is a looser\n");
            while(getChar()!='\n');

            print("Don't be like Snake ");    
            
            if(looser == 1)
                printUinteger(mySnake->id);
            else
                printUinteger(mySnake->id==1? 2 : 1);
            while(getChar()!='\n');
            clear();
        }
    else    {
            
            play_song(4);
            clear();
            
            print("Draw\n");
            while(getChar()!='\n');
            print("Both are extremely bad");
            while(getChar()!='\n');
            clear();
    }
    
    return 1;
    
}

int useKey(tSnake mySnake, unsigned char key, unsigned char * snakeKeys) {
    
    if (key == snakeKeys[0])    {

        changeSnakeDirection(mySnake, LEFT);
        return 1;
} 
    else if (key == snakeKeys[1])   {
        // Notar que la matriz que imprimer los pixeles crece para abajo.
        changeSnakeDirection(mySnake, UP);
        return 1;
} 
    else if (key == snakeKeys[2])   {
    
        changeSnakeDirection(mySnake, DOWN);
        return 1;
} 
    else if (key == snakeKeys[3])   {

        changeSnakeDirection(mySnake, RIGHT);
        return 1;
} 
    
// No es ninguna de las anteriores.
    return 0;
    
}

void spawnSnake(tSnake babySnake, unsigned int id)   {
    
    babySnake->headPos = 1;
    babySnake->eating = 0;
    babySnake->id = id;

    for(int i=0; i<=babySnake->headPos; i++)  {
        babySnake->body[i].x = (COLUMNS / 2) - 1 + i;
    // Consideracion para que no spawneen las snakes en la misma posicion.
        babySnake->body[i].y = (ROWS / 2) + 2*babySnake->id;
        babySnake->body[i].direction = RIGHT;
    }    


}


void spawnApple(tApple apple, tSnake snake1, tSnake snake2)   {
    int x; 
    int y; 
    int tryAgain = 1;
        int i;

    while(tryAgain) {

        x = randInt(1, ROWS-1);
        y = randInt(1, COLUMNS-1);

            i=0;

            do      {          
                tryAgain = snake1->body[i].x == x && snake1->body[i].y == y;
                ++i;
            }
            while(i<=snake1->headPos && !tryAgain);
        
            while(i<=snake2->headPos && !tryAgain)      {          
                tryAgain = snake2->body[i].x == x && snake2->body[i].y == y;
                ++i;
            }
            

    } 

    apple->x = x;
    apple->y = y;

}

static int isOpositeDirection(tDirection dir1, tDirection dir2) {
    
    return  (dir1 == RIGHT && dir2 == LEFT) || (dir1 == LEFT && dir2 == RIGHT) ||
            (dir1 == UP && dir2 == DOWN) || (dir1 == DOWN && dir2 == UP);
            
}

void changeSnakeDirection(tSnake snake, tDirection newDirection)   {
    
// Si la direccion es opuesta a la direccion a la que va el cuerpo proximo a la cabeza, no deberia cambiar de direccion.
// Si es solo una cabeza se contempla que se pueda mover en cualquier direccion.

    if (snake->headPos==0 || 
        !isOpositeDirection(snake->body[snake->headPos-1].direction, newDirection)) 
     
           snake->body[snake->headPos].direction = newDirection;
    
   
}

static void refreshSnakeDirections(tSnake snake)    {
    for(int i=0; i<snake->headPos; ++i) 
        snake->body[i].direction = snake->body[i+1].direction;
}

static void moveBody(tSnake snake, unsigned int bodyPos) {
    
    switch(snake->body[bodyPos].direction)   {

        case RIGHT: {
            snake->body[bodyPos].x++;
            break;
        }
        case LEFT: {
            snake->body[bodyPos].x--;
            break;
        }
        case DOWN: {
            snake->body[bodyPos].y--;
            break;
        }
        case UP: {
            snake->body[bodyPos].y++;
            break;
        }
        
        default:    {
            print("Error in moveSnake function: Direction not defined\n");
            break;
        }
    }

}

int checkCrash(tSnake snake, struct snakeBody otherSnakeHead )    {
    for(int i=0; i<snake->headPos; ++i) {
    // Checkea cuerpito por cuerpito 
        if (snake->body[snake->headPos].x == snake->body[i].x &&
            snake->body[snake->headPos].y == snake->body[i].y)  {
                // Pierde @snake.
                return 1;
            }
        if(getPlayers()==2 && otherSnakeHead.x == snake->body[i].x &&
            otherSnakeHead.y == snake->body[i].y)   {
                // Pierde @otherSnake.
                return 2;
            }
            
    }

    if(getPlayers()==2 && otherSnakeHead.x  == snake->body[snake->headPos].x &&
            otherSnakeHead.y == snake->body[snake->headPos].y)
        // Empate tecnico.
            return 3;

// Checkear si toco una pared es simplement checkear si la cabeza toco la pared
// pues el cuerpo nunca debería poder hacerlo.
    
    return snake->body[snake->headPos].x == 0       || 
        snake->body[snake->headPos].x == COLUMNS    ||
        snake->body[snake->headPos].y == 0          ||
        snake->body[snake->headPos].y == ROWS;
}

int moveSnake(tSnake snake, struct snakeBody otherSnakeHead)    {
   

    if(snake->eating)  {
        play_song(3);
        int auxX = snake->body[snake->headPos].x; 
        int auxY = snake->body[snake->headPos].y; 
        tDirection auxDirection = snake->body[snake->headPos].direction;

        moveBody(snake, (snake->headPos)++);

        snake->body[snake->headPos] = snake->body[snake->headPos-1];

        snake->body[snake->headPos-1].x = auxX;
        snake->body[snake->headPos-1].y = auxY;
        snake->body[snake->headPos-1].direction = auxDirection;

        snake->eating = 0;
    }
     
    else    {
        
    // Para borrar la posicion anterior de la cola.
        drawBLackSquare(snake->body[0].x, snake->body[0].y);

        for(int i=0; i<=snake->headPos; ++i) 
            moveBody(snake, i);

        refreshSnakeDirections(snake);

    }    
    
    return checkCrash(snake, otherSnakeHead);
}


void feedSnake(tApple apple, tSnake snake)   {

    if (snake->body[snake->headPos].x == apple->x && 
    snake->body[snake->headPos].y == apple->y)  {
    
        snake->eating = 1;
    }
        
}


int isSnakeKey(unsigned char key)    {
    return snakeKeys[key];
}
