#include <stdlib.h>
#include <string.h>
#include "radio.h"

#define MAX_MSC 4096

struct _Radio {
  Music *songs[MAX_MSC];
  Bool relations[MAX_MSC][MAX_MSC];
  int num_music;
  int num_relations;
};

int _radio_get_music_index(const Radio *r, long id) {
  int i;
  if (!r) return -1;

  for (i = 0; i < r->num_music; i++) {
    if (music_getId(r->songs[i]) == id) {
      return i;
    }
  }
  return -1;
}

Radio * radio_init() {
  int i, j;
  Radio *r = (Radio *)malloc(sizeof(struct _Radio));
  if (!r) return NULL;
  r->num_music = 0;
  r->num_relations = 0;

  for (i = 0; i < MAX_MSC; i++) {
    r->songs[i] = NULL;
    for (j = 0; j < MAX_MSC; j++) {
      r->relations[i][j] = FALSE;
    }
  }
  return r;
}

void radio_free(Radio *r) {
  int i;
  if (!r) return;

  for (i = 0; i < r->num_music; i++) {
    music_free(r->songs[i]);
  }
  free(r);
}

Bool radio_contains(const Radio *r, long id) {
  if (!r) return FALSE;
  return _radio_get_music_index(r, id) != -1 ? TRUE : FALSE;
}

Status radio_newMusic(Radio *r, char *desc) {
  Music *new_music;
  long id;

  if (!r || !desc || r->num_music >= MAX_MSC) return ERROR;

  new_music = music_initFromString(desc);
  if (!new_music) return ERROR;

  id = music_getId(new_music);
  if (radio_contains(r, id) == TRUE) {
    music_free(new_music);
    return OK;
  }

  r->songs[r->num_music] = new_music;
  r->num_music++;
  return OK;
}

Status radio_newRelation(Radio *r, long orig, long dest) {
  int i_orig, i_dest;
  if (!r) return ERROR;

  i_orig = _radio_get_music_index(r, orig);
  i_dest = _radio_get_music_index(r, dest);

  if (i_orig == -1 || i_dest == -1) return ERROR;

  if (r->relations[i_orig][i_dest] == FALSE) {
    r->relations[i_orig][i_dest] = TRUE;
    r->num_relations++;
  }
  return OK;
}

int radio_getNumberOfMusic(const Radio *r) {
  if (!r) return -1;
  return r->num_music;
}

int radio_getNumberOfRelations(const Radio *r) {
  if (!r) return -1;
  return r->num_relations;
}

Bool radio_relationExists(const Radio *r, long orig, long dest) {
  int i_orig, i_dest;
  if (!r) return FALSE;

  i_orig = _radio_get_music_index(r, orig);
  i_dest = _radio_get_music_index(r, dest);

  if (i_orig == -1 || i_dest == -1) return FALSE;

  return r->relations[i_orig][i_dest];
}

int radio_getNumberOfRelationsFromId(const Radio *r, long id) {
  int i_orig, i, count = 0;
  if (!r) return -1;

  i_orig = _radio_get_music_index(r, id);
  if (i_orig == -1) return -1;

  for (i = 0; i < r->num_music; i++) {
    if (r->relations[i_orig][i] == TRUE) {
      count++;
    }
  }
  return count;
}

long *radio_getRelationsFromId(const Radio *r, long id) {
  int i_orig, i, count, index = 0;
  long *relations_array;

  if (!r) return NULL;

  i_orig = _radio_get_music_index(r, id);
  if (i_orig == -1) return NULL;

  count = radio_getNumberOfRelationsFromId(r, id);
  if (count <= 0) return NULL;

  relations_array = (long *)malloc(count * sizeof(long));
  if (!relations_array) return NULL;

  for (i = 0; i < r->num_music; i++) {
    if (r->relations[i_orig][i] == TRUE) {
      relations_array[index++] = music_getId(r->songs[i]);
    }
  }
  return relations_array;
}

int radio_print(FILE *pf, const Radio *r) {
  int i, j, printed = 0;
  if (!pf || !r) return -1;

  for (i = 0; i < r->num_music; i++) {
    printed += music_plain_print(pf, r->songs[i]);
    printed += fprintf(pf, ": ");
    for (j = 0; j < r->num_music; j++) {
      if (r->relations[i][j] == TRUE) {
        printed += music_plain_print(pf, r->songs[j]);
        printed += fprintf(pf, " ");
      }
    }
  printed += fprintf(pf, "\n");
  }
  return printed;
}
