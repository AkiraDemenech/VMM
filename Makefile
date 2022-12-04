simulador:
	make lru 
	make fifo
	make lista
	make vmm
	cp a.out simulador.out
	cp simulador.out simulador 
	echo Sucesso!

lista:	
	gcc -g -c list.c

fifo:	
	gcc -g -c fifo.c

lru:	
	gcc -g -c lru.c

vmm:
	gcc -g vmm.c list.o fifo.o lru.o -o a.out 


clean:
	rm -f *.out 
	rm -f *.o	
	rm -f simulador 

clear:	
	clear
	make clean 

doclear:
	clear
	make doclean

doclean:
	rm -f -r html/* latex/*	

doc:	
	doxygen docsimulador
	echo Documentado com sucesso!
