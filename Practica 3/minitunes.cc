// Guerrero Selva, Margarita
// Rider Garcia, Lawrence Arthur

#include <iostream>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include "Player.h"
#include "Song.h"
#include "Constants.h"
#include "Collection.h"
#include "Playlist.h"
#include "Utils.h"

using namespace std;


int main(int argc, char *argv[]){
	char opcion;
	
	Collection collection;
	Song cancion;
	Playlist playlist(&collection);
	string filename="";
	
	if (Utils::manageArguments(argc, argv, collection, playlist)==true){
		do{
			cout<<"--------------------------------\n----------- MiniTunes  ---------";
			cout<<"\n--------------------------------\n";
			cout<<"1- Add song\n2- Edit song\n3- Delete song\n4- Show collection\n5- Manage playlist";
			cout<<"\n6- Import iTunes JSON data\n7- Export playlist to XSPF\n8- Save data\n9- Load data";
			cout<<"\n0- Play playlist\nq- Quit\n";
			cout<<"Option: ";
			cin>>opcion;
			Utils::limpiarBuffer();
			switch(opcion){
				case '1':
					collection.addSong();
					break;
				case '2':
					collection.editSong();
					break;
				case '3':
					collection.deleteSong();
					break;
				case '4':
					collection.show();
					break;
				case '5':
					playlist.manage();
					break;
				case '6':
					collection.importJSON(filename);
					break;
				case '7':
					playlist.saveXSPF();
					break;
				case '8':
					Utils::saveData(collection, playlist);
					break;
				case '9':
					Utils::loadData(collection, playlist);
					break;
				case '0':
					playlist.play();
					break;
				case 'q':
					break;
				default:
					cout<<ERRNOOPTION<<endl;
					break;
			}
		}while (opcion!='q');
	}
}