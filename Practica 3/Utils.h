#ifndef _UTILS_H_
#define _UTILS_H_

#include <iostream>
#include "Constants.h"
#include "Collection.h"
#include "Playlist.h"

using namespace std;

class Utils{
	public:
		static bool saveData(const Collection &collection, const Playlist &playlist);
		static bool loadData(Collection &collection, Playlist &playlist);
		static bool manageArguments(int argc, char *argv[], Collection &collection, Playlist &playlist);
		static void limpiarBuffer();
};

#endif