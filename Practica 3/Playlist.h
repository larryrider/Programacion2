#ifndef _PLAYLIST_H_
#define _PLAYLIST_H_

#include <iostream>
#include "Constants.h"
#include "Player.h"
#include "Song.h"
#include "Collection.h"


using namespace std;

class Playlist{
	private:
		vector<int> idSong;
		Collection *pcollection;
		
		vector<int> separa() const;
	public:
		Playlist(Collection *collection);
		int size() const;
		void print(int position) const;
		void manage();
		void addSongs();
		void removeSongs();
		void clearSongs();
		bool saveXSPF() const;
		void play() const;
		bool read(ifstream &fichero);
		bool write(ofstream &fichero) const;
};

#endif

