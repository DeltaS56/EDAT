#include <stdio.h>
#include <stdlib.h>
#include "music.h"
#include "radio.h"
#include "queue.h"

int main(int argc, char *argv[]) {
  int option;
  Music *m = NULL;

  if (!q) return 2;

  m = (Music *)queue_pop(q);
  if (m)
    music_formatted_print(stdout, m);
  else 
    printf("\nNo song currently playing.\n");

  return EXIT_SUCCESS;
}

