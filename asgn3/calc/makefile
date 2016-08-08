objects = tokenizer.o

all : calc

calc : calc.o $(objects)
	gcc -Wall -o $@ $< $(objects)

%.o : %.c
	gcc -Wall -c -g $<
	
clean : 
	rm -f calc calc.o
	rm -f $(objects)
