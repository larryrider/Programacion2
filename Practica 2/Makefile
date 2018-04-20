OPTS=-Wall -g
OBJS=Player.o minitunes.o
CC=g++

minitunes: $(OBJS)
	$(CC) $(OPTS) $(OBJS) -o minitunes

minitunes.o: minitunes.cc Player.h
	$(CC) $(OPTS) -c minitunes.cc

Player.o: Player.cc Player.h
	$(CC) $(OPTS) -c Player.cc

clean:
	rm -rf $(OBJS)
	rm minitunes
