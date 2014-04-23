#include <cstdlib>
#include <algorithm>
#include <functional>
#include <iterator>
#include "clause.h"
#include "solver.h"

using namespace Sat;

bool Solver::solve(FormulaImpl &f)
{
  // vector<int> p = findPure(f);
  // pureLit(f,p);
  // int uni = getUnitProp(f.getClauses());
  // unitProp(f, uni);
  // return true;
  return splitting(f);
}

/*
1. If the set of clauses is empty, it returns true
2. Otherwise, if the set of clauses contains an empty clause, it returns false
3. Otherwise, if there is a unit clause, it applies unit propagation and then calls itself recursively
4. Otherwise, if there is a pure literal, it applies the pure literal rule and then calls itself recursively
5. Otherwise, it applies either the resolution rule or the splitting rule and calls itself recursively
*/
bool Solver::splitting(FormulaImpl &f)
{
  FormulaImpl c(f);
  auto clauses = c.getClauses();

  // condition 1
  if (clauses.empty())
    return true;

  // condition 2
  auto empty = [](const Clause &clause) { return clause.numLits() == 0; };
  auto contains_empty = std::find_if(std::begin(clauses), std::end(clauses), empty) != std::end(clauses);

  if (contains_empty)
    return false;

  int uni = getUnitProp(clauses);

  // condition 3
  if (uni != 0) {
    assgn.emplace(uni, true);

    return splitting(unitProp(c, uni));
  }

  // condition 4
  else {
    std::vector<int> p = getPure(c);

    if (!p.empty()) {
      for (const auto &lit : p)
        assgn.emplace(lit, true);

      return splitting(pureLit(c, p));
    }

    // condition 5
    else {
      int firstLit = c.getClause(0).getLit(0);
      assgn.emplace(firstLit, true);

      if (splitting(unitProp(c, firstLit)))
        return true;
      else {
        assgn.erase(firstLit);
        assgn.emplace(firstLit, false);

        return splitting(unitProp(f, -firstLit));
      }
    }
  }
}

/*
a literal and a list of clauses;
returns a new list of clauses resulting from applying the pure literal rule with the given literal.
*/
FormulaImpl &Solver::unitProp(FormulaImpl &f, int uni)
{
  std::vector<int> lits;

  while (uni != 0) {
    assgn.emplace(uni, true);

    for (auto it = std::begin(f.clauses), end = std::end(f.clauses); it != end;) {
      bool flag = false;

      for (auto jt = std::begin(it->lits), jt_end = std::end(it->lits); jt != jt_end; ++jt) {
        if (*jt == -uni) {
          it->lits.erase(jt);
          break;
        } else if (*jt == uni) {
          flag = true;
          break;
        }
      }

      if (!flag)
        ++it;
      else
        it = f.clauses.erase(it);
    }

    uni = getUnitProp(f.getClauses());
  }

  // f.setClauses (f.clauses);
  return f;
}

int Solver::getUnitProp(std::vector<ClauseImpl> &c) const
{
  auto one_lit = [](const ClauseImpl &clause) { return clause.get_lits().size() == 1; };
  auto lit = std::find_if(std::begin(c), std::end(c), one_lit);

  if (lit != std::end(c))
    return lit->getLit(0);

  return 0;
}

/*
if it creates a clause that contains both a variable and its negation, filter the clause out
*/
FormulaImpl &Solver::pureLit(FormulaImpl &f, std::vector<int> p)
{
  for (const auto &lit : p) {
    for (auto it = std::begin(f.clauses), end = std::end(f.clauses); it != end;) {
      if (it->hasLit(lit)) {
        assgn.emplace(lit, true);
        it = f.clauses.erase(it);
      } else {
        ++it;
      }
    }
  }

  return f;
}

std::vector<int> Solver::getPure(FormulaImpl &f)
{
  std::unordered_map<int, int> litTable;
  std::vector<int> p;

  for (auto it = std::begin(f.clauses), end = std::end(f.clauses); it != end; ++it) {
    for (auto jt = std::begin(it->lits), jt_end = std::end(it->lits); jt != jt_end; ++jt) {
      auto jt_abs = std::abs(*jt);

      if (litTable.find(jt_abs) == std::end(litTable)) {
        litTable.emplace(jt_abs, *jt < 0 ? -1 : 1);
      } else {
        if (litTable.find(jt_abs)->second == -1 && *jt > 0) {
          litTable.erase(*jt);
          litTable.emplace(jt_abs, 0);
        } else if (litTable.find(jt_abs)->second == 1 && *jt < 0) {
          litTable.erase(jt_abs);
          litTable.emplace(jt_abs, 0);
        }
      }
    }
  }

  int lit;

  for (auto it = std::begin(litTable), end = std::end(litTable); it != end; ++it) {
    if (it->second != 0) {
      lit = it->first * it->second;
      p.emplace_back(lit);
    }
  }

  return p;
}
