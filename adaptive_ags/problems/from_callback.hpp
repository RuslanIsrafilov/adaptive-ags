#ifndef ADAPRIVE_AGS_PROBLEMS_FROM_CALLBACK_HPP_
#define ADAPRIVE_AGS_PROBLEMS_FROM_CALLBACK_HPP_

#include "adaptive_ags/problem.hpp"
#include "adaptive_ags/problems/grishagins_impl.hpp"

namespace adaptive_ags {
namespace problems {

typedef scalar_t (*problem_callback_t)(void *context,
                                       const vector_t &x);

class ProblemFromCallback : public Problem {
public:
  explicit ProblemFromCallback(void *context,
                               problem_callback_t callback,
                               const BoundingBox &bbox)
      : context_(context),
        callback_(callback),
        bbox_(bbox) { }

  const BoundingBox &bbox() const override {
    return bbox_;
  }

  const Minimizer &ground_truth() const override {
    return ground_truth_;
  }

  scalar_t compute(const vector_t &x) const override {
    return callback_(context_, x);
  }

private:
  void *context_;
  problem_callback_t callback_;
  BoundingBox bbox_;
  Minimizer ground_truth_;
};

} // namespace problems
} // namespace adaptive_ags

#endif
