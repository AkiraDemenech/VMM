
#include "vmm.h"

MANAGER new_lru (int size_limit);
int lru_access (MANAGER replace_man, int address);
int lru_remove (MANAGER replace_man, int address);

