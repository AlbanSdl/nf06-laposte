import networkx as nx
import matplotlib.pyplot as plt
# G = nx.petersen_graph()
# G.add_node(1)

# subax1 = plt.subplot(131)
# nx.draw(G, with_labels=True, font_weight='bold')
# subax2 = plt.subplot(122)
# nx.draw_shell(G, nlist=[range(5, 10), range(5)], with_labels=True, font_weight='bold')
#plt.show()  
G=nx.Graph()
G.add_node(1,pos=(0,0))
G.add_node(2,pos=(-1,-1))
G.add_node(3,pos=(1,1))
G.add_edge(1,2)
pos=nx.get_node_attributes(G,'pos')
pos
{1: (1, 1), 2: (2, 2)}
nx.draw(G,pos)
plt.show()  
