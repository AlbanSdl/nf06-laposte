import ctypes

lib = ctypes.CDLL('build/libevrp.dll')
lib.test()