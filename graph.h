#pragma once

#include <stdint.h>
#include <vector>

struct Tile
{
  int32_t x, y;
};

struct HalfEdge
{
  HalfEdge *next_edge;
  Vertex *vertex_pointer;
  uint16_t weight;
};

struct Vertex
{
  Tile tile;
  Vertex *next_vertex;
  HalfEdge *adjacency_list;
  bool visited;
};

class graph
{
private:
  std::vector<Vertex> vertices;
  std::vector<HalfEdge> edges;
public:
  graph();
  ~graph();
  
  // Aggiunge nuovo vertice con etichetta la stringa. Fallisce se gia' presente
  bool addVertex(Tile);
  // Aggiunge nuovo arco tra i due nodi con etichette le due stringe e peso
  // l'intero. Fallisce se non sono presenti tutti e due i nodi o se l'arco
  // tra i due e' gia' presente.
  bool addEdge(Tile, Tile, uint16_t weight);

  // Rimuove arco tra i due nodi con etichette le due stringe
  // Fallisce se non sono presenti tutti e due i nodi o se non esiste l'arco
  // tra i due.
  bool removeEdge(Tile from, Tile to);

  // Restituisce true se il grafo e' vuoto, false altrimenti
  bool isEmpty();

  // Ritorna il numero di vertici del grafo
  int numVertices();

  // Ritorna il numero di archi del grafo
  int numEdges();

  // Calcola e ritorna (nel secondo parametro) il grado del nodo.
  // Fallisce se il nodo non esiste
  bool nodeDegree(Tile, int &);

  // Verifica se due vertici sono adiacenti (ovvero se esiste un arco)
  bool areAdjacent(Tile, Tile);

  // Ritorna la lista di adiacenza di un vertice
  std::vector<Tile> adjacentList(Tile);

  // Calcola, se esiste, un cammino tra due vertici
  void findPath(Tile, Tile, std::vector<Tile> &, int &);
};
