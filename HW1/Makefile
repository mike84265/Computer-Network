CFLAGS = -O3 -Wall -std=c++11
CFLAGS_DBG = -g -Wall -std=c++11 -DDEBUG
CFLAGS = -g -Wall -std=c++11 -DDEBUG

main:
	@make util.o
	@make mySocket.o
	@make bot.o
	@make GuessNum.o
	@echo 'Merging object files...'
	@g++ $(CFLAGS) GuessNum.o bot.o util.o mySocket.o main.cpp -o bot

%.o: %.cpp
	@echo 'Compiling $< ...'
	@g++ $(CFLAGS) -c -o $@ $<

debug:
	g++ $(CFLAGS_DBG) util.cpp -c -o util.o
	g++ $(CFLAGS_DBG) mySocket.cpp -c -o mySocket.o
	g++ $(CFLAGS_DBG) bot.cpp -c -o bot.o
	g++ $(CFLAGS_DBG) GuessNum.cpp -c -o GuessNum.o
	g++ $(CFLAGS_DBG) main.cpp util.o mySocket.o bot.o GuessNum.o -o bot_debug
ctags:
	@echo "Tagging..."
	@ctags -R ./

test:
	make calculator.o
	g++ $(CFLAGS) test4.cpp calculator.o -o test4.out

clean:
	@rm -f *.o
	@rm bot
	@rm bot_debug
