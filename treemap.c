#include "treemap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct TreeNode TreeNode;

struct TreeNode {
  Pair *pair;
  TreeNode *left;
  TreeNode *right;
  TreeNode *parent;
};

struct TreeMap {
  TreeNode *root;
  TreeNode *current;
  int (*lower_than)(void *key1, void *key2);
};

int is_equal(TreeMap *tree, void *key1, void *key2) {
  if (tree->lower_than(key1, key2) == 0 && tree->lower_than(key2, key1) == 0)
    return 1;
  else
    return 0;
}

TreeNode *createTreeNode(void *key, void *value) {
  TreeNode *new = (TreeNode *)malloc(sizeof(TreeNode));
  if (new == NULL)
    return NULL;
  new->pair = (Pair *)malloc(sizeof(Pair));
  new->pair->key = key;
  new->pair->value = value;
  new->parent = new->left = new->right = NULL;
  return new;
}

TreeMap *createTreeMap(int (*lower_than)(void *key1, void *key2)) {
  TreeMap *new = (TreeMap *)malloc(sizeof(TreeMap));
  new->root = NULL;
  new->current = NULL;
  new->lower_than = lower_than;
  return new;
}
// new->lower_than = lower_than;

void insertTreeMap(TreeMap *tree, void *key, void *value) {
  if (tree == NULL || key == NULL || value == NULL) {
    return;
  }
  if (tree->root == NULL) {
    tree->root = createTreeNode(key, value);
    tree->current = tree->root;
    return;
  }
  TreeNode *aux = tree->root;
  TreeNode *parent = NULL;
  while (aux != NULL) {
    parent = aux;
    if (is_equal(tree, key, aux->pair->key)) {
      return;
    }
    if (tree->lower_than(key, aux->pair->key)) {
      aux = aux->left;
    } else {
      aux = aux->right;
    }
  }

  TreeNode *newNode = createTreeNode(key, value);
  if (tree->lower_than(key, parent->pair->key)) {
    parent->left = newNode;
  } else {
    parent->right = newNode;
  }
  newNode->parent = parent;

  tree->current = newNode;
}

TreeNode *minimum(TreeNode *x) {
  while (x->left != NULL) {
    x = x->left;
  }
  return x;
}

void removeNode(TreeMap *tree, TreeNode *node) {
  if (node == NULL)
    return;

  if (node->left != NULL && node->right != NULL) {
    TreeNode *successor = minimum(node->right);
    node->pair = successor->pair;
    node = successor;
  }

  TreeNode *child = (node->left != NULL) ? node->left : node->right;

  if (child != NULL)
    child->parent = node->parent;

  if (node->parent == NULL)
    tree->root = child;
  else if (node == node->parent->left)
    node->parent->left = child;
  else
    node->parent->right = child;

  if (tree->root != NULL && tree->root->left == NULL &&
      tree->root->right == NULL)
    tree->root = NULL;

  free(node->pair);
  free(node);
}

void eraseTreeMap(TreeMap *tree, void *key) {
  if (tree == NULL || tree->root == NULL)
    return;

  if (searchTreeMap(tree, key) == NULL)
    return;
  TreeNode *node = tree->current;
  removeNode(tree, node);
}

Pair *searchTreeMap(TreeMap *tree, void *key) {
  TreeNode *aux = tree->root;
  while (aux != NULL) {
    if (is_equal(tree, key, aux->pair->key)) {
      tree->current = aux;
      return aux->pair;
    }
    if (tree->lower_than(key, aux->pair->key)) {
      aux = aux->left;
    } else {
      aux = aux->right;
    }
  }
  return NULL;
}

Pair *upperBound(TreeMap *tree, void *key) {
  TreeNode *aux = tree->root;
  TreeNode *ub = NULL;
  while (aux != NULL) {
    if (tree->lower_than(key, aux->pair->key) ||
        is_equal(tree, key, aux->pair->key)) {
      ub = aux;
      aux = aux->left;
    } else {
      aux = aux->right;
    }
  }

  if (ub == NULL)
    return NULL;
  return ub->pair;
}

Pair *firstTreeMap(TreeMap *tree) {
  TreeNode *aux = minimum(tree->root);
  tree->current = aux;
  return aux->pair;
}

Pair *nextTreeMap(TreeMap *tree) {
  TreeNode *aux = tree->current;
  if (aux->right != NULL) {
    tree->current = minimum(aux->right);
    return tree->current->pair;
  }
  TreeNode *parent = aux->parent;
  while (parent != NULL && aux == parent->right) {
    aux = parent;
    parent = parent->parent;
  }
  tree->current = parent;
  if (parent != NULL) {
    return parent->pair;
  }
  return NULL;
}
