#include <stdio.h>
#include <stdlib.h>
#include "bstree.h"
#include "radio.h"
#include "list.h"

int main(int argc, char *argv[]) {
  Radio *r = NULL;
  FILE *file = NULL;
  BSTree *t = NULL;
  List *pl = NULL;
  Music **songs = NULL;
  Music *m_min = NULL, *m_max = NULL;
  long min_id, max_id;
  int i, min_index, max_index;

  if (argc < 4) {
    fprintf(stderr, "Insuficientes argumentos: %s <.txt> <min_id> <max_id>\n", argv[0]);
    return EXIT_FAILURE;
  }

  file = fopen(argv[1], "r");
  if (!file) {
    fprintf(stderr, "Error al abrir %s\n", argv[1]);
    return EXIT_FAILURE;
  }

  min_id = atol(argv[2]);
  max_id = atol(argv[3]);

  r = radio_init();
  if (radio_readFromFile(file, r) == ERROR) {
    radio_free(r);
    fclose(file);
    return EXIT_FAILURE;
  }
  fclose(file);

  t = tree_init(music_plain_print, music_cmp);
  if (!t) {
    radio_free(r);
    return EXIT_FAILURE;
  }

  songs = radio_getSongs(r);
  for (i = 0; i < radio_getNumberOfMusic(r); i++) {
    tree_insert(t, songs[i]);
  }

  min_index = _radio_get_music_index(r, min_id);
  max_index = _radio_get_music_index(r, max_id);

  if (min_index == -1 || max_index == -1) {
    fprintf(stderr, "Error: IDs no encontrados en la radio.\n");
    tree_destroy(t);
    radio_free(r);
    return EXIT_FAILURE;
  }

  m_min = songs[min_index];
  m_max = songs[max_index];

  printf("Searching songs between ID %ld and %ld...\n", min_id, max_id);
  pl = tree_rangeSearch(t, m_min, m_max);

  if (pl) {
    printf("Found %d songs in range:\n", list_size(pl));
    list_print(stdout, pl, (p_list_ele_print)music_plain_print);
  } else {
    printf("Error or no songs found in range.\n");
  }

  if (pl) list_free(pl);
  tree_destroy(t);
  radio_free(r);

  return EXIT_SUCCESS;
}

/* 1.¿Qué características observas en la línea resultante?, 2.¿a qué se debe? : La lista está ordenada de menor a mayor por el ID de la canción. Esto ocurre porque nuestra función _tree_rangeSearch_rec recorre el árbol en Inorden 
  primero la rama izquierda con los nodos menores, luego el nodo central, y por último la rama derecha con los nodos mayores */