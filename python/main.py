import ctypes
import networkx as nx
import numpy as np
import matplotlib.pyplot as plt

# lib = ctypes.CDLL('build/libevrp.dll')
# lib.enter_car()

A = np.matrix([[0,1,1,0],[1,0,0,0],[1,1,0,1],[0,0,0,0]])

# Create DiGraph from A
G = nx.from_numpy_matrix(A, create_using=nx.DiGraph)

# Use spring_layout to handle positioning of graph
layout = nx.spring_layout(G)

# Draw the graph using the layout - with_labels=True if you want node labels.
nx.draw(G, layout, with_labels=True)

# Get weights of each edge and assign to labels
labels = nx.get_edge_attributes(G, "weight")

# Draw edge labels using layout and list of labels
nx.draw_networkx_edge_labels(G, pos=layout, edge_labels=labels)

# Show plot
plt.show()