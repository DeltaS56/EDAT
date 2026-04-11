#include <stdio.h>
#include <stdlib.h>
#include "music.h"
#include "radio.h"
#include "queue.h"

int now_playing_menu(Queue *q) {
  int option;
  Music *m = NULL;

  if (!q) return 2;

  m = (Music *)queue_pop(q);
  if (m) music_formatted_print(stdout, m);
  else printf("\nNo song currently playing.\n");
  
  printf("Upcoming:\n");
  queue_print(stdout, q, (p_queue_ele_print)music_plain_print);
  
  printf("\n1. Next song\n");
  printf("2. Exit\n");
  printf("Choose an option: ");

  scanf("%d", &option);

  return option;
}

int main(int argc, char *argv[]) {
  Radio *r = NULL;
  FILE *file = NULL;
  Queue *q = NULL;
  int option = 1, i;

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

  q = queue_new();
  if (!q) {
    radio_free(r);
    return EXIT_FAILURE;
  }

  for (i = 0; i < radio_getNumberOfMusic(r); i++) {
    queue_push(q, radio_getSong(r, i));
  }

  while (!queue_isEmpty(q) && option == 1)
    option = now_playing_menu(q);

  radio_free(r);
  queue_free(q);
  return EXIT_SUCCESS;
}