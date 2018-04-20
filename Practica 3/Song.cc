#include "Song.h"


Song::Song(int id, string title, string artist, string album, string genre, string url){
	this->id = id;
	this->title = title;
	this->artist = artist;
	this->album = album;
	this->genre = genre;
	this->url = url;
}

int Song::getId() const{
	return id;
}

string Song::getTitle() const{
	return title;
}

string Song::getArtist() const{
	return artist;
}

string Song::getAlbum() const{
	return album;
}

string Song::getGenre() const{
	return genre;
}

string Song::getUrl() const{
	return url;
}

void Song::setId(int id){
	this->id = id;
}

void Song::setTitle(string title){
	this->title = title;
}

void Song::setArtist(string artist){
	this->artist = artist;
}

void Song::setAlbum(string album){
	this->album = album;
}

void Song::setGenre(string genre){
	this->genre = genre;
}

void Song::setUrl(string url){
	this->url = url;
}


Song::Song(string JSONline){  //getSongFromJSONline
	unsigned int pos;
	string titlestring;
	string collectstring;
	string artisstring;
	string generstring;
	string urlstring;
	
	pos=JSONline.find(TRACKNAME);           //Almacena en pos la posicion donde ha encontrado 'trackname:' (almacenaría la posicion de la 't' en el string)
	if(pos != string::npos){                //Si se ha encontrado trackname: (title), entonces
		pos = pos+TRACKNAME.length();   //Se suma el tamaño de 'trackname:', guardando en pos la posicion de ':'
		
		titlestring= getNameFromLine(pos, JSONline);  //Se le pasa por referencia la linea y la posicion de la palabra que deseamos extraer
		title=titlestring;
	}
	
	pos=JSONline.find(COLLENAME);
	if(pos != string::npos){                           //si se ha encontrado collename: (album)
		pos = pos+COLLENAME.length();
		collectstring= getNameFromLine(pos, JSONline);
		album=collectstring;
	}
	
	pos=JSONline.find(ARTISTNAME);
	if(pos != string::npos){                           //si se ha encontrado artisname: (artist)
		pos = pos+ARTISTNAME.length();
		artisstring= getNameFromLine(pos, JSONline);
		artist=artisstring;
	}
	
	pos=JSONline.find(GENRENAME);
	if(pos != string::npos){                           //si se ha encontrado genrename: (genre)
		pos = pos+GENRENAME.length();
		generstring= getNameFromLine(pos, JSONline);
		genre=generstring;
	}
	
	pos=JSONline.find(URLNAME);
	if(pos != string::npos){                          //si se ha encontrado urlname: (url)
		pos = pos+URLNAME.length();
		urlstring= getNameFromLine(pos, JSONline);
		url=urlstring;
	}
}

string Song::getNameFromLine(unsigned int &pos, string JSONline) const{   //Se le pasa una linea de tipo string, y extrae desde la posicion 'pos' hasta las proximas comillas
	unsigned int i;
	char carac;
	string name="";
	bool encontrada=false;
	
	for(i=pos;i<JSONline.length() && encontrada==false;i++){  //Recorre un string y almacena los caract. hasta encontrar unas comillas
		carac=JSONline[i];
		
		if (carac=='"'){
			encontrada=true;
		} else{
			name=name+carac;
		}
	}
	return (name);
}

Song::Song(const SongBin &cancionbin){ //binaryToSong
	id=cancionbin.id;
	
	title=string (cancionbin.title);   //Convierte de char[] a string
	artist=string (cancionbin.artist);
	album=string (cancionbin.album);
	genre=string (cancionbin.genre);
	url=string (cancionbin.url);
}

void Song::printPlaylist() const{ //printPlaylistSong
	cout<<" "<<id<<": "<<title << " (" <<artist << ")"<<endl;
}

void Song::printXSPF(ofstream &fichero) const{ //printXSPFSong
	fichero << "    <track>" << endl
	     << "    <title>" << title << "</title>" << endl
	     << "    <creator>" << artist << "</creator>" << endl
	     << "    <album>" << album << "</album>" << endl
	     << "    <location>" << url << "</location>" <<endl
	     << "    <meta rel=\"genre\">" << genre << "</meta>" << endl
	     << "    </track>" << endl;
}

SongBin Song::toBinary() const{ //songToBinary
	SongBin nueva;
	
	nueva.id=id;
	
	//strcpy(nueva.title,cancion.title.c_str()); //Estas dos lineas son la otra forma de hacerlo
	//nueva.title[kTITLE-1] = '\0';
	
	if (title.size()< (unsigned) kTITLE){
		strcpy(nueva.title,title.c_str());    //Convierte el string (cancion.title) al tipo char[] , y lo almacena en nueva.title
	} else{
		string newTitle = title.substr (0,kTITLE-1);  //Acorta en newTitle desde la posicion 0 del string hasta el maximo
		strcpy(nueva.title,newTitle.c_str());
	}
	
	if (artist.size()< (unsigned) kARTIST){
		strcpy(nueva.artist,artist.c_str());    
	} else{
		string newArtist = artist.substr (0,kARTIST-1);
		strcpy(nueva.artist,newArtist.c_str());
	}
	
	if (album.size()< (unsigned) kALBUM){
		strcpy(nueva.album,album.c_str());
	} else{
		string newAlbum = album.substr (0,kALBUM-1);
		strcpy(nueva.album,newAlbum.c_str());
	}
	
	if (genre.size()< (unsigned) kGENRE){
		strcpy(nueva.genre,genre.c_str()); 
	} else{
		string newGenre = genre.substr (0,kGENRE-1);
		strcpy(nueva.genre,newGenre.c_str());
	}
	
	if (url.size()< (unsigned) kURL){
		strcpy(nueva.url,url.c_str());
	} else{
		string newUrl = url.substr (0,kURL-1);
		strcpy(nueva.url,newUrl.c_str());
	}
	return(nueva);
}

bool Song::isEqual(const Song &song) const{
	bool igual=false;
	
	if(title==song.title && artist==song.artist){
		igual=true;
	}
	return (igual);
}

istream& operator>>(istream &is, Song &f){ //demandSong
	cout<<TITLE; 
	getline(is,f.title);
	
	cout<<ARTIST;
	getline(is,f.artist);
	
	cout<<ALBUM;
	getline(is,f.album);
	
	cout<<GENRE;
	getline(is,f.genre);
	
	cout<<URL;
	getline(is,f.url);
	
	return (is);
}


ostream& operator<<(ostream &os, const Song &f){ //printSong
	os<<f.id<<" | "<<f.title<<" | "<<f.artist<<" | "<<f.album <<" | "<<f.genre;
	return os;
}










