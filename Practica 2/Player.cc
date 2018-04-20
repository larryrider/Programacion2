#include "Player.h"
#include <sys/stat.h> // mkfifo
#include <unistd.h>   // unlink
#include <cstdio>     // remove
#include <cstdlib>    // system

const string kFIFO="/tmp/p2fifofile";

// Funcion para reproducir una cancion. Necesita una URL como parámetro.
void playSong(string url, Player &player)
{
  // Cerrar y limpiar fichero anterior si ya existia
  if (player.fifo.is_open()) 
  {
    player.fifo << "quit" << endl;
    player.fifo.close();
  }
  remove(kFIFO.c_str());

  if (url!="")
  {
    // Crear fichero fifo de comunicacion con mplayer
    if (mkfifo(kFIFO.c_str(), 0666 ) < 0 ) 
    {
      cout << "Error opening file " << kFIFO << endl; 
    }
    else 
    {
      string mplayercommand="mplayer -idle -slave -quiet -input file=" + kFIFO + " " + url + " >/dev/null 2>/dev/null&";

      system(mplayercommand.c_str()); // Ejecutar mplayer
      player.fifo.open(kFIFO.c_str()); // Abrir fichero fifo para enviar mensajes a mplayer
      if (!player.fifo.is_open()) 
          cout << "Error opening file " << kFIFO << endl;
    }
  }
  else cout << "No URL for this song" << endl;
}

// Funcion para pausar o quitar la pausa a una cancion
void pauseResumeSong(Player &player)
{
  if (player.fifo.is_open())
    player.fifo << "pause" << endl;
}

// Importante: Llamar a esta funcion una vez se ha terminado de usar el reproductor
void endPlayer(Player &player)
{
   if (player.fifo.is_open())
   {
     player.fifo << "quit" << endl;
     player.fifo.close();
     unlink(kFIFO.c_str());
   }
}
