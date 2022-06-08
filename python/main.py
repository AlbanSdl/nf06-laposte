from ctypes import POINTER, Structure, CDLL, byref, c_float, c_int


class Car(Structure):
    _fields_ = ("capacity", c_int), ("battery", c_int), ("speedkmh", c_float), ("rechargetps", c_float)

class Node(Structure):
    _fields_ = ("id", c_int), ("distances", POINTER(c_float))

class RoutedVehicle(Structure):
    _fields_ = ("nodes", POINTER(c_int)), ("nodeCount", c_int), ("time", c_float)


lib = CDLL('build/libevrp.dll')
computePath = lib.compute_path
result = (RoutedVehicle * 1)()
computePath(
    (Node * 2)(Node(0, (c_float * 2)(0, 2)), Node(1, (c_float * 2)(2, 0))),
    c_int(2),
    (Car * 1)(Car(2, 100, 50, 5)),
    c_int(1),
    byref(result)
)
for i in result:
    print("Length (in nodes, excluding first warehouse)", i.nodeCount, "time", i.time)
    print("Route: 0", end="->")
    for j in range(i.nodeCount):
        print(i.nodes[j], end="->")
