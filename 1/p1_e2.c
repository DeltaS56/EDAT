#include <stdio.h>
#include <stdlib.h>
#include "radio.h"

int main(int argc, char *argv[]){
  Radio *r = NULL;
  Status st;

  r = radio_init();
  if (!r) return EXIT_FAILURE;

  st = radio_newMusic(r, char *desc)

  return EXIT_SUCCESS;
}
