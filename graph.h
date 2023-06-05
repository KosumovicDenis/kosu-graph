#pragma once

#include <stdint.h>
#include <iostream>
#include <math.h>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <algorithm>


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
  int32_t y, x, z;
  friend std::ostream &operator<<(std::ostream &os, const Tile &t)
  {
    return (os << "y: " << t.y << " - x: " << t.x << " - z: " << t.z);
  }
  friend std::istream &operator>>(std::istream &is, Tile &t)
  {
    LOG("Inserisci y:");
    is >> t.y;
    LOG("Inserisci x:");
    is >> t.x;
    LOG("Inserisci z:");
    is >> t.z;
    return is;
  }
  friend bool operator==(const Tile &a, const Tile &b)
  {
    if (a.x == b.x && a.y == b.y && a.z == b.z)
      return true;
    return false;
  }
  friend bool operator<(const Tile &a, const Tile &b)
  {
    if (a.z == b.z)
    {
      if (a.y == b.y)
      {
        return a.x < b.x;
      }
      else
      {
        return a.y < b.y;
      }
    }
    else 
      return a.z < b.z;
  }
  friend bool operator>(const Tile &a, const Tile &b)
  {
    if (a.z == b.z)
    {
      if (a.y == b.y)
      {
        return a.x > b.x;
      }
      else
      {
        return a.y > b.y;
      }
    }
    else
      return a.z > b.z;
  }
};

struct HalfEdge
{
  HalfEdge *next_edge;
  int32_t vertex_index;
  uint16_t weight;
};

struct Vertex
{
  Tile tile;
  HalfEdge *adjacency_list;
  Vertex(Tile t, HalfEdge *adjacency_list, bool visited)
  {
    this->tile = t;
    this->adjacency_list = adjacency_list;
  }
  Vertex(Tile t)
  {
    this->tile = t;
  }
};

class graph
{
private:
  std::vector<Vertex> graph_;
  // std::vector<HalfEdge> edges;

public:
  graph();
  ~graph();

  // The AddVertex function adds a new vertex (node) to the graph with the given tile.
  // It checks if the vertex already exists and returns false if it does.
  // Otherwise, it creates a new Vertex object and adds it to the graph.
  bool AddVertex(Tile);

  // The AddEdge function adds an edge between two vertices in the graph.
  // It checks if both vertices exist, if the edge already exists, and if the source and target vertices are the same. 
  // If any of these conditions are met, it returns false. Otherwise, it adds two half-edges between the vertices with the specified weight.
  bool AddEdge(Tile, Tile, uint16_t weight);

  // The RemoveEdge function removes an edge between two vertices in the graph.
  // It checks if both vertices exist and if the edge exists. If any of these conditions are not met, it returns false.
  // Otherwise, it removes the corresponding half-edges.
  bool RemoveEdge(Tile from, Tile to);

  // The NumVertices function returns the number of vertices in the graph by returning the size of the graph_ vector.
  int NumVertices();

  // The NumEdges function returns the number of edges in the graph.
  // It iterates over all vertices and calls the NodeDegree function to count the edges incident to each vertex.
  // The total count is divided by 2 since each edge is counted twice.
  int NumEdges();

  // The NodeDegree function calculates the degree of a given vertex (node) in the graph.
  // It counts the number of adjacent vertices by iterating over the adjacency list of the vertex.
  bool NodeDegree(Tile, int &);

  // The AreAdjacent function checks if two vertices are adjacent (connected by an edge) in the graph. 
  // It uses the AuxAreAdjacent function to determine adjacency.
  bool AreAdjacent(Tile, Tile);

  // The GetAdjacencyList function returns the adjacency list of a vertex as a vector of tiles.
  // It retrieves the adjacency list of the vertex and converts it to a vector of tiles.
  std::vector<Tile> GetAdjacencyList(Tile t);

  std::vector<std::pair <Tile, uint16_t>> GetWeightedAdjacencyList(Tile v1);

  int32_t GetNode(const Tile &t);

  // The FindPathDFS function finds a path between two vertices in the graph, using DFS algorithm.
  // It calls the FindPathAux function and populates the path vector with the tiles of.
  void FindPathDFS(Tile, Tile, std::vector<Tile> &, int &);

  void FindPathAStar(const Tile& start, const Tile& goal, std::vector<Tile>& path, int& len, int const direction);

  void PrintGraph();

  void PrintMaze();

  void PrintMazePath(std::vector<Tile>& path);

};
