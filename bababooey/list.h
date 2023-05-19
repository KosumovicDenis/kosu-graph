#pragma once

#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <string>

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
  int16_t x, y;
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

namespace list
{

  /**************************** Tipi e costanti *********************************************/
  typedef Tile Elem; // tipo base, compatibile con il tipo Label dell'grafo

  const Elem emptyElem = { 0 };
  const int BLOCKDIM = 10000;

  struct list;

  typedef struct
  {
    Elem *list;
    int size;
    int maxsize;
  } List;

  // typedef list * List;

  // ATTENZIONE: per la realizzazione del laboratorio 8 non vi serviranno tutte le seguenti funzioni ma solo un sottoinsieme
  void clear(List &);          /* "smantella" la lista */
  void set(int, Elem, List &); /* modifica l'elemento in posizione pos */
  void add(int, Elem, List &); /* inserisce l'elemento in posizione pos, shiftando a destra gli altri elementi */
  void addBack(Elem, List &);  /* inserisce l'elemento alla fine della lista */
  void addFront(Elem, List &); /* inserisce l'elemento all'inizio della lista */
  void removePos(int, List &); /* cancella l'elemento in posizione pos dalla lista  */
  Elem get(int, const List &); /* restituisce l'elemento in posizione pos */
  List createEmpty();          /* crea la lista vuota e la restituisce */
  bool isEmpty(const List &);
  int size(const List &);
}

void printList(const list::List &);