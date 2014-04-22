#include <iostream>
#include <istream>
#include "clause.h"

using namespace Sat;

void Clause::readClause(std::istream &is)
{
  static int lit = 0;

  if (lit != 0)
    addLiteral(lit);

  while (!is.eof()) {
    is >> lit;

    if (lit == 0)
      break;

    addLiteral(lit);
  }

  is >> lit;
}

void Clause::dumpClause() const
{
  int i;

  std::cout << "(";

  for (i = 0; i < numLits() - 1; ++i)
    std::cout << getLit(i) << ", ";

  if (numLits() != 0)
    std::cout << getLit(i);

  std::cout << ")" << std::endl;
}
