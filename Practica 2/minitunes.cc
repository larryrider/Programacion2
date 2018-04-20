// Guerrero Selva, Margarita
// Rider Garcia, Lawrence Arthur

#include <iostream>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include "Player.h"

using namespace std;

const string ERRNOSONG = "Error: Unknown song ";
const string ERRNOOPTION = "Error: Unknown option\n";
const string TITLE = "Title: ";
const string ARTIST = "Artist: ";
const string ALBUM = "Album: ";
const string GENRE = "Genre: ";
const string URL = "Url: ";
const string THESONG = "The song ";
const string SELSONG = "Select song: ";
const string ISINCOLLECTION = " is already in the collection\n";
const string SEARCH = "Search: ";
const string ERRNORESULTS = "No results\n";
const string EDIT = "Edit (1-Title, 2-Artist, 3-Album, 4-Genre, 5-Url): ";
const string DELETE = "Delete ";
const string CONFIRM = "? (Y/N): ";
const string NOTDELETED = "Song not deleted\n";
const string SELSONGSEP = "Select songs (list separated by spaces):";
const string EMPTYCOL = "Empty collection";
const string EMPTYPLAY = "Empty playlist\n";
const string ERRSONG = "Error: Song ";
const string NOTPLAY = " not in playlist";
const string CLEARPLAY = "Clear playlist? (Y/N): ";
const string FICHERR = "Error opening file ";
const string FILEN = "Filename: ";
const string FINDKINDSONG="\"kind\":\"song\"";
const string TRACKNAME="\"trackName\":\"";
const string COLLENAME="\"collectionName\":\"";
const string ARTISTNAME="\"artistName\":\"";
const string GENRENAME="\"primaryGenreName\":\"";
const string URLNAME="\"previewUrl\":\"";
const string CONFIRMDELCOL="Delete previous collection? (Y/N):";
const char FILEBINARY[]="minitunes.dat";
const string COMMAND="Command: ";
const string ERRSYNTAX="Syntax: ./minitunes [-d] [-i jsonfilename]";

const int kTITLE=30;
const int kARTIST=60;
const int kALBUM=60;
const int kGENRE=30;
const int kURL=255;

typedef struct{
	int id; // El identificador de una nueva cancion debe ser igual a idNextSong
	string title;
	string artist;
	string album;
	string genre;
	string url;
} Song;

typedef struct{
	vector<Song> songs;
	int idNextSong;  // Cada vez que se añada una cancion a la coleccion, incrementar idNextSong
} Collection;

typedef struct{
	vector<int> idSong;
} Playlist;

typedef struct{
	int id;
	char title[kTITLE];
	char artist[kARTIST];
	char album[kALBUM];
	char genre[kGENRE];
	char url[kURL];
} SongBin;


// Función para inicializar los valores de la colección
Collection createCollection(){
	Collection collection;
	collection.idNextSong=1;
	
	return (collection);
}

// Procedimiento para limpiar el buffer de entrada
void limpiarBuffer(){
	while(cin.get() !='\n');
} 

// Declaracion de funciones
Song demandSong();
bool isSongInCollection(const Collection &collection, Song cancion);
bool addSong(Collection &collection);
void printSong(Song cancion);
bool showCollection(const Collection &collection);
int findIdSong(const Collection &collection, int id);
void editSong(Collection &collection);
void deleteSong(Collection &collection);

void managePlaylist(Playlist &playlist, const Collection &collection);
void addSongsPlaylist(Playlist &playlist, const Collection &collection);
vector<int> separa();
void printPlaylist(const Playlist &playlist, const Collection &collection, int position);
void printPlaylistSong(Song cancion);
void removeSongsPlaylist(Playlist &playlist, const Collection &collection);
void clearSongsPlaylist(Playlist &playlist);

bool importJSON(Collection &collection, string filename);
Song getSongFromJSONline(string JSONline);
string getNameFromLine(unsigned int &pos, string JSONline);
bool addSongToCol(Song cancion, Collection &collection);

bool saveXSPFPlaylist(const Playlist &playlist, const Collection &collection);
void printXSPFSong(Song cancion, ofstream &fichero);

bool saveData(const Collection &collection, const Playlist &playlist);
bool writePlaylist(const Playlist &playlist, ofstream &fichero);
bool writeCollection(const Collection &collection, ofstream &fichero);
SongBin songToBinary(const Song &cancion);

