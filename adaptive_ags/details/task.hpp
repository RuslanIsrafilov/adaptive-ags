#ifndef ADAPRIVE_AGS_DETAILS_TASK_HPP_
#define ADAPRIVE_AGS_DETAILS_TASK_HPP_

#include <queue>
#include <utility>

#include "adaptive_ags/method.hpp"
#include "adaptive_ags/details/intervals.hpp"
#include "boost/heap/fibonacci_heap.hpp"

namespace adaptive_ags {
namespace details {

class Task;
class TaskQueue;
class TaskContext;

class BaseTask {
public:
  struct compare {
    bool operator()(const BaseTask *lhs,
                    const BaseTask *rhs) const {
      return lhs->weight() < rhs->weight();
    }
  };

  explicit BaseTask(scalar_t reliability)
      : iset_(reliability) { }

  scalar_t weight() const {
    return iset_.best_weight();
  }

protected:
  IntervalSet iset_;
};

using TaskHandle = boost::heap::fibonacci_heap<BaseTask *,
  boost::heap::compare<BaseTask::compare>>::handle_type;

class Task : public BaseTask {
public:
  /**
   * @brief      Creates a root task and pushes it
   *             to the queue in the given context
   *
   * @param[in]  context  The context object
   *
   * @return     Reference to the created root task
   */
  static Task &create_root(TaskContext &context);

  /**
   * @brief      Executes a single iteration of the global
   *             search algorithm within the task
   *
   * @return     Returns an epsilon (length of the interval
   *             given the highest characteristic)
   */
  scalar_t execute_iteration();

  /**
   * @return     Characteristic of the task (the largest
   *             characteristic among interval)
   */
  scalar_t weight() const;

  /**
   * @return     Minimal value of the function being
   *             computed during optimization
   */
  scalar_t min_z() const;

  /**
   * @return     Number of the fixed variables for the task.
   *             For the root task level is equal to zero
   */
  dim_t level() const;

  /**
   * @return     { description_of_the_return_value }
   */
  scalar_t epsilon() const;

  /**
   * @brief      Checks whether task is leaf in a tree
   *
   * @return     True if task is a leaf, otherwise False
   */
  bool leaf() const;

  const TaskHandle &handle() const {
    return handle_;
  }

  void attach_to_queue(const TaskHandle &handle) {
    handle_ = handle;
    is_attached_ = true;
  }

  void deattach_from_queue() {
    is_attached_ = false;
  }

  bool is_attached_to_queue() const {
    return is_attached_;
  }

private:
  /**
   * @brief      Constructs the task
   *
   * @param[in]  context     The context object
   * @param[in]  fixed_args  Fixed variables
   */
  explicit Task(TaskContext &context,
                Task *parent,
                const vector_t &fixed_args);

  /**
   * @brief      { function_description }
   */
  void initialize();

  /**
   * @brief      { function_description }
   *
   * @return     { description_of_the_return_value }
   */
  scalar_t execute_iteration_internal();

  /**
   * @brief      { function_description }
   *
   * @param[in]  x     { parameter_description }
   *
   * @return     { description_of_the_return_value }
   */
  scalar_t compute(scalar_t x);

  /**
   * @brief      Calculates the in leaf.
   *
   * @param[in]  x     { parameter_description }
   *
   * @return     In leaf.
   */
  scalar_t compute_in_leaf(const vector_t &x);

  /**
   * @brief      { function_description }
   *
   * @param[in]  index  The index
   * @param[in]  z      { parameter_description }
   */
  void update(scalar_t x, scalar_t z);

  /**
   * @brief      { function_description }
   *
   * @param[in]  z_before  The z before
   * @param[in]  z_after   The z after
   *
   * @return     { description_of_the_return_value }
   */
  bool cause_parent_update(scalar_t z_before, scalar_t z_after);

  /**
   * @brief      Sets the variable argument.
   *
   * @param[in]  x     { parameter_description }
   *
   * @return     { description_of_the_return_value }
   */
  const vector_t &set_variable_arg(scalar_t x);

  TaskContext &context_;
  vector_t args_;
  Task *parent_;
  TaskHandle handle_;
  bool is_attached_;
};

class TaskQueue {
public:
  Task &push(Task *task) {
    auto handle = queue_.push(task);
    task->attach_to_queue(handle);
    return *task;
  }

  void update(Task *task) {
    if (task->is_attached_to_queue()) {
      queue_.update(task->handle());
    }
  }

  Task *pop() {
    BaseTask *base_task = queue_.top();
    queue_.pop();
    Task *task = static_cast<Task *>(base_task);
    task->deattach_from_queue();
    return task;
  }

