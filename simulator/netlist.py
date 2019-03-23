import copy
from utils import efilter
from components import *
from node import *
from circuit import *

class NetlistIterator(object):

	# The symbol used to specify comments
	COMMENT_SYMBOL = "#"

	def __init__(self, filename):
		"""
		Construct the object and associate it with a given file.
		"""
		self.filename = filename

	def __enter__(self):
		"""
		Open the file for reading.
		"""
		self.f = open(self.filename, 'r')
		return self

	def __exit__(self, exception_type, exception_value, traceback):
		"""
		Clean up the open file descriptor.
		"""
		self.f.close()

	def __iter__(self):
		"""
		Iterate over the cleaned lines once any blank lines or block
		comments have been removed.
		"""
		lines = (self.clean(l) for l in self.f if self.contains_data(l))
		for line in lines:
			yield line

	@classmethod
	def contains_data(cls, line):
		"""
		Checks whether a line contains any valid data. This has the
		effect of filtering out blank lines or lines that just contain
		comments.
		"""
		line = line.strip()
		return (not line.startswith(cls.COMMENT_SYMBOL)) and len(line) > 0

	@classmethod
	def clean(cls, line):
		"""
		Cleans a line by removing leading and trailing whitespace and chopping
		off any inline comments, returning only the portion of the string
		which contains meaningful data.

		This line will also account for added internal spacing, such that the
		returned string will have tokens delimited by single spaces. For
		example, the string: "  foo bar    baz  " will be cleaned to the
		string: "foo bar baz"
		"""
		i = line.find(cls.COMMENT_SYMBOL)  # find inline comments
		line = line if i < 0 else line[:i] # remove comments
		line = line.strip()                # remove leading/trailing whitespace
		line = " ".join(line.split())      # account for extra internal spacing
		return line


class Netlist(object):

	def __init__(self, net_file):
		"""
		Given a netlist filepath, reads the file and generates a netlist
		represented in the simulator's internal data format.
		"""
		self.read_netlist_file(net_file)

	def read_netlist_file(self, net_file):
		"""
		Parses a netlist file into the circuit simulators internal
		format.
		"""
		with NetlistIterator(net_file) as net_iterator:
			components = efilter(
				lambda c : c is not None,
				(Component.from_netlist_description(l) for l in net_iterator))

		# construct each node and hook up components to nodes
		nodes = dict()
		for component in components:
			for node_id in component.get_nodes():
				node = nodes.get(node_id, Node(node_id))
				node.add_component(component)
				nodes[node_id] = node

		self.circuit = Circuit(components)

	def as_circuit(self):
		"""
		Returns a copy of the circuit representation of a netlist.
		"""
		return copy.deepcopy(self.circuit)
