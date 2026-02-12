#include "music.h"
#include <stdio.h>
#include <stdlib.h>

void freeall(Music *m1, Music *m2, Music *m3);

int main(int argc, char *argv[]){
  Music *m1 = NULL, *m2 = NULL, *m3 = NULL;
  int comparison;

  m1 = music_init();
  if (!m1) {
  return EXIT_FAILURE;
  }
  m2 = music_init();
  if (!m2) {
    music_free(m1);
    return EXIT_FAILURE;
  }

  music_setId(m1, 10);
  music_setTitle(m1, "Blinding Lights");
  music_setArtist(m1, "The Weeknd");
  music_setDuration(m1, 200);
  music_setId(m2, 20);
  music_setTitle(m2, "Bohemian Rhapsody");
  music_setArtist(m2, "Queen");
  music_setDuration(m2, 355);

  music_formatted_print(stdout, m1);
  music_formatted_print(stdout, m2);
  puts("");

  comparison = music_cmp(m1, m2);
  if (comparison == -1) {
    music_free(m1);
    music_free(m2);
    return EXIT_FAILURE;
  }
  printf("Equals? %s\n", comparison == 0 ? "Yes" : "No");

  m3 = music_copy(m1);
  if (!m3) {
    music_free(m1);
    music_free(m2);
    return EXIT_FAILURE;
  }

  printf("Music 2 title: %s\n", music_getTitle(m2));
  printf("Music 3 id: %ld\n", music_getId(m3));

  music_formatted_print(stdout, m1);
  music_formatted_print(stdout, m3);
  puts("");

  comparison = music_cmp(m1, m3);
  if (comparison == -1) {
    freeall(m1, m2, m3);
    return EXIT_FAILURE;
  }
  printf("Equals? %s\n", comparison == 0 ? "Yes" : "No");


  freeall(m1, m2, m3);
  return EXIT_SUCCESS;
}

void freeall(Music *m1, Music *m2, Music *m3){
  music_free(m1);
  music_free(m2);
  music_free(m3);
};
