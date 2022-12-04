
typedef void* LIST;

LIST list_node (int value_size);

LIST list_get_next (LIST this);

void list_set_next (LIST this, LIST next);

LIST list_get_prev (LIST this);

void list_set_prev (LIST this, LIST prev);

void list_del (LIST node);

void *list_value (LIST node);

int list_len (LIST node);







