// Guerrero Selva, Margarita
//Rider Garcia, Lawrence Arthur

#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

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


// Función para inicializar los valores de la colección
Collection createCollection(){
	Collection collection;
	collection.idNextSong=1;

	return collection;
}

// Procedimiento para limpiar el buffer de entrada
void limpiarBuffer(){
	while(cin.get() !='\n');
} 

// Declaracion de funciones
Song demandSong();
bool isSongInCollection(const Collection &collection, Song cancion);
bool addSong(Collection &collection);
bool showCollection(const Collection &collection);
void printSong(Song cancion);
int findIdSong(const Collection &collection, int id);
void editSong(Collection &collection);
void deleteSong(Collection &collection);


int main(){
	char opcion;
	
	Collection collection=createCollection();
	Song cancion;
	
	do{	
		cout<<"---------------------------------------------\n--------------- MiniTunes -------------------";
		cout<<"\n---------------------------------------------\n";
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
				// a partir de aqui practica 2
				break;	
			case '6':
				
				break;	
			case '7':
				
				break;	
			case '8':
				
				break;	
			case '9':
				
				break;
			case '0':
				
				break;
			case 'q':
				break;
			default:
				cout<<"\n\nError: Unknown option\n\n";
				break;
		}
	}while (opcion!='q');
}

Song demandSong(){ 
	Song cancion;
	
	limpiarBuffer();
	
	cout<<"Title: ";
	getline(cin,cancion.title);
	
	cout<<"Artist: ";
	getline(cin,cancion.artist);
	
	cout<<"Album: ";
	getline(cin,cancion.album);
	
	cout<<"Genre: ";
	getline(cin,cancion.genre);
	
	cout<<"Url: ";
	getline(cin,cancion.url);

	return cancion;
}

bool isSongInCollection(const Collection &collection, Song cancion){
	unsigned int i;
	bool encontrada=false;
	
	for(i=0;i<collection.songs.size() && encontrada==false;i++){
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
	
	if (repetida==false){
		cancion.id=collection.idNextSong;
		collection.songs.push_back(cancion);
		collection.idNextSong++;
		anyadir=true;
	} else{
		cout<<"The song "<<cancion.title <<" is already in the collection\n";
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
	
	cout<<"Search: ";
	getline(cin,buscar);
		
	for(i=0;i<collection.songs.size();i++){
		if(collection.songs[i].title.find(buscar) != string::npos){
			contador++;
			printSong(collection.songs[i]);
			cout<<endl;
		} else if(collection.songs[i].artist.find(buscar) != string::npos){
			contador++;
			printSong(collection.songs[i]);
			cout<<endl;
		} else if(collection.songs[i].album.find(buscar) != string::npos){
			contador++;
			printSong(collection.songs[i]);
			cout<<endl;
		} else if(collection.songs[i].genre.find(buscar) != string::npos){
			contador++;
			printSong(collection.songs[i]);
			cout<<endl;
		}
	}
	
	if (contador==0){
		cout<<"No results\n";
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
	int j;
	
	mostrar=showCollection(collection);
	
	if (mostrar==true){
		cout<<"Select song: ";
		cin>>buscarid;
		posicion=findIdSong(collection, buscarid);
		
		if (posicion==-1){
			cout<<"Error: Unknown song "<<buscarid;
		} else{
			do{
				cout<<"Edit (1-Title, 2-Artist, 3-Album, 4-Genre, 5-Url): ";
				cin>>opcion;
				if (opcion>5 || opcion<1)
					cout<<"Error: Unknown option";
			}while(opcion>5 || opcion<1);
			
			limpiarBuffer();
			
			j=buscarid-1;
			
			switch(opcion){
				case 1:
					cout<<"Title: ";
					getline(cin,cancion.title);
					collection.songs[j].title=cancion.title;
					break;
				case 2:
					cout<<"Artist: ";
					getline(cin,cancion.artist);
					collection.songs[j].artist=cancion.artist;
					break;
				case 3:
					cout<<"Album: ";
					getline(cin,cancion.album);
					collection.songs[j].album=cancion.album;
					break;
				case 4:
					cout<<"Genre: ";
					getline(cin,cancion.genre);
					collection.songs[j].genre=cancion.genre;
					break;
				case 5: 
					cout<<"Url: ";
					getline(cin,cancion.url);
					collection.songs[j].url=cancion.url;
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
	int j;
	
	mostrar=showCollection(collection);
	
	if (mostrar==true){
		cout<<"Select song: ";
		cin>>id;
		buscarid=findIdSong(collection,id);
		if(buscarid!=-1){
			cout<<"Delete ";
			j=id-1;
			printSong(collection.songs[j]);
			cout<<"? (Y/N): ";
			cin>>confirmar;
			if (confirmar=='Y'){
				collection.songs.erase(collection.songs.begin()+j);
			} else{
				cout<<"Song not deleted\n";
			}
		} else{
			cout<<"Error: Unknown song "<<id;
		}
	}
}
