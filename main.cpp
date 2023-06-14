#include "graph.h"

#include <fstream>

graph g;

int main(int argc, char const *argv[])
{
  // First floor
  g.AddVertex({0,0});
  g.AddVertex({0,1});
  g.AddVertex({0,2});
  g.AddVertex({0,3});
  g.AddVertex({0,5});
  g.AddVertex({1,0});
  g.AddVertex({1,1});
  g.AddVertex({1,2});
  g.AddVertex({1,3});
  g.AddVertex({1,5});
  g.AddVertex({2,0});
  g.AddVertex({2,1});
  g.AddVertex({2,2});
  g.AddVertex({2,3});
  g.AddVertex({2,4});
  g.AddVertex({2,5});
  g.AddVertex({3,0});
  g.AddVertex({3,1});
  g.AddVertex({3,2});
  g.AddVertex({3,3});
  g.AddVertex({3,4});
  g.AddVertex({3,5});
  g.AddVertex({4,2});
  g.AddVertex({4,3});
  g.AddVertex({4,4});
  g.AddVertex({4,5});
  g.AddVertex({4,5});
  g.AddVertex({5,1});
  g.AddVertex({5,2});
  g.AddVertex({5,3});
  g.AddVertex({5,4});
  g.AddVertex({5,5});
  g.AddEdge({0,0}, {0,1}, 1);
  g.AddEdge({0,0}, {1,0}, 1);
  g.AddEdge({0,1}, {0,2}, 1);
  g.AddEdge({0,1}, {1,1}, 1);
  g.AddEdge({0,2}, {0,3}, 1);
  g.AddEdge({0,2}, {1,2}, 1);
  g.AddEdge({0,3}, {1,3}, 1);
  g.AddEdge({0,5}, {1,5}, 1);
  g.AddEdge({1,0}, {1,1}, 1);
  g.AddEdge({1,1}, {2,1}, 1);
  g.AddEdge({1,2}, {1,3}, 1);
  g.AddEdge({1,3}, {2,3}, 1);
  g.AddEdge({1,5}, {2,5}, 1);
  g.AddEdge({2,0}, {2,1}, 1);
  g.AddEdge({2,0}, {3,0}, 1);
  g.AddEdge({2,1}, {2,2}, 1);
  g.AddEdge({2,1}, {3,1}, 1);
  g.AddEdge({2,2}, {3,2}, 1);
  g.AddEdge({2,3}, {2,4}, 1);
  g.AddEdge({2,4}, {2,5}, 1);
  g.AddEdge({2,4}, {3,4}, 1);
  g.AddEdge({2,5}, {3,5}, 1);
  g.AddEdge({3,0}, {3,1}, 1);
  g.AddEdge({3,1}, {3,2}, 1);
  g.AddEdge({3,2}, {3,3}, 1);
  g.AddEdge({3,2}, {4,2}, 1);
  g.AddEdge({3,3}, {4,3}, 1);
  g.AddEdge({3,4}, {4,4}, 1);
  g.AddEdge({4,2}, {4,3}, 1);
  g.AddEdge({4,2}, {5,2}, 1);
  g.AddEdge({4,3}, {4,4}, 1);
  g.AddEdge({4,3}, {5,3}, 1);
  g.AddEdge({4,4}, {4,5}, 1);
  g.AddEdge({4,4}, {5,4}, 1);
  g.AddEdge({4,5}, {5,5}, 1);
  g.AddEdge({5,1}, {5,2}, 1);
  g.AddEdge({5,2}, {5,3}, 1);
  g.AddEdge({5,3}, {5,4}, 1);
  g.AddEdge({5,4}, {5,5}, 1);


  std::cout << "Numero vertici: " << g.NumVertices() << std::endl;
  std::cout << "Numero angoli: " << g.NumEdges() << std::endl;

  std::cout << "-- Graph --\n";
  g.PrintGraph();
  std::cout << "-- Map --\n";
  g.PrintMaze();
  std::cout << "-- A* --\n";
  std::vector<Tile> path;
  int len;
  g.FindPathAStar({0,0}, {3,0}, path, len, 0);
  std::cout << "Total weigth: ";
  std::cout << len << std::endl;
  g.PrintMazePath(path);
  std::cout << "Path vector: ";
  for (int i = 0; i < path.size(); i++)
  {
    std::cout << path.at(i) << std::endl;
  }

  g.ChangeTileAdjacencyListWeight({1,1}, 5);
  g.PrintGraph();

  g.RemoveTileAdjacencyList({1,1});
  g.PrintGraph();

  g.FindPathAStar({0,0}, {3,0}, path, len, 0);
  std::cout << "Total weigth: ";
  std::cout << len << std::endl;
  g.PrintMazePath(path);
  std::cout << "Path vector: ";
  for (int i = 0; i < path.size(); i++)
  {
    std::cout << path.at(i) << std::endl;
  }
  g.PrintMaze({0,0,0});
  return 0;
}