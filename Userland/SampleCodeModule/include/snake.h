#ifndef _SNAKE_H
#define _SNAKE_H

    #define ROWS 30
    #define COLUMNS 30
    #define SLOTS ROWS*COLUMNS

/*---------------Players managment---------------------*/
    static unsigned int playersInGame = 0;           /**/
    #define getPlayers() (playersInGame)             /**/
    #define setPlayers(x) (getPlayers() = (x))       /**/
                                                     /**/
    static int incorrectplayers()    {               /**/
        return getPlayers()>2 || getPlayers() == 0;  /**/
    }                                                /**/
/*-----------------------------------------------------*/

    typedef enum Direction { RIGHT, LEFT, DOWN, UP} tDirection;

    struct snakeBody {

        unsigned int x;
        unsigned int y;
        tDirection direction;
    };

    typedef struct  {

        struct snakeBody body[SLOTS];

        unsigned int id;
        unsigned int headPos;
        unsigned char eating;           // flag.

    } * tSnake;

    typedef struct  {

        unsigned int x;
        unsigned int y;
    } * tApple;


// Comienza el juego.
    void startSnake(unsigned int players);

// Acciona. Devuelve 1 si muere.
    int creep(tSnake mySnake, tApple myApple, struct snakeBody otherSnakeHead);

    int useKey(tSnake mySnake, unsigned char key, unsigned char * snakeKeys);

    int checkCrash(tSnake snake, struct snakeBody otherSnakeHead);

//  Instancia snake.
    void spawnSnake(tSnake babySnake, unsigned int id);

    void changeSnakeDirection(tSnake snake, tDirection newDirection);

// moveSnake():

//  Aumenta una posicion en la direccion que se esta moviendo
//  cada uno de los cuerpos.

//  Debe llamarse en cada segundo del juego independientemente de que 
//  se haya llamaod a changeSnakeDirection().

//  Retorna 1 si hubo una colision entre la cabeza y algun cuerpo o muro.

    int moveSnake(tSnake snake, struct snakeBody otherSnakeHead);

    void spawnApple(tApple apple, tSnake snake1, tSnake snake2);

    void feedSnake(tApple apple, tSnake snake);

//  Devuelve 1 si es una tecla valida, validan los ascii de keysSnake1, keysSnake2 y 'p'.
//  El criterio de validacion esta predefinido en el archivo defs.h.

    int isSnakeKey(unsigned char key);

static unsigned char keysSnake1[4] = {'a', 's', 'w', 'd'};
static unsigned char keysSnake2[4] = {'j', 'k', 'i', 'l'};

#endif