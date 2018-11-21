#include <iostream>

#include "adaptive_ags/method.hpp"
#include "adaptive_ags/problems/grishagins.hpp"

std::ostream &operator << (std::ostream &stream, const adaptive_ags::vector_t &vector) {
  stream << "[ ";
  for (auto &x : vector) {
    stream << x << ' ';
  }
  return stream << ']';
}

int main(int argc, char const *argv[]) {
  /* All parameters of Adaptive AGS solver */
  auto params = adaptive_ags::Parameters()
    .max_iters(1000)
    .epsilon(0.01)
    .init_max_iters(10)
    .init_epsilon(0.05)
    .reliability(4.0)
    .enable_updates(false);

  /* Minimize pre-made function, see for example
   * https://www.sciencedirect.com/science/article/pii/S1877050915010248 */
  const int grishagins_func_number = 1;
  auto problem = adaptive_ags::problems
    ::GrishaginsProblem(grishagins_func_number);

  /* Minimize pre-made function */
  auto result = adaptive_ags::Method().run(problem, params);

  std::cout << "Grishagin's function:" << std::endl
            << "f_min = " << result.minimizer().value() << std::endl
            << "x_min = " << result.minimizer().argument() << std::endl
            << "trials = " << result.statistics().trials_number() << std::endl;

  return 0;
}


