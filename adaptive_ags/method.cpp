#include "adaptive_ags/method.hpp"
#include "adaptive_ags/details/task.hpp"

namespace adaptive_ags {

Result Method::run(const Problem &problem,
                   const Parameters &parameters) {

  details::TaskQueue queue;
  details::TaskContext context = { problem,
                                   parameters,
                                   queue };

  const auto &root = details::Task::create_root(context);

  for (size_t i = 0; i < parameters.max_iters_; i++) {
    const scalar_t epsilon = parameters.epsilon();

    if (queue.empty() || root.epsilon() < epsilon) {
      break;
    }

    details::Task *task = queue.pop();
    if (task->execute_iteration() >= epsilon) {
      queue.push(task);
    }
  }

  context.result_.finalize(problem.ground_truth(),
                           TerminationStatus::max_iterations_exceeded);
  return context.result_;
}

} // namespace adaptive_ags
