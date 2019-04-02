#!/usr/bin/env python

"""
visualizer.py

Given .cso files, generates graphs of the audio signal in both the time
and frequency domains. Also reports the cross correlation or similarity
between the last two files. Supports up to three files, allowing users
to plot the original signal, the output signal and the benchmark signal.


"""

import sys
import numpy as np
from circuitSimulatorResults import CircuitSimulatorResults
from circuitSimulatorOutput import CircuitSimulatorOutput

def main():

	if len(sys.argv) <= 1 or len(sys.argv) > 4:
		print("please provide from 1, 2 or 3 files")
		return

	if len(sys.argv) == 2:
		cso = CircuitSimulatorOutput(sys.argv[1])
		cso.plot()
		return

	cso = CircuitSimulatorResults(*sys.argv[1:])
	cso.plot()

if __name__ == "__main__":
	main()