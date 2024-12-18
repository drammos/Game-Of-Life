# paths
MODULES = ../modules
INCLUDE = ../include
LIB = ../lib
FILES = ../Input

# Compile options. Το -I<dir> λέει στον compiler να αναζητήσει εκεί include files
CFLAGS = -Wall -no-pie -fPIE -g -I$(INCLUDE)

# Αρχεία .o
INCL = $(INCLUDE)/LifeState.h $(INCLUDE)/ADTVector.h $(INCLUDE)/ADTMap.h $(INCLUDE)/ADTList.h
OBJS = lifegame.o $(MODULES)/LifeState.o $(MODULES)/ADTVector.o $(MODULES)/ADTMap.o $(MODULES)/ADTList.o $(LIB)/libbmp.a

# the executable program
EXEC = lifegame

# parameters
ARGS = test1.rle 0 0 200 200 500 1 1 10 output.gif

# το ίδιο για το libbmp.a
$(LIB)/libbmp.a:
	$(MAKE) -C $(LIB) libbmp.a

$(EXEC): $(OBJS) $(INCL)
	gcc $(OBJS) -o $(EXEC) -lm

clean:
	rm -f $(OBJS) $(EXEC)

run: $(EXEC)
	./$(EXEC) $(ARGS)

gdb: $(OBJS)
	gcc $(OBJS) -g3 -o $(EXEC) -lm
	gdb $(EXEC)
