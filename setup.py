#!/usr/bin/env python

"""
setup.py file for primes module
"""

from distutils.core import setup, Extension


primes_module = Extension('_primes',
                           sources=['primes_wrap.c', 'primes.c'],
                           )

setup (name = 'primes',
       version = '0.1',
       author      = "Gaurav Atreya",
       description = """Generation of prime numbers as well as saving them in a file for later use, the backend is writen in C and is relatively fast.""",
       ext_modules = [primes_module],
       py_modules = ["primes"],
       )
