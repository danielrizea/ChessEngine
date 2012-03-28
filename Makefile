CC=g++
CFLAGS=-c -Wall
LDFLAGS= -O3
SOURCES=ChessBoard.cpp Game.cpp main.cpp Tools.cpp Xboard.cpp Mind.cpp MoveTree.cpp Book.cpp
EXECUTABLE=xchess
OBJECTS=$(SOURCES:.cpp=.o)
HEADERS=stuff.h $(SOURCES:.cpp=.h)

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $(EXECUTABLE) $(OBJECTS) 

%.o: %.cpp %.h
	$(CC) $(CFLAGS) $(<) -o $(@)

clean:
	rm -f *.o $(EXECUTABLE)

run: all
	xboard -debug -fcp ./$(EXECUTABLE) -fd "`pwd`"
battle: all
	xboard -debug -fcp ./$(EXECUTABLE) -scp ./$(EXECUTABLE)
gnu: all
	xboard -fcp gnuchess -scp ./$(EXECUTABLE)
