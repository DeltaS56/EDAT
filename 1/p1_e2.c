#include <stdio.h>
#include <stdlib.h>
#include "radio.h"

int main(int argc, char *argv[]){
  Radio *r = NULL;
  Status status;

  r = radio_init();
  if (!r) return EXIT_FAILURE;

  status = radio_newMusic(r, "id:\"111\" title:\"Paint It, Black\" artist:\"The Rolling Stones\" duration:\"202\"");
  printf("Inserting Paint It, Black... result...: %d\n", status == OK ? 1 : 0);

  status = radio_newMusic(r, "id:\"222\" title:\"Every Breath You Take\" artist:\"The Police\" duration:\"253\"");
  printf("Inserting Every Breath You Take... result...: %d\n\n", status == OK ? 1 : 0);

  status = radio_newRelation(r, 222, 111);
  if (status == OK)
    printf("Inserting radio recommendation: 222 --> 111\n\n");

  printf("111 -> 222? %s\n", radio_relationExists(r, 111, 222) == TRUE ? "Yes" : "No");
  printf("222 -> 111? %s\n", radio_relationExists(r, 222, 111) == TRUE ? "Yes" : "No");

  printf("Number of connections from 111: %d\n\n", radio_getNumberOfRelationsFromId(r, 111));

  printf("All radio recommendations:\n");
  radio_print(stdout, r);
  
  radio_free(r);
  return EXIT_SUCCESS;
}
