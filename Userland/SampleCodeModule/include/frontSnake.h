// frontSnake.h

#ifndef _FRONTSNAKE_H
#define _FRONTSNAKE_H

    #include <snake.h>

    void drawMap();

    void drawSnake(tSnake snake);
    void drawApple(tApple apple);
    

    // Simula @iterations spawns de manzanas en snake.
    // Si @iterations = 0, simula infinitamente.
    // void drawAppleSimulation(unsigned int iterations);

    void printSnakeScore(tSnake snake);

    // printSnakeInfo():

    //  Imprime la informacion de la serpiente.
    //  Los bodies se cuentan de menor a mayor desde la cabeza hasta la cola.
    void printSnakeInfo(tSnake snake);
    
    // Lo mismo sin enters :)
    void printSnakeInfo2(tSnake snake);

    void cleanSankeText();

    void printAppleInfo(tApple apple);

#endif