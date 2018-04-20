#ifndef _PLAYERC_H_
#define _PLAYERC_H_

#include <iostream>
#include <fstream>

using namespace std;

typedef struct
{
  fstream fifo;
} Player;

// Funcion para reproducir una cancion. Necesita una URL como parámetro.
void playSong(string url, Player &player);
// Funcion para pausar o quitar la pausa a una cancion
void pauseResumeSong(Player &player);
// Importante: Llamar a esta funcion una vez se ha terminado de usar el reproductor
void endPlayer(Player &player);

#endif
