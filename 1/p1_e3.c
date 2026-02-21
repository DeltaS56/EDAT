#include <stdio.h>
#include <stdlib.h>
#include "radio.h"

int main(int argc, char *argv[]){
  FILE *file = NULL;
  Radio *r = NULL;

  if (argc < 2) {
    fprintf(stdout, "Insuficientes argumentos: %s <.txt>\n", argv[0]);
    return EXIT_FAILURE;
  }
  
  file = fopen(argv[1], "r");
  if (!file) {
    fprintf(stdout, "Error al abrir el fichero %s\n", argv[1]);
    return EXIT_FAILURE;
  }

  r = radio_init();
  if (!r) {
    fclose(file);
    return EXIT_FAILURE;
  }

  if (radio_readFromFile(file, r) == ERROR) {
    fprintf(stdout, "Error al leer la radio del archivo %s\n", argv[1]);
    radio_free(r);
    fclose(file);
    return EXIT_FAILURE;
  }

  printf("Radio recommendations:\n");
  radio_print(stdout, r);

  radio_free(r);
  fclose(file);
  return EXIT_SUCCESS;
}
