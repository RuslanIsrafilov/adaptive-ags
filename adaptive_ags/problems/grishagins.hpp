#ifndef ADAPRIVE_AGS_PROBLEMS_GRISHAGINS_PROBLEM_HPP_
#define ADAPRIVE_AGS_PROBLEMS_GRISHAGINS_PROBLEM_HPP_

#include "adaptive_ags/problem.hpp"
#include "adaptive_ags/problems/grishagins_impl.hpp"

namespace adaptive_ags {
namespace problems {

class GrishaginsProblem : public Problem {
public:
  explicit GrishaginsProblem(int number = 1)
      : impl_(number),
        ground_truth_minimizer_(impl_.ground_truth_minima(),
                                impl_.ground_truth_minimizer()) { }

  const BoundingBox &bbox() const override {
    return impl_.bbox();
  }

  const Minimizer &ground_truth() const override {
    return ground_truth_minimizer_;
  }

  scalar_t compute(const vector_t &x) const override {
    return impl_.compute(x);
  }

private:
  /* Grishagin's function is implemented only for the double type.
   * Implementation is wrapped with specialization of GrishaginsProblemImpl
   * template for the double type. By default, scalar_t is double, if it
   * changes to float, it will be a compilation error. */
  GrishaginsProblemImpl<scalar_t> impl_;
  Minimizer ground_truth_minimizer_;
};

} // namespace problems
} // namespace adaptive_ags

#endif
