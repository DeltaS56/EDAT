#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "music.h"
#include "radio.h"
#include "stack.h"

int show_player_menu(Stack *history);
int show_player_menu(Stack *history) {
    int option;
    Music *m = NULL;
	
	/* Obtenemos canción actual del top de la pila */
	m = (Music *)stack_top (history);
    if (m != NULL) {
		music_formatted_print(stdout, m);
    } else {
        printf("\nNo song currently playing.\n");
    }
	
	/* Imprimimos historial (pila) */
	printf("\nRecently Played:\n");
	stack_print(stdout, history, music_plain_print);
	
	/* Mostramos menu y esperamos selección */
    printf("\n1. Back to previous song\n");
    printf("2. Exit\n");
    printf("Choose an option: ");

    scanf("%d", &option);

    return option;
}

/* TODO MAIN FUNCTION */
int main(int argc, char *argv[]){
  Radio *r = NULL;
  FILE *file = NULL;
  Stack *s = NULL;
  int i, option;

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
  fclose(file);

  s = stack_init();
  if (!s) {
    radio_free(r);
    return EXIT_FAILURE;
  }

  for (i = 0; i < radio_getNumberOfMusic(r); i++) {
    stack_push(s, radio_getSong(r, i));
  }

  while (!stack_isEmpty(s)) {
    option = show_player_menu(s);
    if (option == 1 && !stack_isEmpty(s)) {
      stack_pop(s);
    } else break;
  }

  stack_free(s);
  radio_free(r);
  return EXIT_SUCCESS;
}
