import numpy as np
import adaptive_ags as ags

def minimize_premade_function(params):
    # Minimize pre-made function, for example
    # https://www.sciencedirect.com/science/article/pii/S1877050915010248
    result = ags.minimize('grishagins:1', **params)

    print('Grishagin function:')
    print('f_min =', result.minimum)
    print('x_min =', result.minimizer)
    print('trials =', result.trials_number)
    print()

def minimize_custom_function(params):
    # Shubert function
    # https://www.sfu.ca/~ssurjano/shubert.html
    f_i = np.arange(1, 5 + 1)
    def f(x):
        l = f_i * np.cos((f_i + 1) * x[0] + f_i)
        r = f_i * np.cos((f_i + 1) * x[1] + f_i)
        return np.sum(l) * np.sum(r)

    # Bounding box: x_1, x_2 \in [-2, 2]
    bbox = ([ -2, -2 ], # x_1
            [  2,  2 ]) # x_2

    # Create Shubert problem
    problem = ags.CustomProblem(f, bbox)

    # Minimize pre-made function
    result = ags.minimize(problem, **params)

    print('Shubert function:')
    print('f_min =', result.minimum)
    print('x_min =', result.minimizer)
    print('trials =', result.trials_number)
    print()

def main():
    # All parameters of Adaptive AGS solver
    params = dict(
        max_iters      = 1000,
        init_max_iters = 10,
        epsilon        = 0.01,
        init_epsilon   = 0.05,
        reliability    = 3.5,
        enable_updates = False
    )

    # Example of premade function minimization
    minimize_premade_function(params)

    # Example of custom function minimization
    minimize_custom_function(params)

if __name__ == '__main__':
    main()
