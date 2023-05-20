#pragma once

#include <stdint.h>
#include <iostream>
#include <vector>

const std::string red("\033[0;31m");
const std::string green("\033[0;32m");
const std::string blue("\033[0;34m");
const std::string reset("\033[0m");

#define LOG(x) std::cout << x << std::endl;
#define LOG_r(x) std::cout << red << x << reset << std::endl;
#define LOG_g(x) std::cout << green << x << reset << std::endl;
#define LOG_b(x) std::cout << blue << x << reset << std::endl;

struct Tile
{
  int32_t x, y;
  friend std::ostream &operator<<(std::ostream &os, const Tile &t)
  {
    return (os << "X : " << t.x << "\tY : " << t.y);
  }
  friend std::istream &operator>>(std::istream &is, Tile &t)
  {
    LOG("Inserisci x:");
    is >> t.x;
    LOG("Inserisci y:");
    is >> t.y;
    return is;
  }
  friend bool operator==(const Tile &a, const Tile &b)
  {
    if (a.x == b.x && a.y == b.y)
      return true;
    return false;
  }
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
  std::vector<Vertex> graph_;
  std::vector<HalfEdge> edges;

public:
  graph();
  ~graph();

  // Aggiunge nuovo vertice con etichetta la tile. Fallisce se gia' presente
  bool AddVertex(Tile);

  // Aggiunge nuovo arco tra i due nodi con etichette le due tile e peso
  // l'intero. Fallisce se non sono presenti tutti e due i nodi o se l'arco
  // tra i due e' gia' presente.
  bool AddEdge(Tile, Tile, uint16_t weight);

  // Rimuove arco tra i due nodi con etichette le due tile
  // Fallisce se non sono presenti tutti e due i nodi o se non esiste l'arco
  // tra i due.
  bool RemoveEdge(Tile from, Tile to);

  // Ritorna il numero di vertici del grafo
  int NumVertices();

  // Ritorna il numero di archi del grafo
  int NumEdges();

  // Calcola e ritorna (nel secondo parametro) il grado del nodo.
  // Fallisce se il nodo con etichetta la tilez non esiste
  bool NodeDegree(Tile, int &);

  // Verifica se due vertici sono adiacenti (ovvero se esiste un arco)
  bool AreAdjacent(Tile, Tile);

  // Ritorna la lista di adiacenza di un vertice
  std::vector<Tile> GetAdjacencyList(Tile t);

  int32_t GetNode(Tile &t);

  // Calcola, se esiste, un cammino tra due vertici
  void FindPath(Tile, Tile, std::vector<Tile> &, int &);
};
