#include "Playlist.h"
#include "Utils.h"

Playlist::Playlist(Collection *collection) {
	pcollection=collection;
}

int Playlist::size() const{
	int tamanyo;
	tamanyo=idSong.size();
	
	return (tamanyo);
}

void Playlist::print(int position) const{ //printPlaylist
	unsigned int i;
	int posicionid;
	
	for(i=0; i<idSong.size(); i++){   
		posicionid= pcollection->findIdSong(idSong[i]);
		
		if(i== (unsigned) position){
			cout<<">";
			pcollection->getSongAt(posicionid).printPlaylist();
		} else{
			pcollection->getSongAt(posicionid).printPlaylist();
		} 
	}
}

void Playlist::manage(){ //managePlaylist	
	char opcion,op;
	
	do{
		cout<<"1- Add songs";
		cout<<"\n2- Remove songs";
		cout<<"\n3- Clear";
		cout<<"\nq- Back to main menu";
		cout<<"\nOption: ";
		
		cin>>opcion;
		Utils::limpiarBuffer();
		
		switch (opcion){
			case '1':
				addSongs();
				break;
			case '2':
				removeSongs();
				break;
			case '3':
				if (idSong.size()==0){
					cout<<EMPTYPLAY;
				}
				else{
					cout<<CLEARPLAY;
					cin>>op;
					Utils::limpiarBuffer();
					if (op=='Y')
						clearSongs();
				}
				break;
			case 'q':
				break;
			default:
				cout<<ERRNOOPTION;
				break;
		}
	}while (opcion !='q');
}

void Playlist::addSongs(){ //addSongsPlaylist
	int i,j;
	vector<int> idCanciones;
	int contador;
	
	if (pcollection->size()==0){
		cout<<EMPTYCOL<<endl;
	} else{
		pcollection->show();
		cout<<SELSONGSEP;
		idCanciones=separa();
		
		for(i=0;(unsigned)i<idCanciones.size();i++){
			contador=0;
			for(j=0;j<pcollection->size();j++){
				if (idCanciones[i]==pcollection->getSongAt(j).getId()){
					idSong.push_back(idCanciones[i]);
					contador++;
				}
			}
			if (contador==0){
				cout<<ERRNOSONG<<idCanciones[i]<<endl;
			}
		}
	}
}

vector<int> Playlist::separa() const{   //Este modulo se encarga de leer un string y pasarlo a un vector de enteros
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

void Playlist::removeSongs(){ //removeSongsPlaylist
	unsigned int i;
	int j;
	vector<int> listaremove;
	int contador;
	int position=-1;
	int tamPlaylist;
	
	if (idSong.size()==0){
		cout<<EMPTYPLAY<<endl;
	} else{
		print(position);
		cout<<SELSONGSEP;
		listaremove=separa();
		tamPlaylist = idSong.size();
		
		for(i=0;i<listaremove.size();i++){
			contador=0;
			for (j=tamPlaylist-1; j>=0;j--){
				if(listaremove[i]==idSong[j]){
					idSong.erase(idSong.begin()+j);
					contador++;
				}
			}
			if (contador==0)
				cout<<ERRSONG<<listaremove[i]<<NOTPLAY<<endl;
		}
	}
}

void Playlist::clearSongs(){ //clearSongsPlaylist
	idSong.clear();	
}

bool Playlist::saveXSPF() const{ //saveXSPFPlaylist
	ofstream fichero;
	bool guardado=true;
	unsigned int i;
	Song cancion;
	unsigned int posicionid;
	string filename;
	
	if (idSong.size()>0){
		cout<<FILEN;
		getline(cin,filename);
		
		fichero.open(filename.c_str(),ios::out);
		if (fichero.is_open()){
			fichero << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl
			     << "<playlist version=\"1\" xmlns=\"http://xspf.org/ns/0/\">" << endl
			     << "  <title>MiniTunes playlist</title>" << endl
			     << "  <trackList>" << endl;
			     
			for(i=0;i<idSong.size();i++){
				posicionid=pcollection->findIdSong(idSong[i]); //Encuentra en la collection la cancion del playlist
				pcollection->getSongAt(posicionid).printXSPF(fichero); //Escribir en el fichero esa cancion
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

void Playlist::play() const{ //playPlaylist
	unsigned int position;
	char opcion=' ';
	int tammaxplay;
	int i=0;
	bool defecto=true;
	
	
	if (idSong.size()>0){
		Player player;
		
		tammaxplay=idSong.size();  //almaceno el tamaño de la playlist, para controlar la reproduccion circular
		
		while(opcion!='q'){
			if (opcion=='s' || opcion=='a' || defecto){  //El bool defecto sirve para que comience la reproduccion la primera vez que se llame a la funcion
				position=i;                          //Posiscion de la cancion que se tiene que reproducir
				print(position);   //Se le manda la posicion de la cancion que se esta reproduciendo
				position=idSong[position];      //Se reutiliza la variable position para almacenar el id de la cancion
				player.playSong(pcollection->getSongAt(position-1).getUrl());  //Comienza la reproduccion
				defecto=false;
			}
			
			cout<<COMMAND;
			cin>>opcion;
			Utils::limpiarBuffer();
			
			switch (opcion){
				case 'p':
					player.pauseResumeSong();  //Pausar/reanudar
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
					//Fin de reproduccion.
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

bool Playlist::read(ifstream &fichero){ //readPlaylist
	bool notread=true;
	int tamanyo;
	int i;
	int songid;
	
	if (fichero.is_open()){
		fichero.read((char *) &tamanyo, sizeof(int));  //Lee el tamaño de la playlist
		
		for(i=0;i<tamanyo;i++){
			fichero.read((char *) (&songid),sizeof(int));  //Lee las canciones del playlist
			idSong.push_back(songid);             //Añade esas canciones
		}
	} else {
		notread=false;
	}
	return (notread);
}

bool Playlist::write(ofstream &fichero) const{ //writePlaylist
	bool notwrite=true;
	int tamanyo;
	int i;
	
	if (fichero.is_open()){
		tamanyo=idSong.size();
		fichero.write((char *) &tamanyo, sizeof(int));
		
		for(i=0;i<tamanyo;i++){
			fichero.write((char *) (&idSong[i]),sizeof(int));  //Se van guardando todas las canciones de la coleccion
		}
	} else {
		notwrite=false;
	}
	return (notwrite);
}
