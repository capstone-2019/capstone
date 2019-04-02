import numpy as np
import matplotlib.pyplot as plt
from circuitSimulatorOutput import CircuitSimulatorOutput

class CircuitSimulatorInputOutput(CircuitSimulatorOutput):

	def __init__(self, inFile, outFile):
		self.input = CircuitSimulatorOutput(inFile)
		self.output = CircuitSimulatorOutput(outFile)

	def plotSignal(self):
		fig, ax = plt.subplots(2)
		self.input._plotSignal(ax[0])
		self.output._plotSignal(ax[1])
		plt.show()

	def plotFft(self):
		fig, ax = plt.subplots(2)
		self.input._plotFft(ax[0])
		self.output._plotFft(ax[1])
		plt.show()

	def plot(self):
		fig, ax = plt.subplots(2, 2)

		self.input._plot(ax[0][0], ax[0][1])
		self.output._plot(ax[1][0], ax[1][1])
		plt.show()
