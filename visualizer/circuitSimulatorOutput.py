import numpy as np
import matplotlib.pyplot as plt

class CircuitSimulatorOutput:

	def __init__(self, filename):
		self.f = open(filename, "r")

		# parse input
		lines = self.f.readlines()
		self.samplerate = int(lines[0].strip())
		self.numFrames = int(lines[1].strip())

		l = lines[2].strip().split(",")
		self.frames = np.asarray(map(float, lines[2].strip().split(",")[:-1]), dtype=np.float32)
		period = 1.0 /self.samplerate
		self.t = np.asarray([period * i for i in range(self.numFrames)])

		self.fft = None
		
		self.f.close()

	def _plotSignal(self, ax=None):
		if isinstance(ax, type(None)):
			fig, ax = plt.subplots(1)

		ax.set_title("signal")
		ax.set_xlabel("time")
		ax.set_ylabel("amplitude")
		ax.plot(self.t, self.frames)

	def plotSignal(self):
		self._plotSignal()
		plt.show()

	def _plotFft(self, ax=None):
		if isinstance(ax, type(None)):
			fig, ax = plt.subplots(1)

		# calcualte fft and normalize
		if (self.fft == None):
			self.fft = abs(np.fft.fft(self.frames)/self.numFrames)
			self.fft = self.fft[range(self.numFrames / 2)]

		k = np.arange(self.numFrames)
		T = self.numFrames / self.samplerate
		frq = k / T
		frq = frq[range(self.numFrames/2)]

		ax.set_title("frequency domain")
		ax.set_xlabel("frequency")
		ax.set_ylabel("|Y[freq]|")
		ax.plot(frq, self.fft)

	def plotFft(self, plot=None):
		self._plotFft()
		plt.show()

	def _plot(self, ax0=None, ax1=None):
		if isinstance(ax0, type(None)) or isinstance(ax1, type(None)):
			fig, ax = plt.subplots(2, 1)
			ax0 = ax[0]
			ax1 = ax[1]

		self._plotSignal(ax0)
		self._plotFft(ax1)
		plt.tight_layout()

	def plot(self):
		self._plot()
		plt.show()

	def getData(self):
		return self.frames

	def getSampleRate(self):
		return self.samplerate

	def getNumFrames(self):
		return self.numFrames


