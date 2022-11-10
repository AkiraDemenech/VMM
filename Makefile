simulador:
	make vmm
	cp a.out simulador.out
	cp simulador.out simulador 

vmm:
	gcc -g vmm.c -o a.out 


clean:
	rm -f *.out 
	rm -f *.o	
	rm -f simulador 