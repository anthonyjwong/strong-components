/*
* Name: AJ Wong
* CruzID: 1652596
* Class: CMPS 101
* Date: 7 Jun 2019
* Desc: Implementation file for Graph ADT
* File Name: Graph.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "Graph.h"

// GraphObj
typedef struct GraphObj {
  List *adj;
  int order;
  int size;
  int *color; // white = 0, grey = 1, black = 2
  int *discover;
  int *finish;
  int *parent;
} GraphObj;

// newGraph()
// constructor for the Graph type
Graph newGraph(int n) {
  Graph G = malloc(sizeof(GraphObj));
  G->adj = calloc(n + 1, sizeof(List));
  G->color = calloc(n + 1, sizeof(int));
  G->discover = calloc(n + 1, sizeof(int));
  G->finish = calloc(n + 1, sizeof(int));
  G->parent = calloc(n + 1, sizeof(int));
  assert(G != NULL);
  assert(G->adj != NULL);
  assert(G->color != NULL);
  assert(G->discover != NULL);
  assert(G->finish != NULL);
  assert(G->parent != NULL);
  for (int i = 1; i <= n; i++) {
    G->adj[i] = newList();
    G->color[i] = 0;
    G->discover[i] = UNDEF;
    G->finish[i] = UNDEF;
    G->parent[i] = NIL;
  }
  G->order = n;
  G->size = 0;
  return G;
}

// freeGraph()
// destructor for the Graph type
void freeGraph(Graph* pG) {
  if (pG != NULL && *pG != NULL) {
    for (int i = 1; i <= (*pG)->order; i++) {
      freeList(&((*pG)->adj[i]));
    }
    free((*pG)->adj);
    free((*pG)->color);
    free((*pG)->discover);
    free((*pG)->finish);
    free((*pG)->parent);
    free(*pG);
    *pG = NULL;
  }
}

/*** Private helper functions ***/
void visit(Graph G, int x, int *time, List S) {
  if (G == NULL) {
    fprintf(stderr, "Graph Error: visit() called on NULL Graph reference\n");
    exit(EXIT_FAILURE);
  }
  G->discover[x] = ++(*time);
  G->color[x] = 1;
  moveFront(G->adj[x]);
  while (index(G->adj[x]) != -1) {
    if (G->color[get(G->adj[x])] == 0) {
      G->parent[get(G->adj[x])] = x;
      visit(G, get(G->adj[x]), time, S);
    }
    moveNext(G->adj[x]);
  }
  prepend(S, x);
  G->color[x] = 2;
  G->finish[x] = ++(*time);
}

/*** Access functions ***/
int getOrder(Graph G) {
  if (G == NULL) {
    fprintf(stderr, "Graph Error: getOrder() called on NULL Graph reference\n");
    exit(EXIT_FAILURE);
  }
  return G->order;
}

int getSize(Graph G) {
  if (G == NULL) {
    fprintf(stderr, "Graph Error: getSize() called on NULL Graph reference\n");
    exit(EXIT_FAILURE);
  }
  return G->size;
}

int getParent(Graph G, int u) {
  if (G == NULL) {
    fprintf(stderr, "Graph Error: getParent() called on NULL Graph reference\n");
    exit(EXIT_FAILURE);
  }
  if (u < 1 || u > G->order) {
    fprintf(stderr, "Graph Error: getParent() called using invalid index u\n");
    exit(EXIT_FAILURE);
  }
  return G->parent[u];
}

int getDiscover(Graph G, int u) {
  if (G == NULL) {
    fprintf(stderr, "Graph Error: getDiscover() called on NULL Graph reference\n");
    exit(EXIT_FAILURE);
  }
  if (u < 1 || u > G->order) {
    fprintf(stderr, "Graph Error: getDiscover() called using invalid index u\n");
    exit(EXIT_FAILURE);
  }
  return G->discover[u];
}

int getFinish(Graph G, int u) {
  if (G == NULL) {
    fprintf(stderr, "Graph Error: getFinish() called on NULL Graph reference\n");
    exit(EXIT_FAILURE);
  }
  if (u < 1 || u > G->order) {
    fprintf(stderr, "Graph Error: getFinish() called using invalid index u\n");
    exit(EXIT_FAILURE);
  }
  return G->finish[u];
}

