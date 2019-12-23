#include "morada.h"

morada newMorada() {
    return (morada){ .morada = strdup("undef") };
}

void freeMorada(morada m) {
    free(m.morada);
}

int morada_eCPValido (uint8_t* CP) {
    for(int i = 0; i < 7; ++i)
        if(!isdigit(CP[i]))
            return 0;

    return 1;
}

morada morada_dup (morada m) {
    morada mdup = m;
    mdup.morada = strdup(m.morada);
    return mdup;
}