CPPFLAGS = -std=c++20 -Wall -Werror -pedantic -ggdb -pthread
HDRS = TCPServer.h TCPClient.h Log.h 2PC_Participant.h 2PC_Coordinator.h MessageTypes.h
p2 = participant coordinator

# Pattern rule for compiling .cpp files to .o files
%.o : %.cpp $(HDRS)
	g++ $(CPPFLAGS) -c $< -o $@

# Target for the final executables
p2 : $(p2)

# Build rules for each executable
participant : TCPServer.o participant.o
	g++ -lpthread $^ -o $@

coordinator : TCPClient.o coordinator.o
	g++ -lpthread $^ -o $@

# Clean rule to remove object files and executables
clean :
	rm -f *.o $(p2)
