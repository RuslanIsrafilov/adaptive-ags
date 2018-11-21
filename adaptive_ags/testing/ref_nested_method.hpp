#ifndef ADAPRIVE_AGS_TESTING_REF_NESTED_METHOD_HPP_
#define ADAPRIVE_AGS_TESTING_REF_NESTED_METHOD_HPP_

#include "adaptive_ags/result.hpp"
#include "adaptive_ags/problem.hpp"
#include "adaptive_ags/testing/ref_interval_set.hpp"

namespace adaptive_ags {
namespace testing {

class RefNestedParameters {
  _ADAPTIVE_AGS_REG_PARAM_( size_t,   level_iters, 25   )
  _ADAPTIVE_AGS_REG_PARAM_( scalar_t, epsilon,     0.01 )
  _ADAPTIVE_AGS_REG_PARAM_( scalar_t, reliability, 3.5  )
};

class RefNestedMethod {
public:
  RefNestedMethod()
      : args_(utils::empty_vec()),
        problem_(nullptr),
        params_(nullptr) { }

  Result run(const Problem &problem,
             const RefNestedParameters &parameters) {
    problem_ = &problem;
    params_ = &parameters;
    result_ = Result();
    args_ = vector_t(dimension());
    compute(0);
    return result_;
  }

private:
  scalar_t compute(dim_t lvl) {
    if (lvl == dimension()) {
      return compute_in_leaf();
    }

    const scalar_t a = left_bound(lvl);
    const scalar_t b = right_bound(lvl);
    _ADAPTIVE_AGS_ASSERT_( a < b );

    RefIntervalSet iset(reliability(), nested_iters());
    iset.push_first(a, compute(lvl, a),
                    b, compute(lvl, b));

    for (size_t i = 0; i < nested_iters(); i++) {
      const scalar_t x = iset.next();
      const scalar_t z = compute(lvl, x);
      const scalar_t e = iset.push(x, z);

      if (e < epsilon()) {
        break;
      }
    }

    return iset.min_z();
  }

  scalar_t compute(dim_t lvl, scalar_t x) {
    args_[lvl] = x;
    return compute(lvl + 1);
  }

  scalar_t compute_in_leaf() {
    const scalar_t z = problem_->compute(args_);
    result_.update(z, args_);
    return z;
  }

  dim_t dimension() const {
    return problem_->bbox().dimension();
  }

  scalar_t reliability() const {
    return params_->reliability_;
  }

  scalar_t nested_iters() const {
    return params_->level_iters_;
  }

  scalar_t epsilon() const {
    return params_->epsilon_;
  }

  scalar_t left_bound(dim_t lvl) const {
    _ADAPTIVE_AGS_ASSERT_( lvl < dimension() );
    return problem_->bbox().left()[lvl];
  }

  scalar_t right_bound(dim_t lvl) const {
    _ADAPTIVE_AGS_ASSERT_( lvl < dimension() );
    return problem_->bbox().right()[lvl];
  }

  Result result_;
  vector_t args_;
  const Problem *problem_;
  const RefNestedParameters *params_;
};

} // namespace testing
} // namespace adaptive_ags

#endif
