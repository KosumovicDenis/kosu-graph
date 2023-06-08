/**
 * @file graph.h
 * @brief Definition of the graph class and related data structures.
 */

#pragma once

#include <stdint.h>
#include <iostream>
#include <math.h>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <algorithm>

/**
 * @def LOG(x)
 * @brief Macro for logging messages to the console.
 */
#define LOG(x) std::cout << x << std::endl;

/**
 * @struct Tile
 * @brief Represents a tile in the graph.
 */
struct Tile
{
  int32_t y, x, z;

  /**
   * @brief Overloaded output stream operator for printing Tile objects.
   * @param os The output stream.
   * @param t The Tile object to be printed.
   * @return The output stream after printing the Tile object.
   */
  friend std::ostream &operator<<(std::ostream &os, const Tile &t);

  /**
   * @brief Overloaded input stream operator for reading Tile objects.
   * @param is The input stream.
   * @param t The Tile object to store the read values.
   * @return The input stream after reading the Tile object.
   */
  friend std::istream &operator>>(std::istream &is, Tile &t);

  /**
   * @brief Overloaded equality operator for comparing Tile objects.
   * @param a The first Tile object.
   * @param b The second Tile object.
   * @return True if the Tile objects are equal, false otherwise.
   */
  friend bool operator==(const Tile &a, const Tile &b);

  /**
   * @brief Overloaded less than operator for comparing Tile objects.
   * @param a The first Tile object.
   * @param b The second Tile object.
   * @return True if the first Tile object is less than the second, false otherwise.
   */
  friend bool operator<(const Tile &a, const Tile &b);

  /**
   * @brief Overloaded greater than operator for comparing Tile objects.
   * @param a The first Tile object.
   * @param b The second Tile object.
   * @return True if the first Tile object is greater than the second, false otherwise.
   */
  friend bool operator>(const Tile &a, const Tile &b);
};

/**
 * @struct HalfEdge
 * @brief Represents a half-edge in the graph.
 */
struct HalfEdge
{
  HalfEdge *next_edge;
  int32_t vertex_index;
  uint16_t weight;
};

/**
 * @struct Vertex
 * @brief Represents a vertex in the graph.
 */
struct Vertex
{
  Tile tile;
  HalfEdge *adjacency_list;

  /**
   * @brief Constructs a Vertex object with the given tile and adjacency list.
   * @param t The tile associated with the vertex.
   * @param adjacency_list The adjacency list of the vertex.
   * @param visited Flag indicating if the vertex has been visited during graph traversal.
   */
  Vertex(Tile t, HalfEdge *adjacency_list, bool visited);

  /**
   * @brief Constructs a Vertex object with the given tile.
   * @param t The tile associated with the vertex.
   */
  Vertex(Tile t);
};

/**
 * @class graph
 * @brief Represents a graph data structure.
 */
class graph
{
private:
  std::vector<Vertex> graph_;

public:
  /**
   * @brief Default constructor for the graph class.
   */
  graph();

  /**
   * @brief Destructor for the graph class.
   */
  ~graph();

  /**
   * @brief Adds a new vertex (node) to the graph with the given tile.
   * @param tile The tile associated with the new vertex.
   * @return True if the vertex is successfully

    added, false if it already exists.
   */
  bool AddVertex(Tile tile);

  /**
   * @brief Adds an edge between two vertices in the graph.
   * @param from The tile associated with the source vertex.
   * @param to The tile associated with the target vertex.
   * @param weight The weight of the edge.
   * @return True if the edge is successfully added, false if any of the conditions are not met.
   */
  bool AddEdge(Tile from, Tile to, uint16_t weight);

  /**
   * @brief Changes the weight of an existing edge between two vertices in the graph.
   * @param from The tile associated with the source vertex.
   * @param to The tile associated with the target vertex.
   * @param weight The new weight of the edge.
   * @return True if the weight is successfully changed, false if any of the conditions are not met.
   */
  bool ChangeTileWeight(Tile from, Tile to, uint16_t weight);

