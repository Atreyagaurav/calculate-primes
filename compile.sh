swig -python primes.i
python setup.py build_ext --inplace -I ~/.local/lib/python3.8/site-packages/numpy/core/include/
