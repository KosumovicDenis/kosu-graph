#include <cstddef>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <fstream>
#include <string>

#include "list.h"

using namespace std;

namespace graph
{
  typedef int Weight;

  struct vertexNode; // da definire nel file graph.cpp

  typedef vertexNode *Graph; // un grafo e' identificato dal puntatore al primo vertice inserito

  const Graph emptyGraph = nullptr;

  // createEmptyGraph restituisce il grafo vuoto
  Graph createEmptyGraph();

  // Aggiunge nuovo vertice con etichetta la stringa. Fallisce se gia' presente
  bool addVertex(Tile, Graph &);

  // Aggiunge nuovo arco tra i due nodi con etichette le due stringe e peso
  // l'intero. Fallisce se non sono presenti tutti e due i nodi o se l'arco
  // tra i due e' gia' presente.
  bool addEdge(Tile, Tile, Weight, Graph &);

  // Rimuove arco tra i due nodi con etichette le due stringe
  // Fallisce se non sono presenti tutti e due i nodi o se non esiste l'arco
  // tra i due.
  bool removeEdge(Tile from, Tile to, Graph &g);

  // Restituisce true se il grafo e' vuoto, false altrimenti
  bool isEmpty(const Graph &);

  // Ritorna il numero di vertici del grafo
  int numVertices(const Graph &);

  // Ritorna il numero di archi del grafo
  int numEdges(const Graph &);

  // Calcola e ritorna (nel secondo parametro) il grado del nodo.
  // Fallisce se il nodo non esiste
  bool nodeDegree(Tile, int &, const Graph &);

  // Verifica se due vertici sono adiacenti (ovvero se esiste un arco)
  bool areAdjacent(Tile, Tile, const Graph &);

  // Ritorna la lista di adiacenza di un vertice
  list::List adjacentList(Tile, const Graph &);

  // Calcola, se esiste, un cammino tra due vertici
  void findPath(Tile, Tile, list::List &, int &, const Graph &g);
}

/* Funzioni che non caratterizzano il TDD Graph, ma che servono per input/output */
// tree::Tree readFromFile(string);
void printGraph(const graph::Graph &);