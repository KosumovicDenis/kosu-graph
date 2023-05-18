#include "graph.h"

using namespace graph;

struct halfEdgeVertex
{
  halfEdgeVertex *NextEdge;
  vertexNode *VertexPtr;
  Weight w;
};

struct graph::vertexNode
{
  Tile tile;
  // string info;
  vertexNode *NextVertex;
  halfEdgeVertex *adjList;
  bool visited;
};

//***********************************************************AUSILIARIE********************************************
bool isEmptyEdge(halfEdgeVertex *h)
{
  return (h == NULL);
}

Graph getNode(Tile &t, const Graph g)
{
  Graph aux = g;
  while (!isEmpty(aux))
  {
    if (aux->tile == t)
      return aux;
    aux = aux->NextVertex;
  }
  return emptyGraph;
}

bool isVertexIn(Tile t, vertexNode *&ptr, const Graph &g)
{
  Graph aux = g;
  while (!isEmpty(aux))
  {
    if (aux->tile == t)
    {
      ptr = aux;
      return true;
    }
    aux = aux->NextVertex;
  }
  return false;
}

bool aux_areAdjacent(vertexNode *v1, vertexNode *v2)
{
  halfEdgeVertex *aux = v1->adjList;
  while (!isEmptyEdge(aux))
  {
    if (aux->VertexPtr == v2)
      return true;
    aux = aux->NextEdge;
  }
  return false;
}

void addHalfEdge(vertexNode *from, vertexNode *to, Weight w, const Graph g)
{
  halfEdgeVertex *e = new halfEdgeVertex;
  e->w = w;
  e->VertexPtr = to;
  e->NextEdge = from->adjList;
  from->adjList = e;
}

/*******************************************************************************************************/
// Grafo
/*******************************************************************************************************/

// Restituisce il grafo vuoto
Graph graph::createEmptyGraph()
{
  return emptyGraph;
}

// Aggiunge nuovo vertice con etichetta la stringa. Fallisce se gia' presente
bool graph::addVertex(Tile t, Graph &g)
{
  if (getNode(t, g))
    return false;
  vertexNode *n = new vertexNode;
  halfEdgeVertex *edge = new halfEdgeVertex;
  edge = NULL;
  n->tile = t;
  n->adjList = edge;
  n->visited = false;
  if (isEmpty(g))
  {
    n->NextVertex = emptyGraph;
    g = n;
  }
  else
  {
    n->NextVertex = g;
    g = n;
  }
  return true;
}

// TODO: removeVertex

// Aggiunge un arco di peso "w" tra i nodi con etichetta "f" e "t". Fallisce se esiste gia' l'arco
// se i nodi non esistono nel grafo e se si tenta di inserire un arco tra un nodo ed esso stesso
bool graph::addEdge(Tile from, Tile to, Weight w, Graph &g)
{
  if (from == to)
    return false;
  vertexNode *ptr_from;
  vertexNode *ptr_to;
  if (!isVertexIn(from, ptr_from, g) || !isVertexIn(to, ptr_to, g))
    return false;
  if (aux_areAdjacent(ptr_from, ptr_to))
    return false;
  addHalfEdge(ptr_from, ptr_to, w, g);
  addHalfEdge(ptr_to, ptr_from, w, g);
  return true;
}

// Restituisce true se il grafo e' vuoto, false altrimenti
bool graph::isEmpty(const Graph &g)
{
  return (g == emptyGraph);
}

// Ritorna il numero di vertici del grafo
int graph::numVertices(const Graph &g)
{
  if (isEmpty(g))
    return 0;
  return numVertices(g->NextVertex) + 1;
}

// Ritorna il numero di archi del grafo
int graph::numEdges(const Graph &g)
{
  int tot = 0;
  vertexNode *temp = g;
  while (!isEmpty(temp))
  {
    nodeDegree(temp->tile, tot, g);
    temp = temp->NextVertex;
  }
  return (tot / 2);
}

// Calcola e ritorna (nel secondo parametro) il grado del nodo. Fallisce
// se il nodo non esiste
bool graph::nodeDegree(Tile t, int &degree, const Graph &g)
{
  vertexNode *Vertex = getNode(t, g);
  if (isEmpty(Vertex))
    return false;
  for (halfEdgeVertex *edges = Vertex->adjList; !isEmptyEdge(edges); edges = edges->NextEdge)
  {
    ++degree;
  }
  return true;
}

// Verifica se i due vertici v1 e v2 sono adiacenti (ovvero se esiste un arco)
bool graph::areAdjacent(Tile v1, Tile v2, const Graph &g)
{
  vertexNode *ptr1;
  vertexNode *ptr2;
  if (!isVertexIn(v1, ptr1, g) || !isVertexIn(v2, ptr2, g))
    return false;
  return aux_areAdjacent(ptr1, ptr2);
}

// Restituisce la lista di adiacenza di un vertice
list::List graph::adjacentList(Tile v1, const Graph &g)
{
  list::List lst = list::createEmpty();
  vertexNode *aux = getNode(v1, g);
  halfEdgeVertex *edges = aux->adjList;
  while (!isEmptyEdge(edges))
  {
    addBack(edges->VertexPtr->tile, lst);
    edges = edges->NextEdge;
  }
  return lst;
}

// Ritorna un cammino tra una citta' ed un altra
// Il cammino da "v1" a "v2" alla fine sara' in "path"
// e la lunghezza sara' in "len".
// Si assume che il chiamante fornisca inizialmente un cammino vuoto.
//
// La funzione rappresenta una variante della visita DFS

bool findPathAux(vertexNode *here, vertexNode *to, list::List &path, int &len)
{
  here->visited = true;
  for (halfEdgeVertex *ee = here->adjList; !isEmptyEdge(ee); ee = ee->NextEdge)
  {
    if (ee->VertexPtr->visited)
      continue;
    if (ee->VertexPtr == to)
    {
      addFront(ee->VertexPtr->tile, path);
      len += ee->w;
      return true;
    }
    bool res = findPathAux(ee->VertexPtr, to, path, len);
    if (res)
    {
      addFront(ee->VertexPtr->tile, path);
      len += ee->w;
      return true;
    }
  }
  return false;
}

void graph::findPath(Tile v1, Tile v2, list::List &path, int &len, const Graph &g)
{
  vertexNode *from = getNode(v1, g), *to = getNode(v2, g);
  if (from == to || from == 0 || to == 0)
    return;
  for (vertexNode *n = g; !isEmpty(n); n = n->NextVertex)
    n->visited = false;
  len = 0;
  findPathAux(from, to, path, len);
  return;
}

/*******************************************************************************************************/
// Stampa il grafo
void printGraph(const graph::Graph &g)
{
  Graph aux = g;
  while (!isEmpty(aux))
  {
    cout << aux->tile << " :: ";
    halfEdgeVertex *adjnode = aux->adjList;
    while (!isEmptyEdge(adjnode))
    {
      cout << adjnode->VertexPtr->tile << " " << adjnode->w;
      if (isEmptyEdge(adjnode->NextEdge))
        break;
      cout << " - ";
      adjnode = adjnode->NextEdge;
    }
    aux = aux->NextVertex;
    cout << endl;
  }
}