objects = tokenizer.o regexp.o

all : fref

fref : fref.o $(objects)
	gcc -Wall -o $@ $< $(objects)
	
%.o : %.c
	gcc -Wall -c -g $<
	
clean : 
	rm -f fref fref.o
	rm -f $(objects)
	
