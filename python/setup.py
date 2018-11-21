import os
from setuptools import setup

setup(
    name         = "adaptive_ags",
    version      = "0.0.1",
    author       = "Ruslan Israfilov",
    license      = "MIT",
    packages     = [ 'adaptive_ags' ],
    package_data = { 'adaptive_ags': ['adaptive_ags_.so'] },
    zip_safe     = False
)
