CC = g++
CF = -g -std=c++11
LK = g++
LF = 

hive: obj/board.o \
      obj/piece.o \
      obj/turn.o \
      obj/game.o \
      obj/ai.o \
      obj/shell.o \
      obj/main.o
	$(LK) $(LF) $^ -o $@

obj/main.o: src/main.cc
	$(CC) $(CF) -c -o $@ $<

obj/%.o: src/%.cc src/%.h
	$(CC) $(CF) -c -o $@ $<

clean: 
	rm -f obj/*