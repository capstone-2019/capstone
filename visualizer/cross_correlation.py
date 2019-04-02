import numpy as np
from circuitSimulatorResults import CircuitSimulatorResults

def main():
	cso = CircuitSimulatorResults("../orchestral.cso", "../playback/results_file")
	cso.plot()

if __name__ == "__main__":
	main()