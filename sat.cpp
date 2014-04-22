#include <cstdlib>
#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "formula_impl.h"
#include "solver.h"

using namespace Sat;

int main(int argc, char **argv)
{
  std::vector<std::string> args{argv, argv + argc};

  if (args.size() != 2) {
    std::cout << "Expected filename" << std::endl;
    return EXIT_FAILURE;
  }

  std::ifstream is{args[1]};

  if (!is) {
    std::cout << "File not found" << std::endl;
    return EXIT_FAILURE;
  }

  FormulaImpl f;
  f.readClauses(is);

  assert(f.numClauses() > 0);

  f.dumpClauses();

  Solver sat;

  if (sat.solve(f))
    std::cout << "Satisfiable" << std::endl;
  else
    std::cout << "Unsatisfiable" << std::endl;
}
