#ifndef _CONSTANT_H_
#define _CONSTANT_H_

#include <iostream>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include <string.h>

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


#endif