void addEdge(Graph G, int u, int v) {
  if (G == NULL) {
    fprintf(stderr, "Graph Error: addEdge() called on NULL Graph reference\n");
    exit(EXIT_FAILURE);
  }
  if (u < 1 || u > G->order) {
    fprintf(stderr, "Graph Error: addEdge() called using invalid index u\n");
    exit(EXIT_FAILURE);
  }
  if (v < 1 || v > G->order) {
    fprintf(stderr, "Graph Error: addEdge() called using invalid index v\n");
    exit(EXIT_FAILURE);
  }
  addArc(G, u, v);
  addArc(G, v, u);
  G->size--; // because we want to add one edge, not two arcs
}

void addArc(Graph G, int u, int v) {
  if (G == NULL) {
    fprintf(stderr, "Graph Error: addArc() called on NULL Graph reference\n");
    exit(EXIT_FAILURE);
  }
  if (u < 1 || u > G->order) {
    fprintf(stderr, "Graph Error: addArc() called using invalid index u\n");
    exit(EXIT_FAILURE);
  }
  if (v < 1 || v > G->order) {
    fprintf(stderr, "Graph Error: addArc() called using invalid index v\n");
    exit(EXIT_FAILURE);
  }
  moveFront(G->adj[u]);
  while (index(G->adj[u]) != -1 && v > get(G->adj[u])) {
    moveNext(G->adj[u]);
  }
  if (index(G->adj[u]) != -1) { // index did not fall off list
    insertBefore(G->adj[u], v);
  }
  else {
    append(G->adj[u], v);
  }
  G->size++;

}

void DFS(Graph G, List S) {
  if (G == NULL) {
    fprintf(stderr, "Graph Error: DFS() called on NULL Graph reference\n");
    exit(EXIT_FAILURE);
  }
  if (length(S) != G->order) {
    fprintf(stderr, "Graph Error: DFS() called on Graph and List of different lengths\n");
    exit(EXIT_FAILURE);
  }
  for (int i = 1; i <= G->order; i++) { // reset discovery states
    G->color[i] = 0;
    G->discover[i] = UNDEF;
    G->finish[i] = UNDEF;
    G->parent[i] = NIL;
  }
  int time = 0;
  List copy = copyList(S);
  clear(S);
  moveFront(copy);
  while (index(copy) != -1) {
    if (G->color[get(copy)] == 0) {
      visit(G, get(copy), &time, S);
    }
    moveNext(copy);
  }
  freeList(&copy);
}

/*** Other operations ***/
Graph transpose(Graph G) {
  if (G == NULL) {
    fprintf(stderr, "Graph Error: transpose() called on NULL Graph reference\n");
    exit(EXIT_FAILURE);
  }
  Graph N = newGraph(G->order);
  for (int i = 1; i <= N->order; i++) {
    moveFront(G->adj[i]);
    while (index(G->adj[i]) != -1) {
      addArc(N, get(G->adj[i]), i);
      moveNext(G->adj[i]);
    }
  }
  return N;
}

Graph copyGraph(Graph G) {
  if (G == NULL) {
    fprintf(stderr, "Graph Error: copyGraph() called on NULL Graph reference\n");
    exit(EXIT_FAILURE);
  }
  Graph N = newGraph(G->order);
  for (int i = 1; i <= N->order; i++) {
    moveFront(G->adj[i]);
    while (index(G->adj[i]) != -1) {
      addArc(N, i, get(G->adj[i]));
      moveNext(G->adj[i]);
    }
  }
  return N;
}

void printGraph(FILE* out, Graph G) {
  if (G == NULL) {
    fprintf(stderr, "Graph Error: printGraph() called on NULL Graph reference\n");
    exit(EXIT_FAILURE);
  }
  for (int i = 1; i <= G->order; i++) {
    fprintf(out, "%d: ", i);
    printList(out, G->adj[i]);
    fprintf(out, "\n");
  }
}
