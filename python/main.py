import ctypes

lib = ctypes.CDLL('build/libevrp.dll')
lib.enter_car()