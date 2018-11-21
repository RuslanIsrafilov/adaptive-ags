#include <iostream>

#include "adaptive_ags/method.hpp"
#include "adaptive_ags/testing/utils.hpp"
#include "adaptive_ags/testing/problems.hpp"
#include "adaptive_ags/testing/ref_nested_method.hpp"

namespace adaptive_ags {
namespace {

using testing::RefNestedMethod;
using testing::RefNestedParameters;
using testing::GrishaginsTestProblem;

class OnGrishaginsFunctions : public ::testing::TestWithParam<int> { };

TEST_P(OnGrishaginsFunctions, InCornerCaseTrialsAreEqualToNestedSolver) {
  auto params = Parameters()
    .max_iters(0)
    .epsilon(0.01)
    .init_max_iters(20)
    .init_epsilon(0.01)
    .reliability(3.5);
  auto problem = GrishaginsTestProblem(GetParam());

  auto params_ref = RefNestedParameters()
    .epsilon(params.epsilon())
    .level_iters(params.init_max_iters())
    .reliability(params.reliability());
  auto problem_ref = GrishaginsTestProblem(GetParam());

  Method().run(problem, params);
  RefNestedMethod().run(problem_ref, params_ref);

  testing::assert_points_are_equal(problem.computed_points(),
                                   problem_ref.computed_points());
}

TEST_P(OnGrishaginsFunctions, MethodConverges) {
  auto params = Parameters()
    .max_iters(1000)
    .epsilon(0.01)
    .init_max_iters(10)
    .init_epsilon(0.05)
    .reliability(4.0)
    .enable_updates(false);
  auto problem = GrishaginsTestProblem(GetParam());

  Method().run(problem, params);

  testing::assert_vectors_are_close(problem.computed_minimizer().argument(),
                                    problem.ground_truth().argument(),
                                    0.01 /* epsilon */);
}

INSTANTIATE_TEST_CASE_P(AllGrishaginsFunctions,
                        OnGrishaginsFunctions,
                        ::testing::Range(1, 100 + 1));

} // namespace
} // namespace adaptive_ags
