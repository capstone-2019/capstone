from components import *
import numpy as np
import copy
from linsolve import LinearSystem
import math
from utils import timer_decorator

class Circuit(object):
	"""
	Class to represent a circuit, which is effectively a collection
	of components. This class also provides mechanisms to perform
	various analyses on the circuit, such as the transient analysis which
	is required for the guitar pedal simulator.
	"""

	def __init__(self, components):
		"""
		Initialize the circuit object based on a collection of components
		that make up the circuit.
		"""

		# register all components, tracking relevant unknowns as we go
		self.unknowns = dict()
		self.next_unknown = 0
		self.components = components
		for component in self.components:
			self.register_component(component)

		# start at t = 0, with our initial guesses zerod out
		self.current_time = 0.0
		self.soln = np.zeros((self.total_unknowns, 1))
		self.prev_soln = np.zeros((self.total_unknowns, 1))

	def timescale(self):
		"""
		Returns the time axis values used in the simulation of this
		circuit.
		"""
		return self.voltage_in.timescale()

	def register_component(self, component):
		"""
		Registers a component by updating any necessary metadata and
		recording any new unknowns that this component may introduce.
		"""

		# check to see if this is a registered ground
		if isinstance(component, Ground):
			self.ground = component.node_id

		if isinstance(component, VoltageIn):
			self.voltage_in = component

		if isinstance(component, VoltageOut):
			out_pos, out_neg = component.terminals()
			self.out_pos_id = self.unknowns[out_pos]
			self.out_neg_id = self.unknowns[out_neg]

		# find all unknowns that this element contributes to
		# the system of equations we'll need to solve for this
		# circuit
		for unknown in component.unknowns():
			if unknown not in self.unknowns:
				self.unknowns[unknown] = self.next_unknown
				self.next_unknown += 1

		self.total_unknowns = len(self.unknowns)

	def build_matrix(self, time, dt):
		"""
		Builds and returns the matrix of KCL equations and voltage constraints
		based on this circuit and the current operating points of all
		components.
		"""
		system = LinearSystem(self.unknowns, self.ground)
		for component in self.components:
			component.add_matrix_contribution(
				system,
				self.soln,
				self.prev_soln,
				dt,
				time)
		return system

	def process_deltas(self, deltas, tolerance=1e-8):
		max_delta = max(abs(deltas[i][0]) for i in range(self.total_unknowns))
		converged = max_delta < tolerance or math.isnan(max_delta)
		for i in range(self.total_unknowns):
			self.prev_soln[i][0] += deltas[i][0]
		return converged

	@timer_decorator
	def transient(self, max_iterations=100):

		solution = []       # the output voltages
		input_signal = []   # the input voltages
		timescale = []      # the timestamp associated with each sample

		dt = self.voltage_in.timestep()

		for (time, voltage) in self.voltage_in:

			# record timestamp and input signal
			timescale.append(time)
			input_signal.append(voltage)

			# copy solution from prev iteration into prev_soln
			self.prev_soln[:] = self.soln

			# perform newton's method until convergence,
			# reconstructing KCL/constraint matrix each time
			for iteration in range(max_iterations):
				system = self.build_matrix(time, dt)
				deltas = system.solve()
				converged = self.process_deltas(deltas)
				if converged: break

			# now we have final solution for all the unknowns
			self.soln[:] = self.prev_soln

			# calculate and record output voltage at this timestamp
			vpos = self.soln[self.out_pos_id][0]
			vneg = self.soln[self.out_neg_id][0]
			vout = vpos - vneg
			solution.append(vout)

		return timescale, input_signal, solution
