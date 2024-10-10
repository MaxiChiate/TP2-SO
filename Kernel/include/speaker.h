#ifndef _SPEAKER_H
#define _SPEAKER_H

// poner un sonido
void play_sound(int nFrequence);

// apagar el sonido
void nosound();

// hacer beep
void beep();

//  pone un sonido durante un tiempo
void playFrecuency(double time, unsigned int frecuency);

//pone la música del tetris
void playTetris();

//pone la canción de jingle bells
void playJingleBells();

//pone death song de mario.
void playMario();
#endif