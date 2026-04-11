#include <stdlib.h>

#include "types.h"
#include "radio.h"
#include "music.h"
#include "stack.h"

Status mergeStacks(Stack *sin1, Stack *sin2, Stack *sout) {
  void *e = NULL;
  Stack *ps = NULL;

  if (!sin1 || !sin2 || !sout) {
    return ERROR;
  }

  while (stack_isEmpty(sin1) == FALSE && stack_isEmpty(sin2) == FALSE) {
    if (music_getDuration((Music *)stack_top(sin1)) > music_getDuration((Music *)stack_top(sin2))) {
      e = stack_pop(sin1);
    } else {
      e = stack_pop(sin2);
    }
    stack_push(sout, e);
  }

  if (stack_isEmpty(sin1) == TRUE) {
    ps = sin2;
  } else {
    ps = sin1;
  }

  while (stack_isEmpty(ps) == FALSE) {
    e = stack_pop(ps);
    stack_push(sout, e);
  }

  return OK;  
}

int main(int argc, char *argv[]) {
  FILE *file1 = NULL, *file2 = NULL;
  Radio *r1 = NULL, *r2 = NULL;
  Stack *s1 = NULL, *s2 = NULL, *s3 = NULL;
  int i;

  if (argc < 3) {
    fprintf(stdout, "Insuficientes argumentos: %s <.txt>\n", argv[0]);
    return EXIT_FAILURE;
  }

  file1 = fopen(argv[1], "r");
  file2 = fopen(argv[2], "r");
  if (!file1 || !file2) {
    if (file1) fclose(file1);
    fprintf(stdout, "Error al abrir fichero");
    return EXIT_FAILURE;
  }

  r1 = radio_init();
  r2 = radio_init();
  if (!r1 || !r2) {
    radio_free(r1);
    fclose(file1);
    fclose(file2);
    return EXIT_FAILURE;
  }

  if (radio_readFromFile(file1, r1) == ERROR || radio_readFromFile(file2, r2) == ERROR) {
    fprintf(stdout, "Error al leer la radio del archivo");
    radio_free(r1);
    radio_free(r2);
    fclose(file1);
    fclose(file2);
    return EXIT_FAILURE;
  }

  fclose(file1);
  fclose(file2);

  s1 = stack_init();
  s2 = stack_init();
  s3 = stack_init();
  if (!s1 || !s2 || !s3) {
    radio_free(r1);
    radio_free(r2);
    if (s1) stack_free(s1);
    if (s2) stack_free(s2);
    if (s3) stack_free(s3);
    return EXIT_FAILURE;
  }

  for (i = 0; i < radio_getNumberOfMusic(r1); i++) {
    stack_push(s1, radio_getSong(r1, i));
  }

  for (i = 0; i < radio_getNumberOfMusic(r2); i++) {
    stack_push(s2, radio_getSong(r2, i));
  }

  fprintf(stdout, "Playlist 0:\n");
  stack_print(stdout, s1, music_plain_print);

  fprintf(stdout, "Playlist 1:\n");
  stack_print(stdout, s2, music_plain_print);

  if (mergeStacks(s1, s2, s3) == ERROR) {
    radio_free(r1);
    radio_free(r2);
    stack_free(s1);
    stack_free(s2);
    stack_free(s3);
    return EXIT_FAILURE;
  }
  
  fprintf(stdout, "Playlist combined:\n");
  stack_print(stdout, s3, music_plain_print);
 
  radio_free(r1);
  radio_free(r2);
  stack_free(s1);
  stack_free(s2);
  stack_free(s3);

  return EXIT_SUCCESS;
}