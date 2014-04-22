#include <istream>
#include <algorithm>
#include "clause.h"
#include "formula.h"

using namespace Sat;

void Formula::readClauses(std::istream &is)
{
  while (!is.eof()) {
    Clause &c = addClause();
    c.readClause(is);

    maxVar = std::max(c.maxVar(), maxVar);
  }
}


void Formula::dumpClauses()
{
  for (int i = 0; i < numClauses(); ++i)
    getClause(i).dumpClause();
}
