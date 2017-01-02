CFLAGS = -std=c++11 -Wall -O3
CFLAGS = -std=c++11 -Wall -g
CXX = g++

main:
	@make mySocket.o
	@echo 'Compiling server ...'
	@$(CXX) $(CFLAGS) server.cpp mySocket.o -o server.out
	@echo 'Compiling client ...'
	@$(CXX) $(CFLAGS) client.cpp mySocket.o -o client.out

%.o: %.cpp
	@echo 'Compiling $< ...'
	@g++ $(CFLAGS) -c -o $@ $<

clean:
	@rm -f *.o *.out