  bool empty() const {
    return queue_.empty();
  }

private:
  boost::heap::fibonacci_heap<
    BaseTask *,
    boost::heap::compare<BaseTask::compare>> queue_;
};


class TaskContext {
public:
  TaskContext(const Problem &problem,
              const Parameters &params,
              TaskQueue &queue)
      : problem_(problem),
        params_(params),
        queue_(queue) { }

  const Problem &problem_;
  const Parameters &params_;
  TaskQueue &queue_;
  Result result_;
};


Task &Task::create_root(TaskContext &context) {
  auto root = new Task(context, nullptr, utils::empty_vec());
  return context.queue_.push(root);
}

scalar_t Task::execute_iteration() {
  _ADAPTIVE_AGS_ASSERT_( !iset_.empty() );

  const scalar_t min_z_before = min_z();
  const scalar_t epsilon = execute_iteration_internal();
  const scalar_t min_z_after = min_z();

  cause_parent_update(min_z_before, min_z_after);

  return epsilon;
}

scalar_t Task::weight() const {
  return iset_.best_weight();
}

scalar_t Task::min_z() const {
  return iset_.min_z();
}

dim_t Task::level() const {
  _ADAPTIVE_AGS_ASSERT_( args_.size() > 0 );
  return (dim_t)(args_.size() - 1);
}

scalar_t Task::epsilon() const {
  _ADAPTIVE_AGS_ASSERT_( !iset_.empty() );
  return iset_.best_length();
}

bool Task::leaf() const {
  const auto &bbox = context_.problem_.bbox();
  return bbox.dimension() == level() + 1;
}

Task::Task(TaskContext &context,
           Task *parent,
           const vector_t &fixed_args)
    : BaseTask(context.params_.reliability_),
      context_(context),
      parent_(parent),
      is_attached_(false) {
  _ADAPTIVE_AGS_ASSERT_( (fixed_args.size() == 0) || parent_ );

  /* Initialize args_ with the fixed_args and
   * reserve one element for a variable argument */
  args_.reserve(fixed_args.size() + 1);
  args_.assign(fixed_args.begin(), fixed_args.end());
  args_.push_back(0.0);

  initialize();
}

void Task::initialize() {
  const auto &bbox = context_.problem_.bbox();

  const dim_t lvl = level();
  const scalar_t a = bbox.left()[lvl];
  const scalar_t b = bbox.right()[lvl];
  _ADAPTIVE_AGS_ASSERT_( a < b );

  iset_.push_first(a, compute(a),
                   b, compute(b));

  const size_t init_max_iters = context_.params_.init_max_iters_;
  const scalar_t init_epsilon = context_.params_.init_epsilon_;
  _ADAPTIVE_AGS_ASSERT_( context_.params_.epsilon_ <= init_epsilon );

  for (size_t i = 0; i < init_max_iters; i++) {
    const scalar_t e = execute_iteration_internal();
    if (e < init_epsilon) { break; }
  }
}

scalar_t Task::execute_iteration_internal() {
  const scalar_t next_x = iset_.next();
  const scalar_t next_z = compute(next_x);
  return iset_.push(next_x, next_z);
}

scalar_t Task::compute(scalar_t x) {
  const vector_t &args = set_variable_arg(x);

  if (leaf()) {
    return compute_in_leaf(args);
  }
  else {
    auto child = new Task(context_, this, args);
    return context_.queue_.push(child).min_z();
  }
}

scalar_t Task::compute_in_leaf(const vector_t &x) {
  const scalar_t z = context_.problem_.compute(x);
  context_.result_.update(z, x);
  return z;
}

void Task::update(scalar_t x, scalar_t z) {
  const scalar_t min_z_before = min_z();
  iset_.update(x, z);
  context_.queue_.update(this);
  const scalar_t min_z_after = min_z();

  context_.result_.statistics().register_task_update(level());
  cause_parent_update(min_z_before, min_z_after);
}

bool Task::cause_parent_update(scalar_t z_before,
                               scalar_t z_after) {
  const bool updated = context_.params_.enable_updates_
                       && parent_ && z_after < z_before;
  if (updated) {
    _ADAPTIVE_AGS_ASSERT_( level() > 0 );
    const scalar_t parent_x = args_[level() - 1];
    parent_->update(parent_x, z_after);
  }
  return updated;
}

const vector_t &Task::set_variable_arg(scalar_t x) {
  args_.back() = x;
  return args_;
}

} // namespace details
} // namespace adaptive_ags

#endif
