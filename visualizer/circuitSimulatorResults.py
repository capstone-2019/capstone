import numpy as np
import matplotlib.pyplot as plt
from circuitSimulatorOutput import CircuitSimulatorOutput

class CircuitSimulatorResults(CircuitSimulatorOutput):

	def __init__(self, inFile, outFile = None, outFile2 = None):
		count = 1
		self.input = CircuitSimulatorOutput(inFile)
		self.data = [self.input]
		if outFile != None:
			self.output = CircuitSimulatorOutput(outFile)
			count += 1
			self.data.append(self.output)
		if outFile2 != None:
			self.output2 = CircuitSimulatorOutput(outFile2)
			count += 1
			self.data.append(self.output)

		self.count = count

	def plotSignal(self):
		fig, ax = plt.subplots(self.count)

		for i in range(self.count):
			self.data[i]._plotSignal(ax[i])
		plt.show()

	def plotFft(self):
		fig, ax = plt.subplots(self.count)
		for i in range(self.count):
			self.data[i]._plotFft(ax[i])
		plt.show()

	def getCorrelation(self):
		coeff = np.corrcoef(self.data[-2].getData(), self.data[-1].getData())
		assert(int(coeff[0][1]*1000) == int(coeff[1][0]*1000))
		return coeff[0][1]


	def plot(self):
		fig, ax = plt.subplots(self.count, 2)
		for i in range(self.count):
			self.data[i]._plot(ax[i][0], ax[i][1])
		if (self.count >= 2):
			print("correlation is", self.getCorrelation())
		plt.show()
