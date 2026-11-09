#include <stdlib.h>
#include "db_backend.h"

void event_loop(db_t *db);

int main(void) {

    db_t *db = db_create(); 

    event_loop(db);

    db_destroy(db);

    return 0;
} 