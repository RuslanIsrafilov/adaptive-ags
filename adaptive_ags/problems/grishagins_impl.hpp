#ifndef ADAPRIVE_AGS_PROBLEMS_GRISHAGINS_PROBLEM_IMPL_HPP_
#define ADAPRIVE_AGS_PROBLEMS_GRISHAGINS_PROBLEM_IMPL_HPP_

#include "grishagins/grishagin_function.hpp"

namespace adaptive_ags {
namespace problems {

template<typename T>
class GrishaginsProblemImpl;

template<>
class GrishaginsProblemImpl<double> {
public:
  explicit GrishaginsProblemImpl(int number = 1)
      : bbox_(/* dimension   */ 2,
              /* left_bound  */ 0.0,
              /* right_bound */ 1.0),
        func_(number) { }

  double compute(const vector_t &x) const {
    _ADAPTIVE_AGS_ASSERT_( x.size() >= 2 );
    return func_.calculate(x[0], x[1]);
  }

  const BoundingBox &bbox() const {
    return bbox_;
  }

  double ground_truth_minima() const {
    return func_.minima();
  }

  vector_t ground_truth_minimizer() const {
    double x, y;
    func_.minimizer(x, y);
    return { x, y };
  }

private:
  BoundingBox bbox_;
  gf::grishagin_function func_;
};

} // namespace problems
} // namespace adaptive_ags

#endif
