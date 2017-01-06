CFLAGS = -std=c++11 -Wall -g -DDEBUG
CFLAGS = -std=c++11 -Wall -O3
CXX = g++

main:
	@make socket 
	@make sender.out
	@make receiver.out
	@make server.out

socket:
	@echo 'Compiling mySocket ...'
	@$(CXX) $(CFLAGS) -c -o mySocket.o mySocket.cpp

%.o: %.cpp 
	@echo 'Compiling $< ...'
	@g++ $(CFLAGS) -c -o $@ $< mySocket.o

%.out: %.cpp 
	@echo 'Compiling $< ...'
	@g++ $(CFLAGS)  -o $@ $< mySocket.o 

clean:
	@rm -f *.o *.out
