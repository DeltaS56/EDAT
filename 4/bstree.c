#include <stdio.h>
#include <stdlib.h>

#include "bstree.h"

/* START [_BSTNode] */
typedef struct _BSTNode {
  void *info;
  struct _BSTNode *left;
  struct _BSTNode *right;
} BSTNode;
/* END [_BSTNode] */

/* START [_BSTree] */
struct _BSTree {
  BSTNode *root;
  P_ele_print print_ele;
  P_ele_cmp cmp_ele;
};
/* END [_BSTree] */

/*** BSTNode TAD private functions ***/
void _tree_rangeSearch_rec(BSTNode *node, void *min, void *max, List *list, P_ele_cmp cmp);
int _tree_countLongSongs_rec(BSTNode *node, int min_duration);
void *_tree_find_min_rec(BSTNode *pn);
void *_tree_find_max_rec(BSTNode *pn);
Bool _tree_contains_rec(BSTNode *pn, const void *e, P_ele_cmp cmp);
BSTNode *_tree_insert_rec(BSTNode *pn, const void *e, P_ele_cmp cmp, Status *st);

BSTNode *_bst_node_new() {
  BSTNode *pn = NULL;

  pn = malloc(sizeof(BSTNode));
  if (!pn) {
    return NULL;
  }

  pn->left = NULL;
  pn->right = NULL;
  pn->info = NULL;

  return pn;
}

void _bst_node_free(BSTNode *pn) {
  if (!pn) {
    return;
  }

  free(pn);
}

void _bst_node_free_rec(BSTNode *pn) {
  if (!pn) {
    return;
  }

  _bst_node_free_rec(pn->left);
  _bst_node_free_rec(pn->right);
  _bst_node_free(pn);

  return;
}

int _bst_depth_rec(BSTNode *pn) {
  int depth_l, depth_r;

  if (!pn) {
    return 0;
  }

  depth_l = _bst_depth_rec(pn->left);
  depth_r = _bst_depth_rec(pn->right);

  if (depth_r > depth_l) {
    return depth_r + 1;
  } else {
    return depth_l + 1;
  }
}

int _bst_size_rec(BSTNode *pn) {
  int count = 0;

  if (!pn) {
    return count;
  }

  count += _bst_size_rec(pn->left);
  count += _bst_size_rec(pn->right);

  return count + 1;
}

int _bst_preOrder_rec(BSTNode *pn, FILE *pf, P_ele_print print_ele) {
  int count = 0;

  if (!pn) {
    return count;
  }

  count += print_ele(pf, pn->info);
  count += _bst_preOrder_rec(pn->left, pf, print_ele);
  count += _bst_preOrder_rec(pn->right, pf, print_ele);

  return count;
}

int _bst_inOrder_rec(BSTNode *pn, FILE *pf, P_ele_print print_ele) {
  int count = 0;

  if (!pn) {
    return count;
  }

  count += _bst_inOrder_rec(pn->left, pf, print_ele);
  count += print_ele(pf, pn->info);
  count += _bst_inOrder_rec(pn->right, pf, print_ele);

  return count;
}

int _bst_postOrder_rec(BSTNode *pn, FILE *pf, P_ele_print print_ele) {
  int count = 0;

  if (!pn) {
    return count;
  }

  count += _bst_postOrder_rec(pn->left, pf, print_ele);
  count += _bst_postOrder_rec(pn->right, pf, print_ele);
  count += print_ele(pf, pn->info);

  return count;
}

/*** BSTree TAD functions ***/
BSTree *tree_init(P_ele_print print_ele, P_ele_cmp cmp_ele) {
  BSTree *tree;

  if (!print_ele || !cmp_ele) {
    return NULL;
  }

  tree = malloc(sizeof(BSTree));
  if (!tree) {
    return NULL;
  }

  tree->root = NULL;
  tree->print_ele = print_ele;
  tree->cmp_ele = cmp_ele;

  return tree;
}

void tree_destroy(BSTree *tree) {
  if (!tree) {
    return;
  }

  _bst_node_free_rec(tree->root);
  free(tree);

  return;
}

Bool tree_isEmpty(const BSTree *tree) {
  if (!tree || !tree->root) {
    return TRUE;
  }
  return FALSE;
}

int tree_depth(const BSTree *tree) {
  if (!tree) {
    return -1;
  }

  return _bst_depth_rec(tree->root);
}

size_t tree_size(const BSTree *tree) {
  if (!tree) {
    return -1;
  }

  return _bst_size_rec(tree->root);
}

int tree_preOrder(FILE *f, const BSTree *tree) {
  if (!f || !tree) {
    return -1;
  }

  return _bst_preOrder_rec(tree->root, f, tree->print_ele) + fprintf(f, "\n");
}

int tree_inOrder(FILE *f, const BSTree *tree) {
  if (!f || !tree) {
    return -1;
  }

  return _bst_inOrder_rec(tree->root, f, tree->print_ele) + fprintf(f, "\n");
}