bool loadData(Collection &collection, Playlist &playlist);
void deleteCollection(Collection &collection);
bool readPlaylist(Playlist &playlist, ifstream &fichero);
bool readCollection(Collection &collection, ifstream &fichero);
Song binaryToSong(const SongBin &cancionbin);

void playPlaylist(const Playlist &playlist, const Collection &collection);

bool manageArguments(int argc, char *argv[], Collection &collection, Playlist &playlist);


int main(int argc, char *argv[]){
	char opcion;
	
	Collection collection=createCollection();
	Song cancion;
	Playlist playlist;
	string filename="";
	
	if (manageArguments(argc, argv, collection, playlist)==true){
		do{
			cout<<"--------------------------------\n----------- MiniTunes  ---------";
			cout<<"\n--------------------------------\n";
			cout<<"1- Add song\n2- Edit song\n3- Delete song\n4- Show collection\n5- Manage playlist";
			cout<<"\n6- Import iTunes JSON data\n7- Export playlist to XSPF\n8- Save data\n9- Load data";
			cout<<"\n0- Play playlist\nq- Quit\n";
			cout<<"Option: ";
			cin>>opcion;
			
			switch(opcion){
				case '1':
					addSong(collection);
					break;
				case '2':
					editSong(collection);
					break;
				case '3':
					deleteSong(collection);
					break;	
				case '4':
					showCollection(collection);
					break;	
				case '5':
					managePlaylist(playlist, collection);
					break;	
				case '6':
					importJSON(collection, filename);
					break;	
				case '7':
					saveXSPFPlaylist(playlist,collection);
					break;	
				case '8':
					saveData(collection, playlist);
					break;	
				case '9':
					loadData(collection, playlist);
					break;
				case '0':
					playPlaylist(playlist, collection);
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

Song demandSong(){
	Song cancion;
	
	limpiarBuffer();
	
	cout<<TITLE; 
	getline(cin,cancion.title);
	
	cout<<ARTIST;
	getline(cin,cancion.artist);
	
	cout<<ALBUM;
	getline(cin,cancion.album);
	
	cout<<GENRE;
	getline(cin,cancion.genre);
	
	cout<<URL;
	getline(cin,cancion.url);

	return cancion;
}

bool isSongInCollection(const Collection &collection, Song cancion){
	unsigned int i;
	bool encontrada=false;
	
	for(i=0;i<collection.songs.size() && encontrada==false;i++){   //Se encarga de comparar una cancion en el vector canciones y devolver true si se encuentra
		if (collection.songs[i].title==cancion.title){
			if (collection.songs[i].artist==cancion.artist){
				encontrada=true;
			}	
		}
	}
	return(encontrada);
}

bool addSong(Collection &collection){
	Song cancion;
	bool repetida;
	bool anyadir;
	
	cancion=demandSong();
	repetida=isSongInCollection(collection, cancion);
	
	if (repetida==false){                                 //Si hay una cancion repetida no se añade a la coleccion, en caso contrario si se añade
		cancion.id=collection.idNextSong;
		collection.songs.push_back(cancion);
		collection.idNextSong++;
		anyadir=true;
	} else{
		cout<<THESONG<<cancion.title <<ISINCOLLECTION;
		anyadir=false;
	}
	return (anyadir);
}

void printSong(Song cancion){
	cout<<cancion.id<<" | "<<cancion.title;
	cout<<" | "<<cancion.artist<<" | "<<cancion.album;
	cout<<" | "<<cancion.genre;
}
 
bool showCollection(const Collection &collection){
	string buscar;
	unsigned int i;
	Song cancion;
	bool encontrada;
	int contador=0;
	
	limpiarBuffer();
	
	cout<<SEARCH;
	getline(cin,buscar);
		
	for(i=0;i<collection.songs.size();i++){
		if(collection.songs[i].title.find(buscar) != string::npos || collection.songs[i].artist.find(buscar) != string::npos || 
		   	collection.songs[i].album.find(buscar) != string::npos || collection.songs[i].genre.find(buscar) != string::npos){
			contador++;
			printSong(collection.songs[i]);
			cout<<endl;
		}
	}
	
	if (contador==0){
		cout<<ERRNORESULTS;
		encontrada=false;
	} else{
		encontrada=true;
	}
	return (encontrada);
}

int findIdSong(const Collection &collection, int id){
	unsigned int i;
	int posicion=-1;
	bool salir=false;
	
	for(i=0;i<collection.songs.size() && salir==false;i++){
		if (id==collection.songs[i].id){
			posicion=i;
			salir=true;
		}
	}
	return(posicion);
}

void editSong(Collection &collection){
	bool mostrar;
	int buscarid;
	int posicion;
	int opcion;
	Song cancion;
	
	mostrar=showCollection(collection);
	
	if (mostrar==true){
		cout<<SELSONG;
		cin>>buscarid;
		posicion=findIdSong(collection, buscarid);
		
		if (posicion==-1){
			cout<<ERRNOSONG<<buscarid;
		} else{
			do{
				cout<<EDIT;
				cin>>opcion;
				if (opcion>5 || opcion<1){
					cout<<ERRNOOPTION;
				}
			}while(opcion>5 || opcion<1);
			
			limpiarBuffer();
			
			switch(opcion){
				case 1:
					cout<<TITLE;
					getline(cin,cancion.title);
					collection.songs[posicion].title=cancion.title;
					break;
				case 2:
					cout<<ARTIST;
					getline(cin,cancion.artist);
					collection.songs[posicion].artist=cancion.artist;
					break;
				case 3:
					cout<<ALBUM;
					getline(cin,cancion.album);
					collection.songs[posicion].album=cancion.album;
					break;
				case 4:
					cout<<GENRE;
					getline(cin,cancion.genre);
					collection.songs[posicion].genre=cancion.genre;
					break;
				case 5: 
					cout<<URL;
					getline(cin,cancion.url);
					collection.songs[posicion].url=cancion.url;
					break;
			}
		}
	}
}

void deleteSong(Collection &collection){
	bool mostrar;
	int id;
	int buscarid;
	char confirmar;
	
	mostrar=showCollection(collection);
	
	if (mostrar==true){
		cout<<SELSONG;
		cin>>id;
		buscarid=findIdSong(collection,id);
		if(buscarid!=-1){
			cout<<DELETE;
			printSong(collection.songs[buscarid]);
			cout<<CONFIRM;
			cin>>confirmar;
			if (confirmar=='Y'){
				collection.songs.erase(collection.songs.begin()+buscarid);
			} else{
				cout<<NOTDELETED;
			}
		}
		else {
			cout<<ERRNOSONG<<id;
		}
	}
}

void managePlaylist(Playlist &playlist, const Collection &collection){
	char opcion;
	
	do{
		cout<<"1- Add songs";
		cout<<"\n2- Remove songs";
		cout<<"\n3- Clear";
		cout<<"\nq- Back to main menu";
		cout<<"\nOption: ";
		
		cin>>opcion;
		
		switch (opcion){
			case '1':
				addSongsPlaylist(playlist, collection);
				break;
			case '2':
				removeSongsPlaylist(playlist, collection);
				break;
			case '3':
				clearSongsPlaylist(playlist);
				break;
			case 'q':
				break;
			default:
				cout<<ERRNOOPTION;
				break;
		}
	}while (opcion !='q');
}

void addSongsPlaylist(Playlist &playlist, const Collection &collection){
	unsigned int i,j;
	vector<int> idCanciones;
	int contador;
	  
	if (collection.songs.size()==0){
		cout<<EMPTYCOL<<endl;
	} else{
		showCollection(collection);
		cout<<SELSONGSEP;
		idCanciones=separa();
		
		for(i=0;i<idCanciones.size();i++){
			contador=0;
			for(j=0;j<collection.songs.size();j++){
				if (idCanciones[i]==collection.songs[j].id){
					playlist.idSong.push_back(idCanciones[i]);
					contador++;
				}
			}
			if (contador==0){
				cout<<ERRNOSONG<<idCanciones[i]<<endl;
			}
		}
	}
}

vector<int> separa(){   //Este modulo se encarga de leer un string y pasarlo a un vector de enteros
	string leer;
	string num;
	unsigned int i;
	int numfinal;
	vector<int> idCanciones;
	
	getline(cin,leer);
	num = "";
	
	for(i=0; i<leer.size(); i++){
		if(leer[i] == ' '){
			numfinal=atoi(num.c_str());      //el string num pasarlo a entero y almacenarlo en numfinal
			idCanciones.push_back(numfinal);
			num = "";
		} else{
			num += leer[i];
		}
	}
	
	if(num!=""){      //Caso del ultimo numero
		numfinal=atoi(num.c_str());
		idCanciones.push_back(numfinal);
	}
	return (idCanciones);
}

void printPlaylist(const Playlist &playlist, const Collection &collection, int position){
	unsigned int i;
	int posicionid;
	
	for(i=0; i<playlist.idSong.size(); i++){   
		posicionid=findIdSong(collection,playlist.idSong[i]);
		
		if(i== (unsigned) position){   //El (unsigned) es para que no se queje el -Wall
			cout<<">";
			printPlaylistSong(collection.songs[posicionid]);
		} else{
			printPlaylistSong(collection.songs[posicionid]);
		} 
	}
}

void printPlaylistSong(Song cancion){
	cout<<" "<<cancion.id<<": "<<cancion.title << " (" <<cancion.artist << ")"<<endl;
}

void removeSongsPlaylist(Playlist &playlist, const Collection &collection){
	unsigned int i;
	int j;
	vector<int> listaremove;
	int contador;
	int position=-1;
	int tamPlaylist;
	
	if (playlist.idSong.size()==0){
		cout<<EMPTYPLAY<<endl;
	} else{
		printPlaylist(playlist,collection,position);
		cout<<SELSONGSEP;
		limpiarBuffer();
		listaremove=separa();
		tamPlaylist = playlist.idSong.size();
		
		for(i=0;i<listaremove.size();i++){
			contador=0;
			for (j=tamPlaylist-1; j>=0;j--){
				if(listaremove[i]==playlist.idSong[j]){
					playlist.idSong.erase(playlist.idSong.begin()+j);
					contador++;
				}
			}
			if (contador==0){
				cout<<ERRSONG<<listaremove[i]<<NOTPLAY<<endl;
			}
		}
	}
}

void clearSongsPlaylist(Playlist &playlist){
	char op;
	
	if (playlist.idSong.size()==0){
		cout<<EMPTYPLAY;
	} else{
		cout<<CLEARPLAY;
		cin>>op;
		if (op=='Y'){
			playlist.idSong.clear();
		}
	}
}

bool importJSON(Collection &collection, string filename){
	Song cancion;
	ifstream fichero;
	string JSONline;
	bool importado=false;
	
	if (filename.size()==0){
		cout<<FILEN;
		cin>>filename;
	}
	
	fichero.open(filename.c_str(),ios::in);
	if (fichero.is_open()){
		do{
			JSONline="";
			getline(fichero,JSONline);
			
			if (JSONline.find(FINDKINDSONG) != string::npos){   //Si la linea obtenida tiene "kind:song" se puede extraer la cancion
				cancion=getSongFromJSONline(JSONline);
				addSongToCol(cancion, collection);   //Se añade a la coleccion
				importado=true;
			}
		}while(!fichero.eof());   //Mientras no se llegue al final de fichero, se siguen leyendo las lineas
		 
		fichero.close();
	} else{
		cout<<FICHERR<<filename<<endl;
	}
	
	return (importado);  //Devuelve si se ha importado alguna cancion o no
}

Song getSongFromJSONline(string JSONline){
	Song cancion;
	unsigned int pos;
	string titlename;
	string collectname;
	string artisname;
	string genername;
	string url;
	
	pos=JSONline.find(TRACKNAME);           //Almacena en pos la posicion donde ha encontrado 'trackname:' (almacenaría la posicion de la 't' en el string)
	if(pos != string::npos){                //Si se ha encontrado trackname: (title), entonces
		pos = pos+TRACKNAME.length();   //Se suma el tamaño de 'trackname:', guardando en pos la posicion de ':'
		
		titlename= getNameFromLine(pos, JSONline);  //Se le pasa por referencia la linea y la posicion de la palabra que deseamos extraer
		cancion.title=titlename;
	}
	
	pos=JSONline.find(COLLENAME);
	if(pos != string::npos){                           //si se ha encontrado collename: (album)
		pos = pos+COLLENAME.length();
		collectname= getNameFromLine(pos, JSONline);
		cancion.album=collectname;
	}
	
	pos=JSONline.find(ARTISTNAME);
	if(pos != string::npos){                           //si se ha encontrado artisname: (artist)
		pos = pos+ARTISTNAME.length();
		artisname= getNameFromLine(pos, JSONline);
		cancion.artist=artisname;
	}
	
	pos=JSONline.find(GENRENAME);
	if(pos != string::npos){                           //si se ha encontrado genrename: (genre)
		pos = pos+GENRENAME.length();
		genername= getNameFromLine(pos, JSONline);
		cancion.genre=genername;
	}
	
	pos=JSONline.find(URLNAME);
	if(pos != string::npos){                          //si se ha encontrado urlname: (url)
		pos = pos+URLNAME.length();
		url= getNameFromLine(pos, JSONline);
		cancion.url=url;
	}
	return (cancion);
}

string getNameFromLine(unsigned int &pos, string JSONline){   //Se le pasa una linea de tipo string, y extrae desde la posicion 'pos' hasta las proximas comillas
	unsigned int i;
	char carac;
	string name="";
	bool encontrada=false;
	
	for(i=pos;i<JSONline.length() && encontrada==false;i++){  //Recorre un string y almacena los caract. hasta encontrar unas comillas
		
		carac=JSONline[i];

		if (carac=='"')
			encontrada=true;
		else{
			name=name+carac;
		}
	}
	return (name);
}

bool addSongToCol(Song cancion, Collection &collection){  //Recibe una cancion, y la añade a la coleccion si no esta repetida 
	bool repetida;
	bool anyadir;
	
	repetida=isSongInCollection(collection, cancion);
	
	if (repetida==false){
		cancion.id=collection.idNextSong;
		collection.songs.push_back(cancion);
		collection.idNextSong++;
		anyadir=true;
	}
	else{
		cout<<THESONG<<cancion.title <<ISINCOLLECTION;
		anyadir=false;
	}
	
	return (anyadir);
}

bool saveXSPFPlaylist(const Playlist &playlist, const Collection &collection){
	ofstream fichero;
	bool guardado=true;
	unsigned int i;
	Song cancion;
	unsigned int posicionid;
	string filename;

	limpiarBuffer();
	
	if (playlist.idSong.size()>0){
		cout<<FILEN;
		getline(cin,filename);
		
		fichero.open(filename.c_str(),ios::out);
		if (fichero.is_open()){
			fichero << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl
			     << "<playlist version=\"1\" xmlns=\"http://xspf.org/ns/0/\">" << endl
			     << "  <title>MiniTunes playlist</title>" << endl
			     << "  <trackList>" << endl;
			     
			for(i=0;i<playlist.idSong.size();i++){
				posicionid=findIdSong(collection,playlist.idSong[i]); //Encuentra en la collection la cancion del playlist
				printXSPFSong(collection.songs[posicionid],fichero);  //Escribir en el fichero esa cancion
			}
			fichero << "  </trackList>" << endl << "</playlist>" << endl;
			fichero.close();
		} else{
			cout<<FICHERR<<filename.c_str()<<endl;
			guardado=false;
		}
	} else{
		guardado=false;
		cout<<EMPTYPLAY;
	}
	return (guardado);
}

void printXSPFSong(Song cancion, ofstream &fichero){
	fichero << "    <track>" << endl
	     << "    <title>" << cancion.title << "</title>" << endl
	     << "    <creator>" << cancion.artist << "</creator>" << endl
	     << "    <album>" << cancion.album << "</album>" << endl
	     << "    <location>" << cancion.url << "</location>" <<endl
	     << "    <meta rel=\"genre\">" << cancion.genre << "</meta>" << endl
	     << "    </track>" << endl;
}

bool saveData(const Collection &collection, const Playlist &playlist){
	ofstream fichero;
	fichero.open(FILEBINARY, ios::out | ios::binary);
	
	if (collection.songs.size()>0){    //Si la coleccion esta vacia no se guarda nada
		if (fichero.is_open()){
			writePlaylist(playlist, fichero);
			writeCollection(collection, fichero);
			fichero.close();
		} else{
			cout<<FICHERR<<FILEBINARY<<endl;
		}
	} else{
		cout<<EMPTYCOL<<endl;
	}
	return (true);
}

bool writePlaylist(const Playlist &playlist, ofstream &fichero){
	bool notwrite=false;
	int tamanyo;
	int i;
	
	if (fichero.is_open()){
		tamanyo=playlist.idSong.size();
		fichero.write((char *) &tamanyo, sizeof(int));
		for(i=0;i<tamanyo;i++){
			fichero.write((char *) (&playlist.idSong[i]),sizeof(int));  //Se van guardando todas las canciones de la coleccion
		}
	} else{
		notwrite=true;
	}
	return (notwrite);
}

bool writeCollection(const Collection &collection, ofstream &fichero){
	bool notwrite=false;
	int idnextsong;
	unsigned int i;
	SongBin cancionbin;
	
	if (fichero.is_open()){
		idnextsong=collection.idNextSong;
		fichero.write((char *) (&idnextsong), sizeof(int));  //Se guarda el idNextSong
		
		for (i=0;i<collection.songs.size();i++){
			cancionbin=songToBinary(collection.songs[i]);  //Se convierte del tipo song a songbin para poder almacenar en el binario correctamente
			fichero.write((char *) &cancionbin, sizeof(SongBin));
		}
	} else{
		notwrite=true;
	}
	return (notwrite);
}

SongBin songToBinary(const Song &cancion){
	SongBin nueva;
	nueva.id=cancion.id;
	//strcpy(nueva.title,cancion.title.c_str());
	//nueva.title[kTITLE-1] = '\0';
	
	if (cancion.title.size()< (unsigned) kTITLE){
		strcpy(nueva.title,cancion.title.c_str());    //Convierte el string (cancion.title) al tipo char[] , y lo almacena en nueva.title
	} else{
		string newTitle = cancion.title.substr (0,kTITLE-1);  //Acorta en newTitle desde la posicion 0 del string hasta el maximo
		strcpy(nueva.title,newTitle.c_str());
	}
	
	if (cancion.artist.size()< (unsigned) kARTIST){
		strcpy(nueva.artist,cancion.artist.c_str());    
	} else{
		string newArtist = cancion.artist.substr (0,kARTIST-1);
		strcpy(nueva.artist,newArtist.c_str());
	}
	
	if (cancion.album.size()< (unsigned) kALBUM){
		strcpy(nueva.album,cancion.album.c_str());
	} else{
		string newAlbum = cancion.album.substr (0,kALBUM-1);
		strcpy(nueva.album,newAlbum.c_str());
	}
	
	if (cancion.genre.size()< (unsigned) kGENRE){
		strcpy(nueva.genre,cancion.genre.c_str()); 
	} else{
		string newGenre = cancion.genre.substr (0,kGENRE-1);
		strcpy(nueva.genre,newGenre.c_str());
	}
	
	if (cancion.url.size()< (unsigned) kURL){
		strcpy(nueva.url,cancion.url.c_str());
	} else{
		string newUrl = cancion.url.substr (0,kURL-1);
		strcpy(nueva.url,newUrl.c_str());
	}
	
	return(nueva);
}

bool loadData(Collection &collection, Playlist &playlist){
	ifstream fichero;
	char confirm;
	fichero.open(FILEBINARY, ios::in | ios::binary);
	
	if (fichero.is_open()){
		if (playlist.idSong.size()>0){
			playlist.idSong.clear();
		}
		readPlaylist(playlist, fichero);
		
		if (collection.songs.size()>0){   //Si la coleccion tiene algun valor se pregunta el borrado, si no tiene ningun valor se lee directamente
			cout<<CONFIRMDELCOL;
			cin>>confirm;
			if (confirm=='Y'){
				deleteCollection(collection);
				readCollection(collection, fichero);
			}
		}
		else{
			readCollection(collection, fichero);
		}
		fichero.close();
	} else{
		cout<<FICHERR<<FILEBINARY<<endl;
	}
	return (true);
}

void deleteCollection(Collection &collection){  //Se encarga de vaciar toda la coleccion e inicializar el idNextSong
	collection.songs.clear();
	collection.idNextSong=1;
}

bool readPlaylist(Playlist &playlist, ifstream &fichero){
	bool notread=false;
	int tamanyo;
	int i;
	int songid;
	
	if (fichero.is_open()){
		fichero.read((char *) &tamanyo, sizeof(int));  //Lee el tamaño de la playlist

		for(i=0;i<tamanyo;i++){
			fichero.read((char *) (&songid),sizeof(int));  //Lee las canciones del playlist
			playlist.idSong.push_back(songid);             //Añade esas canciones
		}
	} else{
		notread=true;
	}
	return (notread);
}

bool readCollection(Collection &collection, ifstream &fichero){
	bool notread=false;
	SongBin cancionbin;
	int idnextsong;
	Song cancion;
	int i=0;
	
	if (fichero.is_open()){
		fichero.read((char *) (&idnextsong), sizeof(int));  //Lee el idNextSong
		collection.idNextSong=idnextsong;                   //Se lo asigna a la coleccion
		
		while (!fichero.eof() && i<idnextsong-1){                      //Mientras no llegue al final de fichero, y no se pase del maximo de canciones
			fichero.read((char *) &cancionbin, sizeof(SongBin));    //Va leyendo todas las canciones
			cancion=binaryToSong(cancionbin);                       //Las convierte de songbin a song
			
			if (isSongInCollection(collection, cancion)==false){   //Si ya ha sido añadida a la coleccion, no la vuelve a añadir
				collection.songs.push_back(cancion);
			}
			i++;
		}
	} else{
		notread=true;
	}
	return (notread);
}

Song binaryToSong(const SongBin &cancionbin){
	Song cancion;
	
	cancion.id=cancionbin.id;
	cancion.title=string (cancionbin.title);   //Convierte de char[] a string
	cancion.artist=string (cancionbin.artist);
	cancion.album=string (cancionbin.album);
	cancion.genre=string (cancionbin.genre);
	cancion.url=string (cancionbin.url);
	
	return(cancion);
}

void playPlaylist(const Playlist &playlist, const Collection &collection){
	Player player;
	unsigned int position;
	char opcion=' ';
	int tammaxplay;
	int i=0;
	bool defecto=true;
	
	if (playlist.idSong.size()>0){
		tammaxplay=playlist.idSong.size();  //almaceno el tamaño de la playlist, para controlar la reproduccion circular
		
		while(opcion!='q'){
			if (opcion=='s' || opcion=='a' || defecto){  //El bool defecto sirve para que comience la reproduccion la primera vez que se llame a la funcion
				position=i;                          //Posiscion de la cancion que se tiene que reproducir
				printPlaylist(playlist, collection, position);   //Se le manda la posicion de la cancion que se esta reproduciendo
				position=playlist.idSong[position];      //Se reutiliza la variable position para almacenar el id de la cancion
				playSong(collection.songs[position-1].url, player);  //Comienza la reproduccion
				defecto=false;
			}
			cout<<COMMAND;
			cin>>opcion;
			
			switch (opcion){
				case 'p':
					pauseResumeSong(player);  //Pausar/reanudar
					break;
				case 's':
					i++;                 //Se adelanta una cancion
					if (i>tammaxplay-1)  //Si hemos llegado a la ultima, se vuelve a la primera
						i=0;
					break;
				case 'a':
					i--;           //Se retrasa una cancion
					if (i<0)       //Si hemos llegado a la primera, se vuelve a la ultima
						i=tammaxplay-1;
					break;
				case 'q':
					endPlayer(player);  //Fin de reproduccion
					break;
				default:
					cout<<"Options:\n"
					    <<"p: Pause/resume\n"
					    <<"s: Next song\n"
					    <<"a: Previous song\n"
					    <<"q: Quit\n";
					break;
			}  
		}
	} else{
		cout<<EMPTYPLAY;
	}
}

bool manageArguments(int argc, char *argv[], Collection &collection, Playlist &playlist){
	bool managed=false;
	
	if (argc<4  && argc>1){                             //Minimo y maximo de argumentos correctos
		if (strcmp(argv[1],"-d")==0  &&  argc==2){  //Si el segundo argumento es -d, y no hay mas argumentos ( ./minitunes -d )
			loadData(collection, playlist);     //Se cargan los datos de minitunes.dat (por defecto)
			managed=true;
		} else if (strcmp(argv[1],"-i")==0   &&   argc==3  &&  strcmp(argv[2],"-d")!=0) { //Si el segundo argumento es -i, hay tres argumentos, y el ultimo no es -d
			importJSON(collection, argv[2]);     //Se importa desde el fichero en formato JSON, pasando el tercer argumento como nombre de fichero
			managed=true;
		} else{
			cout<<ERRSYNTAX;
		}
	} else if (argc!=1){
		cout<<ERRSYNTAX;
	}
	
	if (argc==1)  //Que solo tenga un argumento tambien significa que es correcto, puesto que el primero es el nombre del programa
		managed=true;
	
	return (managed);  //Devuelve true si los argumentos han sido correctos
}
