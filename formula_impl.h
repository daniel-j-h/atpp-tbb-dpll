#ifndef formula_impl_h
#define formula_impl_h

#include "formula.h"
#include "clause.h"
#include "clause_impl.h"
#include <vector>

namespace Sat {

class FormulaImpl : public Formula {

public:
  // Add a new Clause to the Formula
  // Return a reference to the new clause
  Clause &addClause() override;

  // Get the number of clauses
  int numClauses() const override;

  // Get the i^th clause in the formula
  Clause &getClause(int i) override;

  bool hasLit(int a) const;

  std::vector<ClauseImpl> &getClauses()
  {
    return clauses;
  };

  void setClauses(std::vector<ClauseImpl> newClauses)
  {
    clauses = newClauses;
  };

  std::vector<ClauseImpl> clauses;

private:
  //	map<int,ClauseImpl, hash, equal> clauses;
};
}

#endif
