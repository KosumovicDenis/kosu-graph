#include "graph.h"

graph::graph()
{
  graph_.reserve(1000);
}
graph::~graph() {}

// The GetNode function searches for a node (vertex) in the graph based on a given tile.
// It iterates over the graph and returns the index of the node if found, or -1 if not found.
int32_t graph::GetNode(const Tile &t)
{
  for (int32_t i = 0; i < graph_.size(); i++)
  {
    if (graph_.at(i).tile == t)
      return i;
  }
  return -1;
}

// The IsVertexIn function is a helper function used by other functions to check if a vertex exists in the graph.
// It also updates the index parameter if the vertex is found.
bool IsVertexIn(Tile t, int32_t &index, const std::vector<Vertex> &graph_)
{
  for (int32_t i = 0; i < graph_.size(); i++)
  {
    if (graph_.at(i).tile == t)
    {
      index = i;
      return true;
    }
  }
  return false;
}

// The AuxAreAdjacent function checks if two nodes (vertices) are adjacent in the graph by examining their adjacency lists.
bool AuxAreAdjacent(int32_t index_from, int32_t index_to, const std::vector<Vertex> &graph_)
{
  HalfEdge *aux = graph_.at(index_from).adjacency_list;
  while (aux != NULL)
  {
    if (aux->vertex_index == index_to)
      return true;
    aux = aux->next_edge;
  }
  return false;
}

// The AddHalfEdge function adds a half-edge between two nodes in the graph.
// It creates a new HalfEdge object and adds it to the adjacency list of the source node.
void AddHalfEdge(int32_t index_from, int32_t index_to, uint16_t weight, std::vector<Vertex> &graph_)
{
  HalfEdge *e = new HalfEdge;
  e->weight = weight;
  e->vertex_index = index_to;
  e->next_edge = graph_.at(index_from).adjacency_list;
  graph_.at(index_from).adjacency_list = e;
}

// The RemoveHalfEdge function removes a half-edge between two nodes in the graph.
// It searches for the specified edge and removes it from the adjacency list of the source node.
void RemoveHalfEdge(int32_t index_from, int32_t index_to, std::vector<Vertex> &graph_)
{
  /* TODO */
  for (HalfEdge *edges = graph_.at(index_from).adjacency_list; edges != nullptr; edges = edges->next_edge)
  {
    if (edges->vertex_index == index_to)
    {
      HalfEdge *temp = edges;
      edges = edges->next_edge;
      graph_.at(index_from).adjacency_list = edges;
      delete temp;
      return;
    }
    if (edges->next_edge->vertex_index == index_to)
    {
      HalfEdge *temp = edges->next_edge;
      edges->next_edge = edges->next_edge->next_edge;
      delete temp;
      return;
    }
  }
}

/*******************************************************************************************************/
// Graph
/*******************************************************************************************************/

bool graph::AddVertex(Tile t)
{
  if (GetNode(t) >= 0)
    return false;
  Vertex n = Vertex(t, nullptr, false);
  graph_.push_back(n);
  return true;
}

bool graph::AddEdge(Tile from, Tile to, uint16_t weight)
{
  if (from == to)
    return false;
  int32_t index_from;
  int32_t index_to;
  if (!IsVertexIn(from, index_from, graph_) || !IsVertexIn(to, index_to, graph_))
    return false;
  if (AuxAreAdjacent(index_from, index_to, graph_))
    return false;
  AddHalfEdge(index_from, index_to, weight, graph_);
  AddHalfEdge(index_to, index_from, weight, graph_);
  return true;
}

bool graph::RemoveEdge(Tile from, Tile to)
{
  if (from == to)
    return false;
  int32_t index_from;
  int32_t index_to;
  if (!IsVertexIn(from, index_from, graph_) || !IsVertexIn(to, index_to, graph_))
    return false;
  if (!AuxAreAdjacent(index_from, index_to, graph_))
    return false;
  RemoveHalfEdge(index_from, index_to, graph_);
  RemoveHalfEdge(index_to, index_from, graph_);
  return true;
}

int graph::NumVertices()
{
  return graph_.size();
}

int graph::NumEdges()
{
  int tot = 0;
  for (int32_t i = 0; i < graph_.size(); i++)
  {
    NodeDegree(graph_.at(i).tile, tot);
  }
  return (tot / 2);
}

