'''
	Gerador de casos de testes 
'''

SMALL_JUMP_SIZE = 16
SMALL_JUMP_PROBABILITY = 0.4
RANDOM_JUMP_PROBABILITY = 0.3

LINES = 1000

PAGES = 256
FRAMES = 128
OFFSET = 8

VIRTUAL = PAGES << OFFSET
PHYSICAL = FRAMES << OFFSET

VIRTUAL_MASK = VIRTUAL - 1

with open('addresses.txt', 'w') as accesses:
	import random 
	virtual_address = int(VIRTUAL * random.random()) 
	random_jumps = small_jumps = False

	for ln in range(1, 1 + LINES): # tamanho do caso de teste
		progress = ln/LINES

		if random.random() > (RANDOM_JUMP_PROBABILITY * (1 - progress) + (random_jumps * progress/LINES)):
			if random.random() > (SMALL_JUMP_PROBABILITY * (1 - progress) + (small_jumps * progress/LINES)):	
				virtual_address += 1
			else:	
				virtual_address += random.randint(-SMALL_JUMP_SIZE, SMALL_JUMP_SIZE)
				print(' ', file=accesses)
				small_jumps += 1
		else:		
			if random.random() > 0.5:
				if random.random() > 0.5:					
					virtual_address = random.randint(1, VIRTUAL)
				else:	
					virtual_address = random.randint(0, virtual_address)
			else:		
				virtual_address = random.randint(virtual_address, VIRTUAL_MASK)
				
			print('\t', file=accesses)
			random_jumps += 1
			
		virtual_address &= VIRTUAL_MASK	

		print(100 * (RANDOM_JUMP_PROBABILITY * (1 - progress) + (random_jumps * progress/LINES)),'\t', 100 * (SMALL_JUMP_PROBABILITY * (1 - progress) + (small_jumps * progress/LINES)))

		print(virtual_address, file=accesses)