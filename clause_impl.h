#ifndef clause_impl_h
#define clause_impl_h

#include "clause.h"
#include <vector>

namespace Sat {

class ClauseImpl : public Clause {

public:
  // Add a literal to the clause
  void addLiteral(int lit) override;

  // Get the largest variable in the clause
  int maxVar() const override;

  // Get the number of variables in the clause
  int numLits() const override;

  // Get the i^th literal in the clause
  int getLit(int i) const override;

  // static size_t numOfClauses(){return clause_num;}

  std::vector<int> get_lits() const;

  void add_lits(int lit);

  std::vector<int> lits;

  bool hasLit(int i) const;


private:
  // Data structure for saving one clause
};
}

#endif
