objects = tokenizer.o exit.o cd.o pwd.o redir.o misc.o

all : cli 

cli : cli.o $(objects)
	gcc -Wall -o $@ $< $(objects)

%.o : %.c
	gcc -Wall -c -g $<
	
clean : 
	rm -f cli cli.o
	rm -f $(objects)
	
