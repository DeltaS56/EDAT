#include <stdlib.h>
#include <stdio.h>

#include "types.h"
#include "radio.h"
#include "music.h"
#include "stack.h"

int main(int argc, char *argv[]) {
  FILE *file = NULL;
  Radio *r = NULL;
  long from_id, to_id;

  if (argc < 4) {
    fprintf(stdout, "Insuficientes argumentos: %s <.txt> <from_id> <to_id>\n", argv[0]);
    return EXIT_FAILURE;
  }

  file = fopen(argv[1], "r");
  if (!file) {
    fprintf(stdout, "Error al abrir fichero\n");
    return EXIT_FAILURE;
  }

  from_id = atol(argv[2]);
  to_id = atol(argv[3]);

  r = radio_init();
  if (!r) {
    fclose(file);
    return EXIT_FAILURE;
  }

  if (radio_readFromFile(file, r) == ERROR) {
    fprintf(stdout, "Error al leer la radio del archivo\n");
    radio_free(r);
    fclose(file);
    return EXIT_FAILURE;
  }
  fclose(file);

  fprintf(stdout, "Radio:\n");
  radio_print(stdout, r);

  fprintf(stdout, "\nFrom music with id: %ld\n", from_id);
  fprintf(stdout, "To music with id: %ld\n", to_id);

  if (radio_depthSearch(r, from_id, to_id) == ERROR) {
    radio_free(r);
    return EXIT_FAILURE;
  }

  radio_free(r);

  return EXIT_SUCCESS;
}

