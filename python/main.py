from ctypes import POINTER, Array, Structure, CDLL, byref, c_float, c_int
import openpyxl
import networkx as nx
import matplotlib.pyplot as plt


class Car(Structure):
    _fields_ = ("capacity", c_int), ("battery", c_int), ("speedkmh", c_float), ("rechargetps", c_float)

class Node(Structure):
    _fields_ = ("id", c_int), ("distances", POINTER(c_float))

class RoutedVehicle(Structure):
    _fields_ = ("nodes", POINTER(c_int)), ("nodeCount", c_int), ("time", c_float)


def computePath(nodes, cars) -> Array[RoutedVehicle]:
    lib = CDLL('build/libevrp.dll')
    computePath = lib.compute_path
    result = (RoutedVehicle * len(cars))()
    computePath(
        (Node * len(nodes))(*nodes),
        c_int(len(nodes)),
        (Car * len(cars))(*cars),
        c_int(len(cars)),
        byref(result)
    )
    return result


def readFromFile(path: str) -> tuple[list[Node], list[Car]]:
    wb_obj = openpyxl.load_workbook(path)
    node_sheet = wb_obj["Feuil1"]

    nodes = []
    for j in range(2, node_sheet.max_row + 1):
        nodes.append({
            "id": j - 2,
            "x": node_sheet.cell(row=j, column=1).value,
            "y": node_sheet.cell(row=j, column=2).value,
        })

    c_nodes = []
    for node in nodes:
        c_nodes.append(Node(id=node["id"], distances=(c_float * len(nodes))(
            *[((node["x"] - n["x"]) ** 2 + (node["y"] - n["y"]) ** 2) ** .5 for n in nodes]
        )))
    
    car_sheet = wb_obj["Feuil2"]
    cars = []
    for i in range(2, car_sheet.max_row + 1):
        cars.append(Car(
            capacity=car_sheet.cell(row=i, column=1).value,
            battery=car_sheet.cell(row=i, column=2).value,
            speedkmh=car_sheet.cell(row=i, column=4).value,
            rechargetps=car_sheet.cell(row=i, column=3).value
        ))

    wb_obj.close()
    return (nodes, c_nodes, cars)


def displayResult(nodes, routedVehicles: Array[RoutedVehicle]):
    G=nx.Graph()

    for node in nodes:
        G.add_node(node["id"], pos=(node["x"], node["y"]))

    for i in routedVehicles:
        for j in range(0, i.nodeCount - 2):
            if i == 0:
                G.add_edge(0, i.nodes[j])
            G.add_edge(i.nodes[j], i.nodes[j + 1])

    pos=nx.get_node_attributes(G, 'pos')
    nx.draw(G, pos, with_labels=True)
    plt.show()


(nodes, c_nodes, cars) = readFromFile("lieux.xlsx")
routes = computePath(c_nodes, cars)
displayResult(nodes, routes)