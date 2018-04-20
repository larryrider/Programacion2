#ifndef _COLLECTION_H_
#define _COLLECTION_H_

#include <iostream>
#include "Song.h"

using namespace std;

class Collection{
	private:
		int idNextSong;
		vector<Song> songs;
		
		bool addSongToCol(Song cancion);
	public:
		Collection();
		Song getSongAt(int index) const;
		int findIdSong(int id) const;
		bool isSongInCollection(Song song) const;
		bool show() const;
		bool addSong();
		void editSong();
		void deleteSong();
		bool importJSON(string filename);
		bool read(ifstream &f);
		bool write(ofstream &f) const;
		int size() const;
		void clear();
};

#endif
