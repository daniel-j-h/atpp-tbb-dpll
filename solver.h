#ifndef solver_h
#define solver_h

#include <vector>
#include <map>
#include "formula_impl.h"

namespace Sat {

class Solver {

public:
  std::map<int, bool> assgn;
  bool solve(FormulaImpl &f);
  bool splitting(FormulaImpl &c);
  FormulaImpl &unitProp(FormulaImpl &f, int uni);
  FormulaImpl &pureLit(FormulaImpl &f, std::vector<int> p);
  int getUnitProp(std::vector<ClauseImpl> &c) const;
  std::vector<int> getPure(FormulaImpl &f);
};
}

#endif
