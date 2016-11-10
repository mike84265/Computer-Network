CFLAGS = -O3 -Wall -std=c++11
CFLAGS_DBG = -g -Wall -std=c++11 -DDEBUG

main:
	make util.o
	make mySocket.o
	make bot.o
	g++ $(CFLAGS) bot.o util.o mySocket.o main.cpp -o bot

%.o: %.cpp
	g++ $(CFLAGS) -c -o $@ $<

debug:
	g++ $(CFLAGS_DBG) util.cpp -c -o util.o
	g++ $(CFLAGS_DBG) mySocket.cpp -c -o mySocket.o
	g++ $(CFLAGS_DBG) bot.cpp util.o mySocket.o -o bot_debug
ctags:
	@echo "Tagging..."
	@ctags -R ./

test:
	make GuessNum.o
	g++ $(CFLAGS) test.cpp GuessNum.o -o test.out

clean:
	@rm -f *.o
	@rm bot
	@rm bot_debug
