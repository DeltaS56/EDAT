#include <stdlib.h>
#include <string.h>
#include "radio.h"
#include "music.h"
#include "stack.h"
#include "queue.h"

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
  r = NULL;
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

  music_setIndex(new_music, r->num_music);
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

Status radio_readFromFile(FILE *fin, Radio *r) {
  char line[256];
  int num_music = 0, i;
  char *tmp = NULL;
  long id_orig, id_dest;
  
  if (!fin || !r) return ERROR;

  if (fgets(line, sizeof(line), fin) == NULL) return ERROR;
  if (sscanf(line, "%d", &num_music) != 1) return ERROR;

  for (i = 0; i < num_music; i++) {
      if (fgets(line, sizeof(line), fin) == NULL) break;
      line[strcspn(line, "\r\n")] = 0;
      
      if (radio_newMusic(r, line) == ERROR) return ERROR;
  }

  while (fgets(line, sizeof(line), fin) != NULL) {
      tmp = strtok(line, " \t\n\r");
      if (tmp == NULL) continue;

      if (sscanf(tmp, "%ld", &id_orig) != 1) continue;

      while ((tmp = strtok(NULL, " \t\n\r")) != NULL) {
          if (sscanf(tmp, "%ld", &id_dest) == 1) {
              radio_newRelation(r, id_orig, id_dest);
          }
      }
  }
  return OK;
}

Music *radio_getSong(Radio *r, int n){
  if (!r) return NULL;

  return r->songs[n];
}

Status radio_depthSearch(Radio *r, long from_id, long to_id) {
  Stack *s = NULL;
  Music *m_curr = NULL, *m_near = NULL;
  int i, i_curr, i_near;
  Status st = OK;

  if (!r) return ERROR;

  for (i = 0; i < r->num_music; i++) {
    music_setState(r->songs[i], NOT_LISTENED);
  }

  i_curr = _radio_get_music_index(r, from_id);
  if (i_curr == -1) return ERROR;

  s = stack_init();
  if (!s) return ERROR;

  music_setState(r->songs[i_curr], LISTENED);
  stack_push(s, r->songs[i_curr]);

  fprintf(stdout, "Music exploration path:\n");

  while (stack_isEmpty(s) == FALSE && st == OK) {
    m_curr = (Music *)stack_pop(s);

    music_plain_print(stdout, m_curr);
    fprintf(stdout, "\n");

    if (music_getId(m_curr) == to_id) {
      break;
    } else {
      i_curr = _radio_get_music_index(r, music_getId(m_curr));
      
      for (i_near = 0; i_near < r->num_music; i_near++) {
        if (r->relations[i_curr][i_near] == TRUE) {
          m_near = r->songs[i_near];
          
          if (music_getState(m_near) == NOT_LISTENED) {
            music_setState(m_near, LISTENED);
            stack_push(s, m_near);
          }
        }
      }
    }
  }

  stack_free(s);
  return OK;
}

/*
* La mayor diferencia entre el DFS y BFS, es que mientras en el DFS, al utilizar pilas (donde lo último que entra es lo primero que sale) si lo primero que entra es 
  el destino, el algoritmo tendría que recorrer todos los caminos alternativos hasta que lo último que quedase en la pila fuese el destino. Mientras tanto, en el BFS, al 
  utilizar colas (donde lo primero que entra es lo primero que sale) el algoritmo va por niveles. Por tanto, si el destino está en el segundo nivel, da igual su orden de
  entrada, que al final como mínimo en dos turnos ya se tiene el destino, garantizando el camino más corto.
*  
* En primer lugar, si se busca el camino más corto, se debe escoger el algoritmo BFS, ya que va por niveles, y es mucho más seguro que el DFS ya que da igual si el destino
  entra antes o después mientras esté en el mismo nivel. Por ejemplo, si tengo dos caminos (A->Z y A->B->C->D) y el destino es Z, si utilizamos DFS y lo primero que
  entra es Z, entonces se tendrá que recorrer todos los caminos alternativos hasta que solo quede Z, mientras que en BFS, Z sería lo primero que se comprobaría, y luego B al
  estar en el mismo nivel. Esto es perfecto para problemas donde tienes que buscar el camino más corto de una cicudad a otro, o en problemas donde tienes que buscar la forma
  más rápida de demostrar que dos personas están conectadas.
  
  En segundo lugar, si buscamos explorar todas las opciones en vez de buscar el camino más corto, entonces se debe escoger el algoritmo DFS. Además, en cuanto a consumo de
  memoria DFS puede ser más eficiente ya que en el caso de que haya una gran cantidad de caminos, BFS iría por niveles y por tanto tendría en cuenta todos los nodos de cada
  nivel, mientras que DFS avanza secuencialmente en profundidad, es decir, avanza por un camino exacto. Esto es perfecto para problemas de laberintos, donde tu pruebas un
  camino en profundidad y si te topas con un muro, pues vuelves atrás y pruebas otro camino, o en problemas de ajedrez, si queremos que la máquina vea en profundidad un
  camino de movimientos óptimo, y si algo no cuadra, puede volver atrás y repetir.
*/
Status radio_breadthSearch(Radio *r, long from_id, long to_id) {
  Queue *q = NULL;
  Music *m_curr = NULL, *m_near = NULL;
  int i, i_curr, i_near;
  Status st = OK;

  if (!r) return ERROR;

  for (i = 0; i < r->num_music; i++) {
    music_setState(r->songs[i], NOT_LISTENED);
  }

  i_curr = _radio_get_music_index(r, from_id);
  if (i_curr == -1) return ERROR;

  q = queue_new();
  if (!q) return ERROR;

  music_setState(r->songs[i_curr], LISTENED);
  queue_push(q, r->songs[i_curr]);

  fprintf(stdout, "Music exploration path:\n");

  while (!queue_isEmpty(q) && st == OK) {
    m_curr = (Music *)queue_pop(q);

    music_plain_print(stdout, m_curr);
    fprintf(stdout, "\n");

    if (music_getId(m_curr) == to_id) break;
    else i_curr = _radio_get_music_index(r, music_getId(m_curr));

    for (i_near = 0; i_near < r->num_music; i_near++) {
      if (r->relations[i_curr][i_near] == TRUE) {
        m_near = r->songs[i_near];

        if (music_getState(m_near) == NOT_LISTENED) {
          music_setState(m_near, LISTENED);
          queue_push(q, m_near);
        }
      }
    }
  }

  queue_free(q);
  return OK;
}