OPTS=-Wall -g
OBJS=Player.o minitunes.o Song.o Collection.o Playlist.o Utils.o
CC=g++

minitunes: $(OBJS)
	$(CC) $(OPTS) $(OBJS) -o minitunes

minitunes.o: minitunes.cc Player.h Constants.h
	$(CC) $(OPTS) -c minitunes.cc

Player.o: Player.cc Player.h
	$(CC) $(OPTS) -c Player.cc

Song.o: Song.cc Song.h Constants.h
	$(CC) $(OPTS) -c Song.cc

Collection.o: Collection.cc Collection.h Constants.h
	$(CC) $(OPTS) -c Collection.cc

Playlist.o: Playlist.cc Playlist.h Constants.h
	$(CC) $(OPTS) -c Playlist.cc

Utils.o: Utils.cc Utils.h Constants.h
	$(CC) $(OPTS) -c Utils.cc

clean:
	rm -rf $(OBJS)
	rm minitunes
