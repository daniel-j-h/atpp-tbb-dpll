#include <cstdlib>
#include <vector>
#include <algorithm>
#include <iterator>
#include "clause.h"
#include "clause_impl.h"

using namespace Sat;

// size_t ClauseImpl::clause_num = 0;

// Add a literal to the clause
void ClauseImpl::addLiteral(int lit)
{
  add_lits(lit);
}

// Get the largest variable in the clause
int ClauseImpl::maxVar() const
{
  using item_t = decltype(lits)::value_type;
  auto abs_maximizer = [](const item_t &lhs, const item_t &rhs) { return std::abs(lhs) < std::abs(rhs); };
  return *std::max_element(std::begin(lits), std::end(lits), abs_maximizer);
}

// Get the number of variables in the clause
int ClauseImpl::numLits() const
{
  return lits.size();
}

// Get the i^th literal in the clause
int ClauseImpl::getLit(int i) const
{
  return lits.at(i);
}

std::vector<int> ClauseImpl::get_lits() const
{
  return lits;
}

void ClauseImpl::add_lits(int lit)
{
  lits.emplace_back(lit);
}

bool ClauseImpl::hasLit(int a) const
{
  return std::find(std::begin(lits), std::end(lits), a) != std::end(lits);
}
