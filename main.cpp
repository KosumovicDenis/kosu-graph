#include "graph.h"

graph g;


int main(int argc, char const *argv[])
{
  Tile t;
  t.x = 0;
  t.y = 0;
  g.AddVertex(t);
  Tile t1 = t;
  t1.x++;
  g.AddVertex(t1);
  Tile t2 = t1;
  t2.x++;
  g.AddVertex(t2);
  Tile t3 = t2;
  t3.x++;
  g.AddVertex(t3);
  g.AddEdge(t, t1, 1);
  g.AddEdge(t1, t2, 1);
  g.AddEdge(t2, t3, 1);
  std::cout << "Numero vertici: " << g.NumVertices() << std::endl;
  std::cout << "Numero angoli: " << g.NumEdges() << std::endl;
  std::cout << "Sono t e t1 adiacenti: " << g.AreAdjacent(t, t1) << std::endl;
  std::vector<Tile> adjacency_list = g.GetAdjacencyList(t);
  std::cout << "Lista di adiacenza di t: " << std::endl;
  for (int i = 0; i < adjacency_list.size(); i++)
  {
    std::cout << adjacency_list.at(i) << std::endl;
  }
  std::cout << "--------------" << std::endl;
  g.PrintGraph();
  std::cout << "--------------" << std::endl;
  std::cout << g.RemoveEdge(t, t1) << std::endl;
  g.PrintGraph();
  std::cout << "--------------" << std::endl;
  g.AddEdge(t, t1, 1);
  g.PrintGraph();
  std::vector<Tile> path;
  int len;
  g.FindPath(t, t3, path, len);
  std::cout << "Peso percorso tot: ";
  std::cout << len << std::endl;
  for (int i = 0; i < path.size(); i++)
  {
    std::cout << path.at(i) << std::endl;
  }

  return 0;
}