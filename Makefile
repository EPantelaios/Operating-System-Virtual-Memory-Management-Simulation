OBJS 	= main.o hashtable.o lru.o second_chance.o functions.o
SOURCE	= main.c hashtable.c lru.c second_chance.c functions.c
HEADER  = hashtable.h lru.h second_chance.h functions.h
OUT  	= main
CC	= gcc
FLAGS   = -std=gnu99 -c -Wall
# -c flag generates object code for separate files

$(OUT): $(OBJS)
	$(CC) $(OBJS) -o $@

#Compile the individual files separately.
main.o: src/main.c
	$(CC) $(FLAGS) src/main.c 

hashtable.o: src/hashtable.c include/hashtable.h
	$(CC) $(FLAGS) src/hashtable.c
	
lru.o: src/lru.c include/lru.h
	$(CC) $(FLAGS) src/lru.c

second_chance.o: src/second_chance.c include/second_chance.h
	$(CC) $(FLAGS) src/second_chance.c

functions.o: src/functions.c include/functions.h
	$(CC) $(FLAGS) src/functions.c


clean:
	rm -f $(OBJS) $(OUT)