bool graph::NodeDegree(Tile t, int &degree)
{
  if (GetNode(t) < 0)
    return false;
  Vertex vertex = graph_.at(GetNode(t));
  for (HalfEdge *edges = vertex.adjacency_list; edges != nullptr; edges = edges->next_edge)
  {
    ++degree;
  }
  return true;
}

bool graph::AreAdjacent(Tile v1, Tile v2)
{
  int32_t index_from = -1;
  int32_t index_to = -1;
  if (!IsVertexIn(v1, index_from, graph_) || !IsVertexIn(v2, index_to, graph_))
    return false;
  return AuxAreAdjacent(index_from, index_to, graph_);
}

std::vector<Tile> graph::GetAdjacencyList(Tile v1)
{
  std::vector<Tile> tile_vect;
  if (GetNode(v1) != -1)
  {
    Vertex aux = graph_.at(GetNode(v1));
    HalfEdge *edges = aux.adjacency_list;
    while (edges != nullptr)
    {
      tile_vect.push_back(graph_.at(edges->vertex_index).tile);
      edges = edges->next_edge;
    }
  }
  return tile_vect;
}

// The FindPathAux function is a helper function used by FindPath to find a path between two vertices in the graph.
// It performs a depth-first search (DFS) starting from the "from" vertex and backtracks to find the path to the "to" vertex.
bool FindPathAux(int32_t here, int32_t to, std::vector<Tile> &path, int &len, std::vector<Vertex> &graph_)
{
  graph_.at(here).visited = true;
  for (HalfEdge *ee = graph_.at(here).adjacency_list; ee != nullptr; ee = ee->next_edge)
  {
    if (graph_.at(ee->vertex_index).visited)
      continue;
    if (graph_.at(ee->vertex_index).tile == graph_.at(to).tile)
    {
      path.insert(path.begin(), graph_.at(ee->vertex_index).tile);
      len += ee->weight;
      return true;
    }
    bool res = FindPathAux(ee->vertex_index, to, path, len, graph_);
    if (res)
    {
      path.insert(path.begin(), graph_.at(ee->vertex_index).tile);
      len += ee->weight;
      return true;
    }
  }
  return false;
}

void graph::FindPathDFS(Tile v1, Tile v2, std::vector<Tile> &path, int &len)
{
  int32_t from = GetNode(v1), to = GetNode(v2);
  if (from == to || from == -1 || to == -1)
    return;
  for (int32_t i = 0; i < graph_.size(); i++)
    graph_.at(i).visited = false;
  len = 0;
  FindPathAux(from, to, path, len, graph_);
  return;
}

void graph::PrintGraph()
{
  for (int32_t i = 0; i < graph_.size(); i++)
  {
    std::cout << "(" << graph_.at(i).tile << ") |->| ";
    HalfEdge *adjnode = graph_.at(i).adjacency_list;
    while (adjnode != nullptr)
    {
      std::cout << "(" << graph_.at(adjnode->vertex_index).tile << ")" << " <- Weight: " << adjnode->weight;
      if (adjnode->next_edge == nullptr)
        break;
      std::cout << " || ";
      adjnode = adjnode->next_edge;
    }
    std::cout << std::endl;
  }
}

