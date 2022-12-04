
#include "vmm.h"

MANAGER new_fifo (int size_limit);
int fifo_access (MANAGER subst_man, int address);
int fifo_remove (MANAGER subst_man, int address);