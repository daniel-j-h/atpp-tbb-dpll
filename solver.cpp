#include <cstdlib>
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

  // condition 1
  if (c.getClauses().size() == 0)
    return true;

  // condition 2
  for (int i = 0; i < c.getClauses().size(); ++i)
    if (c.getClause(i).numLits() == 0)
      return false;

  int uni = getUnitProp(c.getClauses());

  // condition 3
  if (uni != 0) {
    assgn.emplace(uni, true);

    return splitting(unitProp(c, uni));
  }

  // condition 4
  else {
    std::vector<int> p = getPure(c);

    if (p.size() != 0) {
      for (auto &elem : p)
        assgn.emplace(elem, true);

      return splitting(pureLit(c, p));
    }

    // condition 5
    else {
      int firstLit = c.getClause(0).getLit(0);
      assgn.emplace(firstLit, true);

      if (splitting(unitProp(c, firstLit)) == true)
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

    std::vector<ClauseImpl>::iterator it;

    for (it = f.clauses.begin(); it != f.clauses.end();) {
      bool flag = false;
      std::vector<int>::iterator jt;

      for (jt = it->lits.begin(); jt != it->lits.end();) {

        if ((*jt) == -uni) {
          jt = it->lits.erase(jt);
          break;
        } else if ((*jt) == uni) {
          flag = true;
          break;
        } else {
          ++jt;
        }
      }
      if (flag == false)
        ++it;
      else {
        // it->clearLits();
        it = f.clauses.erase(it);
      }
    }

    uni = getUnitProp(f.getClauses());
  }
  // f.setClauses (f.clauses);
  return f;
}

int Solver::getUnitProp(std::vector<ClauseImpl> &c) const
{
  for (auto &elem : c)
    if (elem.get_lits().size() == 1)
      return elem.getLit(0);
  return 0;
}

/*
if it creates a clause that contains both a variable and its negation, filter the clause out
*/
FormulaImpl &Solver::pureLit(FormulaImpl &f, std::vector<int> p)
{
  std::vector<ClauseImpl>::iterator it;

  for (auto &elem : p) {
    for (it = f.clauses.begin(); it != f.clauses.end();) {
      if (it->hasLit(elem)) {
        assgn.emplace(elem, true);
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
  std::map<int, int> litTable;
  std::vector<int> p;

  std::vector<ClauseImpl>::iterator it;

  for (it = f.clauses.begin(); it != f.clauses.end(); ++it) {
    std::vector<int>::iterator jt;
    for (jt = it->lits.begin(); jt != it->lits.end(); ++jt) {
      if (litTable.find(std::abs(*jt)) == litTable.end()) {
        if (*jt < 0)
          litTable.emplace(std::abs(*jt), -1);
        else
          litTable.emplace(std::abs(*jt), 1);
      } else {
        if (litTable.find(std::abs(*jt))->second == -1 && *jt > 0) {
          litTable.erase(*jt);
          litTable.emplace(std::abs(*jt), 0);
        } else if (litTable.find(std::abs(*jt))->second == 1 && *jt < 0) {
          litTable.erase(std::abs(*jt));
          litTable.emplace(std::abs(*jt), 0);
        }
      }
    }
  }

  std::map<int, int>::iterator kt;
  int lit;

  for (kt = litTable.begin(); kt != litTable.end(); ++kt) {
    if (kt->second != 0) {
      lit = (kt->first) * (kt->second);
      p.push_back(lit);
    }
  }
  return p;
}
