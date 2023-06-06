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

std::vector<std::pair <Tile, uint16_t>> graph::GetWeightedAdjacencyList(Tile v1)
{
  std::vector<std::pair <Tile, uint16_t>> tile_vect;
  if (GetNode(v1) != -1)
  {
    Vertex aux = graph_.at(GetNode(v1));
    HalfEdge *edges = aux.adjacency_list;
    while (edges != nullptr)
    {
      tile_vect.push_back(std::pair(graph_.at(edges->vertex_index).tile, edges->weight));
      edges = edges->next_edge;
    }
  }
  return tile_vect;

}

void graph::PrintGraph()
{
  for (int32_t i = 0; i < graph_.size(); i++)
  {
    std::cout << "(" << graph_.at(i).tile << ") |->| ";
    HalfEdge *adjnode = graph_.at(i).adjacency_list;
    while (adjnode != nullptr)
    {
      std::cout << "(" << graph_.at(adjnode->vertex_index).tile << ")"
                << " <- Weight: " << adjnode->weight;
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
  int32_t max_z = ordered_nodes.at(ordered_nodes.size() - 1).z;

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
              if (GetNode({y - 1, x, z}) != -1)
              {
                std::cout << "+---";
              }
              else
              {
                if (GetNode({y, x - 1, z}) != -1)
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
              if (AreAdjacent({y, x, z}, {y - 1, x, z}))
              {
                if (AreAdjacent({y, x, z}, {y, x - 1, z}) && AreAdjacent({y, x - 1, z}, {y - 1, x - 1, z}) && AreAdjacent({y - 1, x - 1, z}, {y - 1, x, z}))
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
              if (GetNode({y, x - 1, z}) != -1)
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
              if (AreAdjacent({y, x, z}, {y, x - 1, z}))
              {
                bool found = false;
                std::vector<Tile> temp_vec = GetAdjacencyList({y, x, z});
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
                if (!found)
                {
                  std::cout << "    ";
                }
              }
              else
              {
                std::vector<Tile> temp_vec = GetAdjacencyList({y, x, z});
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
                if (!found)
                {
                  std::cout << "|   ";
                }
              }
            }
          }
        }
        if (i == 0)
        {
          if (GetNode({y, max_x, z}) != -1 || GetNode({y - 1, max_x, z}) != -1)
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
        if (GetNode({max_y, x - 1, z}) == -1)
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

//--------------------

struct TileDistDirection
{
  Tile tile;
  double distance;
  int direction;
};


// Heuristic function for A* algorithm
int32_t Heuristic(const Tile &current, const Tile &goal)
{
  // Calcolo della distanza di Euclidea come euristica
  return sqrt(pow((current.x - goal.x), 2) + pow((current.y - goal.y), 2));
}

struct TileHasher
{
  std::size_t operator()(const Tile &tile) const
  {
    std::size_t h1 = std::hash<int32_t>{}(tile.y);
    std::size_t h2 = std::hash<int32_t>{}(tile.x);
    std::size_t h3 = std::hash<int32_t>{}(tile.z);
    return h1 ^ (h2 << 1) ^ (h3 << 2);
  }
};

struct CompareDist
{
  bool operator()(const TileDistDirection &a, const TileDistDirection &b)
  {
    return a.distance > b.distance; // Ordine crescente in base alla distanza
  }
};

struct DistanceCalculator
{
  double operator()(const Tile &node1, const Tile &node2) const
  {
    return sqrt(pow((node1.x - node2.x), 2) + pow((node1.y - node2.y), 2));
  }
};

struct Distance {
  double operator()(const TileDistDirection& node1, const Tile& node2, int &new_direction) const {
    int dx = node1.tile.x - node2.x;
    int dy = node1.tile.y - node2.y;
    int dz = node1.tile.z - node2.z;
    int turn_weight = 0;
    if (node1.direction == 0 || node1.direction == 2)
    {
      if (dx != 0)
      {
        turn_weight += 2;
        if (dx > 0)
        {
          new_direction = 3;
        }
        else
        {
          new_direction = 1;
        }
      }
      else
      {
        if (node1.direction == 0)
        {
          if (dy > 0)
          {
            turn_weight += 4;
            new_direction = 2;
          }
        }
        else
        {
          if (dy < 0)
          {
            turn_weight += 4;
            new_direction = 0;
          }
        }
      }
    }
    else
    {
      if (dy != 0)
      {
        turn_weight += 2;
        if (node1.tile.y - node2.y > 0)
        {
          new_direction = 2;
        }
        else
        {
          new_direction = 0;
        }
      }
      else
      {
        if (node1.direction == 1)
        {
          if (dx > 0)
          {
            turn_weight += 4;
          }
        }
        else
        {
          if (dx < 0)
          {
            turn_weight += 4;
          }
        }
      }
    }

    /* Ramp
    if (dz != 0)
    {
      turn_weight += 5;
    }
    */
    return turn_weight;
  }
};


double potential(const Tile &node, const Tile &end)
{
  return sqrt(pow(node.x - end.x, 2) + pow(node.y - end.y, 2));
}

void graph::FindPathAStar(const Tile &start, const Tile &goal, std::vector<Tile> &path, int &len, int const direction)
{
  std::unordered_map<Tile, double, TileHasher> dist;
  std::priority_queue<TileDistDirection, std::vector<TileDistDirection>, CompareDist> open_nodes;
  std::unordered_set<Tile, TileHasher> closed_nodes;
  std::unordered_map<Tile, Tile, TileHasher> predecessor;
  Distance distance;
  // DistanceCalculator distance;

  open_nodes.push({start, 0.0, direction});
  dist[start] = 0.0;

  while (!open_nodes.empty())
  {
    TileDistDirection cur_node = open_nodes.top();
    open_nodes.pop();

    if (cur_node.tile == goal)
    {
      Tile current = cur_node.tile;
      while (!(current == start))
      {
        path.push_back(current);
        current = predecessor[current];
      }
      path.push_back(start);
      std::reverse(path.begin(), path.end());
      len = dist[cur_node.tile];
      return;
    }

    closed_nodes.insert(cur_node.tile);

    for (const auto &neighbor : GetWeightedAdjacencyList(cur_node.tile))
    {
      if (closed_nodes.count(neighbor.first) == 0)
      {
        int new_direction = cur_node.direction;
        double new_dist = dist[cur_node.tile] + distance(cur_node, neighbor.first, new_direction) + neighbor.second;
        if (!dist.count(neighbor.first) || new_dist < dist[neighbor.first])
        {
          dist[neighbor.first] = new_dist;
          predecessor[neighbor.first] = cur_node.tile;
          open_nodes.push({neighbor.first, new_dist, new_direction});
        }
      }
    }
  }

  len = -1;
}

//---------

void graph::PrintMazePath(std::vector<Tile> &path)
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
  int32_t max_z = ordered_nodes.at(ordered_nodes.size() - 1).z;

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
              if (GetNode({y - 1, x, z}) != -1)
              {
                std::cout << "+---";
              }
              else
              {
                if (GetNode({y, x - 1, z}) != -1)
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
              if (AreAdjacent({y, x, z}, {y - 1, x, z}))
              {
                if (AreAdjacent({y, x, z}, {y, x - 1, z}) && AreAdjacent({y, x - 1, z}, {y - 1, x - 1, z}) && AreAdjacent({y - 1, x - 1, z}, {y - 1, x, z}))
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
              if (GetNode({y, x - 1, z}) != -1)
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
              if (AreAdjacent({y, x, z}, {y, x - 1, z}))
              {
                bool found = false;
                std::vector<Tile> temp_vec = GetAdjacencyList({y, x, z});
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
                if (!found)
                {
                  bool in_path = false;
                  for (int8_t p = 0; p < path.size(); p++)
                  {
                    if (Tile{y, x, z} == path.at(p))
                    {
                      in_path = true;
                      break;
                    }
                  }
                  if (in_path)
                  {
                    if (Tile{y, x, z} == path.at(0))
                    {
                      std::cout << "  S ";
                    }
                    else if (Tile{y, x, z} == path.at(path.size()-1))
                    {
                      std::cout << "  E ";
                    }
                    else
                    {
                      std::cout << "  O ";
                    }
                  }
                  else
                  {
                    std::cout << "    ";
                  }
                }
              }
              else
              {
                std::vector<Tile> temp_vec = GetAdjacencyList({y, x, z});
                bool found = false;
                for (int8_t i = 0; i < temp_vec.size(); i++)
                {
                  if (temp_vec.at(i).z > z)
                  {
                    std::cout << "| U ";
                    found = true;
                    break;
                  }
                  else if (temp_vec.at(i).z < z)
                  {
                    std::cout << "| D ";
                    found = true;
                    break;
                  }
                }
                if (!found)
                {
                  bool in_path = false;
                  for (int8_t p = 0; p < path.size(); p++)
                  {
                    if (Tile{y, x, z} == path.at(p))
                    {
                      in_path = true;
                      break;
                    }
                  }
                  if (in_path)
                  {
                    if (Tile{y, x, z} == path.at(0))
                    {
                      std::cout << "| S ";
                    }
                    else if (Tile{y, x, z} == path.at(path.size()-1))
                    {
                      std::cout << "| E ";
                    }
                    else
                    {
                      std::cout << "| O ";
                    }
                  }
                  else
                  {
                    std::cout << "|   ";
                  }
                }
              }
            }
          }
        }
        if (i == 0)
        {
          if (GetNode({y, max_x, z}) != -1 || GetNode({y - 1, max_x, z}) != -1)
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
        if (GetNode({max_y, x - 1, z}) == -1)
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