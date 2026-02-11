#include "types.h"
#include "music.h"

/* START [STR_LENGTH] */
#define STR_LENGTH 64
/* END [STR_LENGTH] */

struct _Music {
    long int id;
    char title[STR_LENGTH];
    char artist[STR_LENGTH];
    unsigned short duration;
    State state;
};

