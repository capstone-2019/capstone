class Node(object):

	"""
	Class to represent a node in the circuit. A node is simply a location
	where multiple components connect.
	"""

	def __init__(self, node_id):
		"""
		Constructs a node, assigning it a unique identifier.
		"""
		self.node_id = node_id
		self.components = []

	def __hash__(self):
		"""
		Hash a node based on the node identifier.
		"""
		return hash(self.node_id)

	def __eq__(self, other):
		"""
		Check if nodes are equal based on the node identifier.
		"""
		return self.node_id == other.node_id

	def __lt__(self, other):
		"""
		Compares nodes based on the node identifier. Can be useful to
		sort nodes for debugging purposes, for example.
		"""
		return self.node_id < other.node_id

	def __repr__(self):
		"""
		Produce a string representation of the node from the node identifer
		and the set of components that converge at this node.
		"""
		component_names = [c.get_name() for c in self.components]
		return "Node {} - components: {}".format(self.node_id, component_names)

	def __str__(self):
		"""
		Produce a string representation of the node from the node identifer
		and the set of components that converge at this node.
		"""
		return "%r" % self

	def add_component(self, component):
		"""
		Hook up a new component to this node.
		"""
		self.components.append(component)
