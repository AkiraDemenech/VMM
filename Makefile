simulador:
	make lru 
	make fifo
	make vmm
	cp a.out simulador.out
	cp simulador.out simulador 

fifo:	
	gcc -c fifo.c

lru:	
	gcc -c lru.c

vmm:
	gcc -g vmm.c fifo.o lru.o -o a.out 


clean:
	rm -f *.out 
	rm -f *.o	
	rm -f simulador 

clear:	
	clear
	make clean 

	
