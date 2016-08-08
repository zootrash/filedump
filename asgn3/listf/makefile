objects = tokenizer.o

all : listf

listf : listf.o $(objects)
	gcc -Wall -o $@ $< $(objects)

%.o : %.c
	gcc -Wall -c -g $<
	
clean : 
	rm -f listf listf.o
	rm -f $(objects)
	
