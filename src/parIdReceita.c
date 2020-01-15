#include "parIdReceita.h"

parIdReceita new_parIdReceita() {
    return (parIdReceita) {
        .IDartigo = 0,   //
        .receita  = NULL //
    };
}

void free_parIdReceita(parIdReceita* const data) { freeN(data->receita); }

int save_parIdReceita(FILE* const f, const parIdReceita* const data) {
    int written = 0;
    written += fwrite(&data->IDartigo, sizeof(uint64_t), 1, f);
    written += save_str(f, data->receita);
    return written == 2;
}

int load_parIdReceita(FILE* const f, parIdReceita* const data) {
    int written = 0;
    written += fread(&data->IDartigo, sizeof(uint64_t), 1, f);
    written += load_str(f, &data->receita);
    return written == 2;
}
