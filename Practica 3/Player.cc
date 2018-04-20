#include "Player.h"
#include <sys/stat.h> // mkfifo
#include <unistd.h>   // unlink
#include <cstdio>     // remove
#include <cstdlib>    // system

const string kFIFO="/tmp/p2fifofile";

// Funcion para reproducir una cancion. Necesita una URL como parámetro.
void Player::playSong(string url)
{
  // Cerrar y limpiar fichero anterior si ya existia  
  if (fifo.is_open()) 
  {
    fifo << "quit" << endl;
    fifo.close();
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
      fifo.open(kFIFO.c_str()); // Abrir fichero fifo para enviar mensajes a mplayer
      if (!fifo.is_open()) 
          cout << "Error opening file " << kFIFO << endl;
    }
  }
  else cout << "No URL for this song" << endl;
}

// Funcion para pausar o quitar la pausa a una cancion
void Player::pauseResumeSong()
{
  if (fifo.is_open())
    fifo << "pause" << endl;
}

// Destructor (equivalente a endPlayer en pr2)
Player::~Player()
{
   if (fifo.is_open())
   {
     fifo << "quit" << endl;
     fifo.close();
     unlink(kFIFO.c_str());
   }
}
