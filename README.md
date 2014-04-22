# atpp-tbb-dpll


Davis Putnam Logemann Loveland (DPLL) algorithm in C++.

Forked from https://github.com/Wenling/DPLL-Algorithm

Disclaimer: do not use it for serious real-world problems -- I warned you!


## Plan

1. Code quality: cleanup
2. Parallelize: using TBB


## Tooling

See Makefile: clang's -Weverything produces extensive diagnostics.

Format with:

    clang-format -style=file

Modernize with:

    clang-modernize -risk=risky -format -style=file -summary -final-syntax-check -include . *.cpp
