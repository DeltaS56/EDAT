#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "radio.h"
#include "list.h"

int main(int argc, char *argv[]) {
  Radio *r = NULL;
  FILE *file = NULL;
  Music *m = NULL;
  List *pl = NULL;
  int i, half;

  if (argc < 2) {
    fprintf(stderr, "Insuficientes argumentos: %s <.txt>\n", argv[0]);
    return EXIT_FAILURE;
  }

  file = fopen(argv[1], "r");
  if (!file) {
    fprintf(stderr, "Error al abrir %s\n", argv[1]);
    return EXIT_FAILURE;
  }

  r = radio_init();
  if (radio_readFromFile(file, r) == ERROR) {
    radio_free(r);
    fclose(file);
    return EXIT_FAILURE;
  }
  fclose(file);

  pl = list_new();
  if (!pl) {
    radio_free(r);
    return EXIT_FAILURE;
  }

  for (i = 0; i < radio_getNumberOfMusic(r); i++) {
    m = radio_getSong(r, i);
    
    if(i % 2 == 0) list_pushBack(pl, m);
    else list_pushFront(pl, m);
  }

  list_print(stdout, pl, (p_list_ele_print)music_plain_print);

  printf("\nFinished inserting. Now we extract from the beginning:\n");

  half = list_size(pl) / 2;
  for (i = 0; i < half; i++) {
    m = (Music *)list_popFront(pl);
    music_plain_print(stdout, m);
    printf(" ");
  }

  printf("\n\nNow we extract from the end:\n");

  while (!list_isEmpty(pl)) {
    m = (Music *)list_popBack(pl);
    music_plain_print(stdout, m);
    printf(" ");
  }
  printf("\n");

  radio_free(r);
  list_free(pl);
  return EXIT_SUCCESS;
}
