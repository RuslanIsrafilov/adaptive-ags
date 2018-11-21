cimport numpy as np
import numpy as np
from libc.string cimport memcpy
from libcpp.memory cimport unique_ptr
from cython.operator import dereference

np_scalar_t = np.float64
ctypedef np.float64_t c_np_scalar_t

cdef vector_to_numpy(const vector_t &vector):
    cdef np.ndarray[c_np_scalar_t, mode='c', ndim=1] array
    array = np.empty(vector.size(), dtype=np_scalar_t)
    memcpy(&array[0], vector.data(), vector.size() * sizeof(scalar_t))
    return array

cdef unique_ptr[BoundingBox] bbox_from_tuple(bbox_tuple):
    # Cython is not able to return instance of class without
    # an empty constructor, so the function returns unique_ptr
    left = np.ascontiguousarray(bbox_tuple[0])
    right = np.ascontiguousarray(bbox_tuple[1])
    return unique_ptr[BoundingBox](new BoundingBox(left, right))

cdef scalar_t custom_problem_callback(void *context, const vector_t &x):
    return <float>(<object>context)(vector_to_numpy(x))


class Bunch(dict):
    __getattr__ = dict.__getitem__
    __setattr__ = dict.__setitem__
    __delattr__ = dict.__delitem__

cdef class PyProblem(object):
    cdef unique_ptr[Problem] c_problem

    cdef Problem *ref(self):
        return self.c_problem.get()

    def compute(self, np.ndarray[c_np_scalar_t, mode='c', ndim=1] x):
        return self.ref().compute(x)


cdef class PyCustomProblem(PyProblem):
    cdef object _func
    def __cinit__(self, func, bbox, ground_truth=None):
        # Need to store reference to external function
        # to convert it to void* and pass to native code,
        # otherwise 'func' may be deleted by GC.
        self._func = func
        self.c_problem.reset(
            new ProblemFromCallback(
                <void *>func, custom_problem_callback,
                dereference( bbox_from_tuple(bbox).get() )
            )
        )


cdef class PyGrishaginsProblem(PyProblem):
    def __cinit__(self, int number = 1):
        self.c_problem.reset(new GrishaginsProblem(number))


cdef class PyParameters(object):
    cdef unique_ptr[Parameters] c_parameters

    cdef Parameters *ref(self):
        return self.c_parameters.get()

    def __cinit__(self, **kw_args):
        self.c_parameters.reset(new Parameters())
        self.set_parameters(kw_args)

    cdef set_parameters(self, kw):
        cdef Parameters d
        all = [ 'max_iters', 'init_max_iters', 'epsilon',
                'init_epsilon', 'reliability', 'enable_updates' ]
        for k, w in kw.items(): assert k in all
        cdef Parameters *p = self.ref()
        p.max_iters_      = kw.get( all[0], d.max_iters_      )
        p.init_max_iters_ = kw.get( all[1], d.init_max_iters_ )
        p.epsilon_        = kw.get( all[2], d.epsilon_        )
        p.init_epsilon_   = kw.get( all[3], d.init_epsilon_   )
        p.reliability_    = kw.get( all[4], d.reliability_    )
        p.enable_updates_ = kw.get( all[5], d.enable_updates_ )

cdef wrap_result(const Result &result):
    cdef const Bias *b = &result.bias()
    cdef const Minimizer *m = &result.minimizer()
    cdef const Minimizer *gt = &result.ground_truth()

    return Bunch(
        # Convert found minimizer
        minimum = m.value() if m.available() else None,
        minimizer = vector_to_numpy(m.argument()) if m.available() else None,

        # Convert ground truth minimizer
        gt_minimum = gt.value() if gt.available() else None,
        gt_minimizer = vector_to_numpy(gt.argument()) if gt.available() else None,

        # Convert bias
        bias_by_value = b.by_value() if b.available() else None,
        bias_by_argument = b.by_argument() if b.available() else None,

        # Statistics
        trials_number = result.statistics().trials_number(),
        total_task_updates = result.statistics().total_task_updates()
    )

def minimize(PyProblem py_problem, PyParameters py_parameters):
    cdef Method method
    cdef Problem *problem = py_problem.ref()
    cdef Parameters *params = py_parameters.ref()
    cdef Result result = method.run(dereference(problem),
                                    dereference(params))
    return wrap_result(result)