void graph::PrintMaze()
{
  std::vector<Tile> ordered_nodes;
  ordered_nodes.reserve(1000);
  for (int32_t i = 0; i < graph_.size(); i++)
  {
    int32_t temp_index = 0;
    for (int32_t j = 0; j < ordered_nodes.size(); j++)
    {
      if (graph_.at(i).tile < ordered_nodes.at(j))
      {
        break;
      }
      temp_index++;
    }
    ordered_nodes.insert(ordered_nodes.begin() + temp_index, graph_.at(i).tile);
  }

  int32_t min_z = ordered_nodes.at(0).z;
  int32_t max_z = ordered_nodes.at(ordered_nodes.size()-1).z;

  for (int8_t z = min_z; z <= max_z; z++)
  {
    std::cout << "Floor: " << (int)z << "\n";
    int32_t max_x = ordered_nodes.at(0).x;
    int32_t min_x = ordered_nodes.at(0).x;
    int32_t max_y = ordered_nodes.at(0).y;
    int32_t min_y = ordered_nodes.at(0).y;
    for (int i = 0; i < ordered_nodes.size(); i++)
    {
      if (z == ordered_nodes.at(i).z)
        max_x = ordered_nodes.at(i).x;
        min_x = ordered_nodes.at(i).x;
        max_y = ordered_nodes.at(i).y;
        min_y = ordered_nodes.at(i).y;
        break;
    }
    for (int i = 0; i < ordered_nodes.size(); i++)
    {
      if (max_x < ordered_nodes.at(i).x && z == ordered_nodes.at(i).z)
        max_x = ordered_nodes.at(i).x;
      if (min_x > ordered_nodes.at(i).x && z == ordered_nodes.at(i).z)
        min_x = ordered_nodes.at(i).x;
      if (max_y < ordered_nodes.at(i).y && z == ordered_nodes.at(i).z)
        max_y = ordered_nodes.at(i).y;
      if (min_y > ordered_nodes.at(i).y && z == ordered_nodes.at(i).z)
        min_y = ordered_nodes.at(i).y;
    }

    for (int8_t y = min_y; y <= max_y; y++)
    {
      for (int8_t i = 0; i < 3; i++)
      {
        for (int8_t x = min_x; x <= max_x; x++)
        {
          if (i == 0)
          {
            if (GetNode({y, x, z}) == -1)
            {
              if (GetNode({y-1, x, z}) != -1)
              {
                std::cout << "+---";
              }
              else
              {
                if (GetNode({y, x-1, z}) != -1)
                {
                  std::cout << "+   ";
                }
                else
                {
                  std::cout << "    ";
                }
              }
            }
            else
            {
              if (AreAdjacent({y, x, z}, {y-1, x, z}))
              {
                if (AreAdjacent({y, x, z}, {y, x-1, z}) && AreAdjacent({y, x-1, z}, {y-1, x-1, z}) && AreAdjacent({y-1, x-1, z}, {y-1, x, z}))
                {
                  std::cout << " ";
                  std::cout << "   ";
                }
                else
                {
                  std::cout << "+";
                  std::cout << "   ";
                }
              }
              else
              {
                std::cout << "+";
                std::cout << "---";
              }
            }
          }
          else if (i == 1)
          {
            if (GetNode({y, x, z}) == -1)
            {
              if (GetNode({y, x-1, z}) != -1)
              {
                std::cout << "|   ";
              }
              else
              {
                std::cout << "    ";
              }
            }
            else
            {
              if (AreAdjacent({y, x, z}, {y, x-1, z}))
              {
                bool found = false;
                std::vector<Tile> temp_vec = GetAdjacencyList({y,x,z});
                for (int8_t i = 0; i < temp_vec.size(); i++)
                {
                  if (temp_vec.at(i).z > z)
                  {
                    std::cout << "  U ";
                    found = true;
                  }
                  else if (temp_vec.at(i).z < z)
                  {
                    std::cout << "  D ";
                    found = true;
                  }
                }
                if (!found){
                  std::cout << "    ";
                }
              }
              else
              {
                std::vector<Tile> temp_vec = GetAdjacencyList({y,x,z});
                bool found = false;
                for (int8_t i = 0; i < temp_vec.size(); i++)
                {
                  if (temp_vec.at(i).z > z)
                  {
                    std::cout << "| U ";
                    found = true;
                  }
                  else if (temp_vec.at(i).z < z)
                  {
                    std::cout << "| D ";
                    found = true;
                  }
                }
                if (!found){
                  std::cout << "|   ";
                }
              }
            }
          }
        }
        if (i == 0)
        {
          if (GetNode({y, max_x, z}) != -1 || GetNode({y-1, max_x, z}) != -1) 
            std::cout << "+\n";
          else
            std::cout << "\n";
        }
        else if (i == 1)
        {
          if (GetNode({y, max_x, z}) != -1)
            std::cout << "|\n";
          else
            std::cout << "\n";
        }
      }
    }
    for (int8_t x = min_x; x <= max_x; x++)
    {
      if (GetNode({max_y, x, z}) == -1)
      {
        if (GetNode({max_y, x-1, z}) == -1)
        {
          std::cout << "    ";
        }
        else
        {
          std::cout << "+   ";
        }
      }
      else
      {
        std::cout << "+---";
      }
    }
    if (GetNode({max_y, max_x, z}) != -1) 
      std::cout << "+\n";
    else
      std::cout << "\n";
  }
  std::cout << std::endl;
}
