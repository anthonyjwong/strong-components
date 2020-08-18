/*
* Name: AJ Wong
* CruzID: 1652596
* Class: CMPS 101
* Date: 7 Jun 2019
* Desc: Finds the strong components of a Graph.
* File Name: FindComponents.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "List.h"
#include "Graph.h"

#define MAX_LEN 256

int main(int argc, char* argv[]) {

  int x,y;
  int stronglyConnectedComponents;
  List *component;
  FILE *in, *out;
  char line[MAX_LEN];
  Graph G = NULL;

  // check command line for correct number of arguments
  if( argc != 3 ) {
    printf("Usage: %s <input file> <output file>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  // open files for reading and writing
  in = fopen(argv[1], "r");
  out = fopen(argv[2], "w");
  if (in == NULL) {
    printf("Unable to read from file %s\n", argv[1]);
    exit(EXIT_FAILURE);
  }
  if (out == NULL) {
    printf("Unable to write to file %s\n", argv[2]);
    exit(EXIT_FAILURE);
  }

  fgets(line, MAX_LEN, in);
  sscanf(line, "%d", &x);
  G = newGraph(x);
  List S = newList();
  for (int i = 1; i <= getOrder(G); i++) {
    append(S, i);
  }

  while( fgets(line, MAX_LEN, in) != NULL)  {
    sscanf(line, "%d %d", &x, &y);
    if (!(x == 0 && y == 0)) {
      addArc(G, x, y);
    }
  }

  Graph tG = transpose(G);

  fprintf(out, "Adjacency list representation of G:\n");
  printGraph(out, G);

  DFS(G, S);
  DFS(tG, S);

  moveFront(S);
  while (index(S) != -1) {
    if (getParent(tG, get(S)) == NIL) {
      stronglyConnectedComponents++;
    }
    moveNext(S);
  }

  component = calloc(stronglyConnectedComponents + 1 , sizeof(List));
  for (int i = 1; i <= stronglyConnectedComponents; i++) {
    component[i] = newList();
  }

  int iter = 1;
  while (length(S) != 0) {
    prepend(component[iter], back(S));
    if (getParent(tG, back(S)) == NIL) {
      iter++;
    }
    deleteBack(S);
  }

  fprintf(out, "\nG contains %d strongly connected components:", stronglyConnectedComponents);
  for (int i = 1; i <= stronglyConnectedComponents; i++) {
    fprintf(out, "\nComponent %d: ", i);
    printList(out, component[i]);
  }
  fprintf(out, "\n");

  /* free stuff */
  freeGraph(&G);
  freeGraph(&tG);
  freeList(&S);
  for (int i = 1; i <= stronglyConnectedComponents; i++) {
    freeList(&(component[i]));
  }
  free(component);

  /* close input and output files */
  fclose(in);
  fclose(out);
  return(EXIT_SUCCESS);

}
