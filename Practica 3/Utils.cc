#include "Utils.h"


bool Utils::saveData(const Collection &collection, const Playlist &playlist){
	ofstream fichero;
	
	fichero.open(FILEBINARY, ios::out | ios::binary);
	
	if (collection.size()>0){    //Si la coleccion esta vacia no se guarda nada
		if (fichero.is_open()){
			
			playlist.write(fichero);
			collection.write(fichero);
			
			fichero.close();
		} else{
			cout<<FICHERR<<FILEBINARY<<endl;
		}
	} else{
		cout<<EMPTYCOL<<endl;
	}
	return (true);
}

bool Utils::loadData(Collection &collection, Playlist &playlist){
	ifstream fichero;
	char confirm;
	
	fichero.open(FILEBINARY, ios::in | ios::binary);
	
	if (fichero.is_open()){
		if (playlist.size()>0){
			playlist.clearSongs();
		}
		
		playlist.read(fichero);
		
		if (collection.size()>0){   //Si la coleccion tiene algun valor se pregunta el borrado, si no tiene ningun valor se lee directamente
			cout<<CONFIRMDELCOL;
			cin>>confirm;
			if (confirm=='Y'){
				collection.clear();
				collection.read(fichero);
			}
		} else{
			collection.read(fichero);
		}
		fichero.close();
	} else{
		cout<<FICHERR<<FILEBINARY<<endl;
	}
	return (true);
}

bool Utils::manageArguments(int argc, char *argv[], Collection &collection, Playlist &playlist){
	bool managed=false;
	
	if (argc<4  && argc>1){                             //Minimo y maximo de argumentos correctos
		if (strcmp(argv[1],"-d")==0  &&  argc==2){  //Si el segundo argumento es -d, y no hay mas argumentos ( ./minitunes -d )
			loadData(collection, playlist);     //Se cargan los datos de minitunes.dat (por defecto)
			managed=true;
		} else if (strcmp(argv[1],"-i")==0   &&   argc==3  &&  strcmp(argv[2],"-d")!=0) { //Si el segundo argumento es -i, hay tres argumentos, y el ultimo no es -d
			collection.importJSON(argv[2]);     //Se importa desde el fichero en formato JSON, pasando el tercer argumento como nombre de fichero
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

void Utils::limpiarBuffer() { 
	while(cin.get() !='\n');
} 