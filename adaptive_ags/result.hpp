#ifndef ADAPRIVE_AGS_RESULT_HPP_
#define ADAPRIVE_AGS_RESULT_HPP_

#include <list>
#include <cassert>

#include "adaptive_ags/problem.hpp"

namespace adaptive_ags {

enum class TerminationStatus {
  max_iterations_exceeded,
  processed_all_tasks,
  converged_in_root
};

class Trial {
public:
  explicit Trial(const vector_t &x,
                 const scalar_t &z)
      : x_(x), z_(z) { }

  const vector_t &x() const {
    return x_;
  }

  const scalar_t &z() const {
    return z_;
  }

private:
  vector_t x_;
  scalar_t z_;
};

class Statistics {
public:
  void register_trial(scalar_t z, const vector_t &x) {
    constexpr bool enable_trials_saving = false;
    trials_number_++;
    if (enable_trials_saving) {
      trials_.emplace_back(x, z);
      assert( trials_.size() == trials_number_ );
    }
  }

  void register_task_update(dim_t level) {
    if (level >= task_updates_.size()) {
      task_updates_.resize(level + 1);
    }
    task_updates_[level]++;
  }

  size_t trials_number() const {
    return trials_number_;
  }

  const std::vector<size_t> &task_updates() const {
    return task_updates_;
  }

  size_t total_task_updates() const {
    size_t total_updates = 0;
    for (size_t i = 0; i < task_updates_.size(); i++) {
      total_updates += task_updates_[i];
    }
    return total_updates;
  }

private:
  size_t trials_number_ = 0;
  std::list<Trial> trials_;
  std::vector<size_t> task_updates_;
};

class Bias {
public:
  Bias() :
    available_(false),
    bias_by_value_(utils::infty()),
    bias_by_argument_(utils::infty()) { }

  explicit Bias(const Minimizer &ground_truth,
                const Minimizer &computed)
      : available_(ground_truth.available() &&
                   computed.available()),
        bias_by_value_(utils::infty()),
        bias_by_argument_(utils::infty()) {
    if (available_) {
      bias_by_value_ = utils::abs( ground_truth.value() -
                                   computed.value() );
      bias_by_argument_ = utils::l2_norm( ground_truth.argument(),
                                          computed.argument() );
    }
  }

  bool available() const {
    return available_;
  }

  scalar_t by_value() const {
    return bias_by_value_;
  }

  scalar_t by_argument() const {
    return bias_by_argument_;
  }

private:
  bool available_;
  scalar_t bias_by_value_;
  scalar_t bias_by_argument_;
};

class Result {
public:
  void update(scalar_t z, const vector_t &x) {
    minimizer_.update(z, x);
    statistics_.register_trial(z, x);
  }

  void finalize(const Minimizer &ground_truth,
                const TerminationStatus &termination_status) {
    bias_ = Bias(ground_truth, minimizer_);
    ground_truth_ = ground_truth;
    termination_status_ = termination_status;
  }

  const Bias &bias() const {
    return bias_;
  }

  const Minimizer &minimizer() const {
    return minimizer_;
  }

  const Minimizer &ground_truth() const {
    return ground_truth_;
  }

  const Statistics &statistics() const {
    return statistics_;
  }

  Statistics &statistics() {
    return statistics_;
  }

  const TerminationStatus &status() const {
    return termination_status_;
  }

private:
  Bias bias_;
  Minimizer minimizer_;
  Minimizer ground_truth_;
  Statistics statistics_;
  TerminationStatus termination_status_;
};

} // namespace adaptive_ags

#endif
