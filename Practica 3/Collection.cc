#include "Collection.h"
#include "Utils.h"
#include "Constants.h"

Collection::Collection(){ //createCollection
	idNextSong=1;
}


int Collection::findIdSong(int id) const{
	unsigned int i;
	int posicion=-1;
	bool salir=false;
	
	for(i=0;i<songs.size() && salir==false;i++){
		if (id==songs[i].getId()){
			posicion=i;
			salir=true;
		}
	}
	
	return(posicion);
}

bool Collection::isSongInCollection(Song cancion) const{
	unsigned int i;
	bool encontrada=false;
	
	for(i=0;i<songs.size() && encontrada==false;i++){   //Se encarga de comparar una cancion en el vector canciones y devolver true si se encuentra
		if (songs[i].getTitle()==cancion.getTitle()){
			if (songs[i].getArtist()==cancion.getArtist()){
				encontrada=true;
			}
		}
	}
	return(encontrada);
}

bool Collection::show() const{
	string buscar;
	unsigned int i;
	bool encontrada;
	int contador=0;
	
	cout<<SEARCH;
	getline(cin,buscar);
	
	for(i=0;i<songs.size();i++){
		if(songs[i].getTitle().find(buscar) != string::npos || songs[i].getArtist().find(buscar) != string::npos || 
		   songs[i].getAlbum().find(buscar) != string::npos || songs[i].getGenre().find(buscar) != string::npos){
			contador++;
			cout<<songs[i];
			cout<<endl;
		}
	}
	
	if (contador==0){
		cout<<ERRNORESULTS;
		encontrada=false;
	}
	else{
		encontrada=true;
	}
	
	return (encontrada);
}

bool Collection::addSong(){
	Song cancion;
	bool repetida;
	bool anyadir;
	
	cin>>cancion;
	
	repetida=isSongInCollection(cancion);
	
	
	if (repetida==false){      //Si hay una cancion repetida no se añade a la coleccion, en caso contrario si se añade
		cancion.setId(idNextSong);
		songs.push_back(cancion);
		idNextSong++;
		anyadir=true;
	}
	else{
		cout<<THESONG<<cancion.getTitle() <<ISINCOLLECTION;
		anyadir=false;
	}
	
	return (anyadir);
}

void Collection::editSong(){
	bool mostrar;
	int buscarid;
	int posicion;
	int opcion;
	string editar;
	
	mostrar=show();
	
	if (mostrar==true){
		cout<<SELSONG;
		cin>>buscarid;
		Utils::limpiarBuffer();
		posicion=findIdSong(buscarid);
		
		if (posicion==-1){
			cout<<ERRNOSONG<<buscarid;
		}
		else{
			do{
				cout<<EDIT;
				cin>>opcion;
				Utils::limpiarBuffer();
				if (opcion>5 || opcion<1)
					cout<<ERRNOOPTION;
			}while(opcion>5 || opcion<1);
			
			switch(opcion){
				case 1:
					cout<<TITLE;
					getline(cin,editar);
					songs[posicion].setTitle(editar);
					break;
				case 2:
					cout<<ARTIST;
					getline(cin,editar);
					songs[posicion].setArtist(editar);
					break;
				case 3:
					cout<<ALBUM;
					getline(cin,editar);
					songs[posicion].setAlbum(editar);
					break;
				case 4:
					cout<<GENRE;
					getline(cin,editar);
					songs[posicion].setGenre(editar);
					break;
				case 5: 
					cout<<URL;
					getline(cin,editar);
					songs[posicion].setUrl(editar);
					break;
			}
		}
	}
}

void Collection::deleteSong(){
	bool mostrar;
	int id;
	int buscarid;
	char confirmar;
	
	mostrar=show();
	
	if (mostrar==true){
		cout<<SELSONG;
		cin>>id;
		Utils::limpiarBuffer();
		buscarid=findIdSong(id);
		if(buscarid!=-1){
			cout<<DELETE;
			cout<<songs[buscarid];
			cout<<CONFIRM;
			cin>>confirmar;
			Utils::limpiarBuffer();
			if (confirmar=='Y'){
				songs.erase(songs.begin()+buscarid);
			} else{
				cout<<NOTDELETED;
			}
		}
		else {
			cout<<ERRNOSONG<<id;
		}
	}
	
}

bool Collection::importJSON(string filename){
	ifstream fichero;
	string JSONline;
	bool importado=false;
	
	if (filename.size()==0){
		cout<<FILEN;
		getline(cin,filename);
	}
	
	fichero.open(filename.c_str(),ios::in);
	if (fichero.is_open()){
		do{
			JSONline="";
			getline(fichero,JSONline);
			
			if (JSONline.find(FINDKINDSONG) != string::npos){   //Si la linea obtenida tiene "kind:song" se puede extraer la cancion
				Song cancion(JSONline);  //Se crea una cancion con los valores extraidos de la JSONline
				addSongToCol(cancion);   //Se añade a la coleccion
				importado=true;
			}
		}while(!fichero.eof());   //Mientras no se llegue al final de fichero, se siguen leyendo las lineas
		 
		fichero.close();
	}
	else {
		cout<<FICHERR<<filename<<endl;
	}
	return (importado);  //Devuelve si se ha importado alguna cancion o no
}

bool Collection::addSongToCol(Song cancion){  //Recibe una cancion, y la añade a la coleccion si no esta repetida 
	bool repetida;
	bool anyadir;
	
	repetida=isSongInCollection(cancion);
	
	if (repetida==false){
		cancion.setId(idNextSong);
		songs.push_back(cancion);
		idNextSong++;
		anyadir=true;
	} else{
		cout<<THESONG<<cancion.getTitle() <<ISINCOLLECTION;
		anyadir=false;
	}
	
	return (anyadir);
}

bool Collection::read(ifstream &fichero){ 
	bool notread=true;
	SongBin cancionbin;
	int saveidnextsong;
	int i=0;
	
	if (fichero.is_open()){
		fichero.read((char *) (&saveidnextsong), sizeof(int));  //Lee el idNextSong
		idNextSong=saveidnextsong;                   //Se lo asigna a la coleccion
		
		while (!fichero.eof() && i<saveidnextsong-1){                      //Mientras no llegue al final de fichero, y no se pase del maximo de canciones
			fichero.read((char *) &cancionbin, sizeof(SongBin));    //Va leyendo todas las canciones               
			Song cancion(cancionbin);                              //Las convierte de songbin a song
			
			if (isSongInCollection(cancion)==false)   //Si ya ha sido añadida a la coleccion, no la vuelve a añadir
				songs.push_back(cancion);
			i++;
		}
	}
	else {
		notread=false;
	}
	return (notread);
}

bool Collection::write(ofstream &fichero) const{
	bool notwrite=true;
	int writeidnextsong;
	unsigned int i;
	SongBin cancionbin;
	
	if (fichero.is_open()){
		writeidnextsong=idNextSong;
		fichero.write((char *) (&writeidnextsong), sizeof(int));  //Se guarda el idNextSong
		
		for (i=0;i<songs.size();i++){
			cancionbin=songs[i].toBinary(); //Se convierte del tipo song a songbin para poder almacenar en el binario correctamente
			
			fichero.write((char *) &cancionbin, sizeof(SongBin));
		}
	}
	else{
		notwrite=false;
	}
	return (notwrite);
}


Song Collection::getSongAt(int index) const{
	Song cancion;
	
	cancion=songs[index];
	
	return(cancion);
}


int Collection::size() const{
	int tamanyo;
	
	tamanyo=songs.size();
	
	return(tamanyo);
}


void Collection::clear(){
	songs.clear();
	idNextSong=1;
}








