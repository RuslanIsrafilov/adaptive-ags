from . import adaptive_ags_ as ags_

class Problem(object):
    def __init__(self):
        self._native = None

    def compute(self, x):
        return self._native.compute(x)

    def native(self):
        return self._native

    def _set(self, native):
        self._native = native


class GrishaginsProblem(Problem):
    def __init__(self, number = 1):
        self._set(ags_.PyGrishaginsProblem(number))


class CustomProblem(Problem):
    def __init__(self, func, bbox, ground_truth = None):
        self._set(ags_.PyCustomProblem(func, bbox))


def create_problem_from_dictionary(descriptor: dict) -> Problem:
    assert descriptor is not None
    assert 'name' in descriptor
    name = descriptor['name']
    if name == 'grishagins':
        return GrishaginsProblem(number = descriptor['number'])
    else:
        raise Exception('Cannnot find problem with '
                        'the name "{}"'.format(name))

def create_problem_from_string(descriptor: str) -> Problem:
    assert descriptor is not None
    descriptor_parts = descriptor.split(':')
    assert len(descriptor_parts) == 2
    name, number_str = tuple(descriptor_parts)
    if name == "grishagins":
        return GrishaginsProblem(number = int(number_str))
    else:
        raise Exception('Cannnot create problem from '
                        'string "{}"'.format(descriptor))

def create_problem(descriptor) -> Problem:
    if isinstance(descriptor, Problem):
        return descriptor
    elif isinstance(descriptor, dict):
        return create_problem_from_dictionary(descriptor)
    elif isinstance(descriptor, str):
        return create_problem_from_string(descriptor)
    else:
        raise Exception('Cannnot create problem object from '
                        'descriptor {}'.format(descriptor))

def minimize(problem_descriptor, **kwargs):
    problem = create_problem(problem_descriptor)
    parameters = ags_.PyParameters(**kwargs)
    return ags_.minimize(problem.native(), parameters)
