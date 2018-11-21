from libcpp cimport bool
from libcpp.vector cimport vector
from libc.stdint cimport uint8_t

cdef extern from "adaptive_ags/defines.hpp" namespace "adaptive_ags":
    ctypedef double scalar_t
    ctypedef uint8_t dim_t
    ctypedef vector[scalar_t] vector_t

cdef extern from "adaptive_ags/problem.hpp" namespace "adaptive_ags":
    cdef cppclass BoundingBox:
        BoundingBox(dim_t)
        BoundingBox(dim_t, scalar_t, scalar_t)
        BoundingBox(const vector_t &, const vector_t &)
        dim_t dimension() const
        const vector_t &left() const
        const vector_t &right() const

    cdef cppclass Minimizer:
        bool available() const
        scalar_t value() const
        const vector_t &argument() const

    cdef cppclass Problem:
        const BoundingBox &bbox() const
        const Minimizer &ground_truth() const
        scalar_t compute(const vector_t &) const

cdef extern from "adaptive_ags/problems/grishagins.hpp" namespace "adaptive_ags::problems":
    cdef cppclass GrishaginsProblem:
        GrishaginsProblem(int) except +

cdef extern from "adaptive_ags/problems/from_callback.hpp" namespace "adaptive_ags::problems":
    ctypedef scalar_t (*problem_callback_t)(void *context, const vector_t &x)
    cdef cppclass ProblemFromCallback:
        ProblemFromCallback(void *context,
                            problem_callback_t callback,
                            const BoundingBox &bbox) except +

cdef extern from "adaptive_ags/result.hpp" namespace "adaptive_ags":
    cdef cppclass Statistics:
        size_t trials_number() const
        size_t total_task_updates() const

    cdef cppclass Bias:
        bool available() const
        scalar_t by_value() const
        scalar_t by_argument() const

    cdef cppclass Result:
        const Bias &bias() const
        const Minimizer &minimizer() const
        const Minimizer &ground_truth() const
        const Statistics &statistics() const

cdef extern from "adaptive_ags/method.hpp" namespace "adaptive_ags":
    cdef cppclass Parameters:
        size_t   max_iters_
        size_t   init_max_iters_
        scalar_t epsilon_
        scalar_t init_epsilon_
        scalar_t reliability_
        bool     enable_updates_

    cdef cppclass Method:
        Result run(const Problem &, const Parameters &)
