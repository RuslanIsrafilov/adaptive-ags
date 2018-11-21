#ifndef ADAPRIVE_AGS_TESTING_PROBLEMS_HPP_
#define ADAPRIVE_AGS_TESTING_PROBLEMS_HPP_

#include <list>
#include "adaptive_ags/testing/defines.hpp"
#include "adaptive_ags/problems/grishagins.hpp"

namespace adaptive_ags {
namespace testing {

class TestProblem : public Problem {
public:
  scalar_t compute(const vector_t &x) const override {
    scalar_t z = compute_override(x);
    // HACK: to update state of the object do const_cast
    auto &self = const_cast<TestProblem &>(*this);
    self.computed_points_.push_back(x);
    self.computed_values_.push_back(z);
    self.minimizer_.update(z, x);
    return z;
  }

  const std::list<scalar_t> &computed_values() const {
    return computed_values_;
  }

  const std::list<vector_t> &computed_points() const {
    return computed_points_;
  }

  const Minimizer &computed_minimizer() const {
    return minimizer_;
  }

protected:
  virtual scalar_t compute_override(const vector_t &x) const = 0;

private:
  std::list<vector_t> computed_points_;
  std::list<scalar_t> computed_values_;
  Minimizer minimizer_;
};


template<typename ProblemType>
class TestProblemTemplate : public TestProblem {
public:
  template<typename ...Args>
  explicit TestProblemTemplate(Args &&...args)
      : impl_(std::forward<Args>(args)...) { }

  const BoundingBox &bbox() const override {
    return impl().bbox();
  }

  const Minimizer &ground_truth() const override {
    return impl().ground_truth();
  }

protected:
  scalar_t compute_override(const vector_t &x) const override {
    return impl().compute(x);
  }

  const ProblemType &impl() const {
    return impl_;
  }

private:
  ProblemType impl_;
};

using GrishaginsTestProblem =
  TestProblemTemplate<problems::GrishaginsProblem>;

} // namespace testing
} // namespace adaptive_ags

#endif
