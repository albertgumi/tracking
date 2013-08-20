CC=g++

FILE=collision

all:
	$(CC) -c `root-config --cflags` $(FILE).cc
	$(CC) -o $(FILE) `root-config --glibs` $(FILE).o

clean:
	$(RM) $(FILE) $(FILE).o
