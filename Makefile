CC = gcc
EXEC = impact
MAIN = main.c 
ALLEGRO = $$(pkg-config --libs allegro-5 allegro_font-5 allegro_ttf-5 allegro_primitives-5 allegro_audio-5 allegro_acodec-5 allegro_image-5 --libs --cflags)
INCLUDE = game.c screen.c player.c sprites.c
CFLAG = Wall
OBJS = main.o game.o screen.o player.o sprites.o

all: hello

hello: $(OBJS)
	$(CC) -o $(EXEC) $(MAIN) $(INCLUDE) $(ALLEGRO)

main.o: main.c game.h
game.o: game.c game.h 
screen.o: screen.c screen.h
player.o: player.c player.h
sprites.o: sprites.c sprites.h


clean:
	-rm -f *~ *.o

purge:
	-rm -f $(OBJS) $(EXEC)