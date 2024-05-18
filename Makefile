CPPFLAGS = -std=c++20 -Wall -Werror -pedantic -ggdb -pthread
HDRS = TCPServer.h TCPClient.h Log.h
ICE4 = participant coordinator

%.o : %.cpp $(HDRS)
	g++ $(CPPFLAGS) -c $< -o $@

ice4 : $(ICE4)

participant : TCPServer.o participant.o Log.o
	g++ -lpthread $^ -o $@

coordinator : TCPClient.o coordinator.o Log.o
	g++ -lpthread $^ -o $@

clean :
	rm -f *.o $(ICE4)

