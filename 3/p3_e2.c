#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "radio.h"

int main(int argc, char *argv[]) {
  Radio *r = NULL;
  FILE *file = NULL;
  long from_id, to_id;

  if (argc < 4) {
    fprintf(stderr, "Insuficientes argumentos: %s <.txt> <from_id> <to_id>\n", argv[0]);
    return EXIT_FAILURE;
  }

  file = fopen(argv[1], "r");
  if (!file) {
    fprintf(stderr, "Error al abrir %s\n", argv[1]);
    return EXIT_FAILURE;
  }

  from_id = atol(argv[2]);
  to_id = atol(argv[3]);
  
  r = radio_init();
  if (radio_readFromFile(file, r) == ERROR) {
    radio_free(r);
    fclose(file);
    return EXIT_FAILURE;
  }
  fclose(file);

  printf("Radio:\n");
  radio_print(stdout, r);

  printf("\n------DFS------\n");
  printf("From music id: %ld\n", from_id);
  printf("To music id: %ld\n", to_id);
  printf("Output:\n");
  radio_depthSearch(r, from_id, to_id);

  printf("\n------BFS------\n");
  printf("From music id: %ld\n", from_id);
  printf("To music id: %ld\n", to_id);
  printf("Output:\n");
  radio_breadthSearch(r, from_id, to_id);

  radio_free(r);
  return EXIT_SUCCESS;
}