int tree_postOrder(FILE *f, const BSTree *tree) {
  if (!f || !tree) {
    return -1;
  }

  return _bst_postOrder_rec(tree->root, f, tree->print_ele) + fprintf(f, "\n");
}

/**** TODO: find_min, find_max, insert, contains, remove ****/
void *_tree_find_min_rec(BSTNode *pn) {
  if (!pn) return NULL;

  if (!pn->left) return pn->info;

  return _tree_find_min_rec(pn->left);
}

void *_tree_find_max_rec(BSTNode *pn) {
  if (!pn) return NULL;

  if (!pn->right) return pn->info;

  return _tree_find_max_rec(pn->right);
}

Bool _tree_contains_rec(BSTNode *pn, const void *e, P_ele_cmp cmp) {
  int c;
  if (!pn) return FALSE;

  c = cmp(e, pn->info);
  if (!c) return TRUE;
  if (c < 0) return _tree_contains_rec(pn->left, e, cmp);

  return _tree_contains_rec(pn->right, e, cmp);
}

BSTNode *_tree_insert_rec(BSTNode *pn, const void *e, P_ele_cmp cmp, Status *st) {
  int c;
  BSTNode *n = NULL;

  if (!pn) {
    n =_bst_node_new();
    if (!n) {
      *st = ERROR;
      return NULL;
    }
    n->info = (void *)e;
    *st = OK;
    return n;
  }

  c = cmp(e, pn->info);
  if (!c) {
    *st = OK;
    return pn;
  }

  if (c < 0) pn->left = _tree_insert_rec(pn->left, e, cmp, st);
  else pn->right = _tree_insert_rec(pn->right, e, cmp, st);

  return pn;
}

BSTNode *_tree_remove_rec(BSTNode *pn, const void *e, P_ele_cmp cmp_ele) {
  int cmp;
  BSTNode *ret_node = NULL;
  void *aux_info = NULL;

  if (!pn) return NULL;

  cmp = cmp_ele(e, pn->info);

  if (cmp < 0) {
    pn->left = _tree_remove_rec(pn->left, e, cmp_ele);
  } else if (cmp > 0) {
    pn->right = _tree_remove_rec(pn->right, e, cmp_ele);
  } else {
    if(!pn->left && !pn->right) {
      _bst_node_free(pn);
      return NULL;

    } else if (!pn->left && pn->right) {
      ret_node = pn->right;
      _bst_node_free(pn);
      return ret_node;

    } else if (pn->left && !pn->right) {
      ret_node = pn->left;
      _bst_node_free(pn);
      return ret_node;

    } else {
      aux_info = _tree_find_min_rec(pn->right);
      pn->info = aux_info;
      pn->right = _tree_remove_rec(pn->right, aux_info, cmp_ele);
    }
      return pn;

    }
    return pn;
}

void _tree_rangeSearch_rec(BSTNode *node, void *min, void *max, List *list, P_ele_cmp cmp) {
  int cmp_min, cmp_max;

  if (!node) return;

  cmp_min = cmp(node->info, min);
  cmp_max = cmp(node->info, max);

  if (cmp_min > 0) {
    _tree_rangeSearch_rec(node->left, min, max, list, cmp);
  }

  if (cmp_min >= 0 && cmp_max <= 0) {
    list_pushBack(list, node->info);
  }

  if (cmp_max < 0) {
    _tree_rangeSearch_rec(node->right, min, max, list, cmp);
  }
}

void *tree_find_min(BSTree *tree) {
  if (!tree || tree_isEmpty(tree) == TRUE)
  return NULL;

  return _tree_find_min_rec(tree->root);
}

void *tree_find_max(BSTree *tree) {
  if (!tree || tree_isEmpty(tree) == TRUE)
  return NULL;

  return _tree_find_max_rec(tree->root);
}

Bool tree_contains(BSTree *tree, const void *e) {
  if (!tree || tree_isEmpty(tree) == TRUE)
  return FALSE;

  return _tree_contains_rec(tree->root, e, tree->cmp_ele);
}

Status tree_insert(BSTree *tree, const void *e) {
  Status st = ERROR;
  
  if (!tree || !e) return ERROR;

  tree->root = _tree_insert_rec(tree->root, e, tree->cmp_ele, &st);
  return st;
}

Status tree_remove(BSTree *tree, const void *e) {
  if (!tree || !e) return ERROR;

  tree->root = _tree_remove_rec(tree->root, e, tree->cmp_ele);

  return OK;
}

List *tree_rangeSearch(const BSTree *tree, void *min, void *max) {
  List *list = NULL;

  if (!tree || !min || !max)
  return NULL;

  list = list_new();
  if(!list) return NULL;

  if(tree_isEmpty(tree) == FALSE) {
    _tree_rangeSearch_rec(tree->root, min, max, list, tree->cmp_ele);
  }

  return list;
}