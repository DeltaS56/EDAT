/**
 * @file  music.c
 * @author Profesores EDAT
 * @date February 2026
 * @mersion 1.0
 * @brief Library to manage ADT Music
 *
 * @details 
 * 
 * @see
 */

#include <string.h>
#include "music.h"

#define STR_LENGTH 64
#define MAX_STATES 2
struct _Music {
    long id;
    char title[STR_LENGTH];
    char artist[STR_LENGTH];
    unsigned short duration;
    State state;
};

/*----------------------------------------------------------------------------------------*/
/*
Private function:
*/
Status music_setField (Music *m, char *key, char *value);

Status music_setField (Music *m, char *key, char *value) {
  if (!key || !value) return ERROR;

  if (strcmp(key, "id") == 0) {
    return music_setId(m, atol(value));
  } else if (strcmp(key, "title") == 0) {
    return music_setTitle(m, value);
  } else if (strcmp(key, "artist") == 0) {
    return music_setArtist(m, value);
  } else if (strcmp(key, "duration") == 0) {
    return music_setDuration(m, atol(value));
  } else if (strcmp(key, "state") == 0) {
    return music_setState(m, (State)atoi(value));
  }

  return ERROR;
}

/*----------------------------------------------------------------------------------------*/

Music *music_initFromString(char *descr) {
  Music *m;
  char *p;
  char *key_start;
  char *value_start;
  char *buffer;

  if (!descr) return NULL;

  buffer = strdup(descr);          /* copiar entrada */
  if (!buffer) return NULL;

  m = music_init();
  if (!m) {
    free(buffer);
    return NULL;
  }

  p = buffer;

  while (*p) {
    /* Skip whitespace */
    while (*p == ' ' || *p == '\t' || *p == '\n')
      p++;

    if (!*p) break;

    /* Parse key */
    key_start = p;
    while (*p && *p != ':')
      p++;

    if (!*p) break;
    *p++ = '\0';   /* terminate key */

    /* Expect opening quote */
    if (*p != '"') break;
    p++;

    /* Parse value */
    value_start = p;
    while (*p && *p != '"')
      p++;

    if (!*p) break;
    *p++ = '\0';   /* terminate value */

    music_setField(m, key_start, value_start);
  }

  free(buffer);
  return m;
}

Music *music_init(){
  Music *music = NULL, *aux = NULL;

  aux = calloc(1, sizeof(Music));
  if (aux == NULL) return NULL;
  music = aux;

  music->id = 0;
  strcpy(music->title, "");
  strcpy(music->artist, "");
  music->duration = 0;
  music->state = NOT_LISTENED;

  return music;
};

void music_free (void *m){
  if (m == NULL) return;

  free(m);
  m = NULL;
};

long music_getId (const Music *m){
  if (m == NULL) return -1;

  return m->id;
};

const char* music_getTitle (const Music * m){
  if (m == NULL) return NULL;

  return m->title;
};

const char* music_getArtist (const Music * m){
  if (m == NULL) return NULL;

  return m->artist;
};

unsigned short music_getDuration (const Music * m){
  if (m == NULL) return -1;

  return m->duration;
};

State music_getState (const Music * m){
  if (m == NULL) return ERROR_MUSIC;

  return m->state;
};

Status music_setId (Music * m, const long id){
  if (m == NULL || !id) return ERROR;

  m->id = id;

  return OK;
};

Status music_setTitle (Music * m, const char * title){
  if (m == NULL || title == NULL) return ERROR;

  strcpy(m->title, title);

  return OK;
};

Status music_setArtist (Music * m, const char * artist){
  if (m == NULL || artist == NULL) return ERROR;

  strcpy(m->artist, artist);

  return OK;
};

Status music_setDuration (Music * m, const unsigned short duration){
  if (m == NULL || duration < 0) return ERROR;

  m->duration = duration;

  return OK;
};

Status music_setState (Music * m, const State state){
  if (m == NULL || state < 0 || state > 2) return ERROR;

  m->state = state;

  return OK;
};

int music_cmp (const void * m1, const void * m2){
  if (m1 == NULL || m2 == NULL) return -1;
  Music song1, song2;

  song1.id = music_getId(m1);
  song2.id = music_getId(m2);
  if (music_getArtist(m1)) 
    strcpy(song1.artist, music_getArtist(m1)); else return -1;
  strcpy(song2.artist, music_getArtist(m2));
  strcpy(song1.title, music_getTitle(m1));
  strcpy(song2.title, music_getTitle(m2));

  if (song1.id < 0 || song2.id < 0 || song) return -1;
  if(!(song1.id  == song2.id)) return 1;
  else if ()


};

void * music_copy (const void * src);

int music_plain_print (FILE * pf, const void * m);


int music_formatted_print (FILE * pf, const void * m) {
	Music * aux;
	int counter = 0, minutes, sec;
	if (!pf || !m) return -1;

	aux = (Music*) m;
	
	if (!aux->duration || aux->duration <= 0) return -1;
	minutes = aux->duration / 60;
    sec = aux->duration % 60;
	
	counter = fprintf(pf, "\t ɴᴏᴡ ᴘʟᴀʏɪɴɢ: %s\n", aux->title);
	counter += fprintf(pf, "\t • Artist %s •\n", aux->artist);
	counter += fprintf(pf, "\t──────────⚪──────────\n");
	counter += fprintf(pf, "\t\t◄◄⠀▐▐ ⠀►►\n");
	counter += fprintf(pf, "\t 0:00 / %02d:%02d ───○ 🔊⠀\n\n", minutes, sec);
	
	return counter;
}