  /**
   * @brief Changes the weight of the adjacency list of a tile in the graph.
   * This function updates the weight of all edges in the adjacency list of the specified tile.
   * @param tile The tile associated with the adjacency list.
   * @param weight The new weight to assign to the adjacency list edges.
   * @return True if the weight is successfully changed, false if the tile is not found.
   */
  bool ChangeTileAdjacencyListWeight(Tile tile, uint16_t weight);

  /**
   * @brief Removes an edge between two vertices in the graph.
   * @param from The tile associated with the source vertex.
   * @param to The tile associated with the target vertex.
   * @return True if the edge is successfully removed, false if any of the conditions are not met.
   */
  bool RemoveEdge(Tile from, Tile to);

  /**
   * @brief Removes the adjacency list of a tile from the graph.
   * 
   * This function removes the adjacency list of the specified tile from the graph.
   * 
   * @param tile The tile associated with the adjacency list to remove.
   * @return True if the adjacency list is successfully removed, false if the tile is not found.
   */
  bool RemoveTileAdjacencyList(Tile tile);  

  /**
   * @brief Returns the number of vertices in the graph.
   * @return The number of vertices in the graph.
   */
  int NumVertices();

  /**
   * @brief Returns the number of edges in the graph.
   * @return The number of edges in the graph.
   */
  int NumEdges();

  /**
   * @brief Calculates the degree of a given vertex in the graph.
   * @param tile The tile associated with the vertex.
   * @param degree The calculated degree of the vertex.
   * @return True if the vertex exists and the degree is calculated successfully, false otherwise.
   */
  bool NodeDegree(Tile tile, int &degree);

  /**
   * @brief Checks if two vertices are adjacent (connected by an edge) in the graph.
   * @param tile1 The tile associated with the first vertex.
   * @param tile2 The tile associated with the second vertex.
   * @return True if the vertices are adjacent, false otherwise.
   */
  bool AreAdjacent(Tile tile1, Tile tile2);

  /**
   * @brief Returns the adjacency list of a vertex as a vector of tiles.
   * @param tile The tile associated with the vertex.
   * @return The adjacency list of the vertex as a vector of tiles.
   */
  std::vector<Tile> GetAdjacencyList(Tile tile);

  /**
   * @brief Returns the weighted adjacency list of a vertex.
   * @param tile The tile associated with the vertex.
   * @return The weighted adjacency list of the vertex as a vector of tile-weight pairs.
   */
  std::vector<std::pair<Tile, uint16_t>> GetWeightedAdjacencyList(Tile tile);

  /**
   * @brief Returns the index of the given tile in the graph vector.
   * @param tile The tile associated with the vertex.
   * @return The index of the tile in the graph vector, or -1 if not found.
   */
  int32_t GetNode(const Tile &tile);

  /**
   * @brief Finds a path between two vertices in the graph using Depth-First Search (DFS) algorithm.
   * @param start The tile associated with the start vertex.
   * @param goal The tile associated with the goal vertex.
   * @param path The vector to store the tiles of the found path.
   * @param len The length of the found path.
   */
  void FindPathDFS(Tile start, Tile goal, std::vector<Tile> &path, int &len);

  /**
   * @brief Finds a path between two vertices in the graph using the A* algorithm.
   * @param start The tile associated with the start vertex.
   * @param goal The tile associated with the goal vertex.
   * @param path The vector to store the tiles of the found path.
   * @param len The length of the found path.
   * @param direction The direction of the search.
   */
  void FindPathAStar(const Tile &start, const Tile &goal, std::vector<Tile> &path, int &len, int const direction);

  /**
   * @brief Prints the graph.
   */
  void PrintGraph();

  /**
   * @brief Prints the maze.
   */
  void PrintMaze();

  /**
   * @brief Prints the maze path.
   * @param path The path to be printed.
   */
  void PrintMazePath(std::vector<Tile> &path);
};
