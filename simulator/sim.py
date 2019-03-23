#!/usr/bin/env python2

from argparse import ArgumentParser
from exceptions import IOError
from netlist import Netlist
import sys
from utils import plot
import matplotlib.pyplot as plt

def main():
	"""
	Main function to drive the simulator. The expected usage is:
	./sim.py -c <path to circuit netlist>
	"""

	# set up argument parser
	parser = ArgumentParser()
	parser.add_argument('-c', help='Circuit netlist file', required=True)

	try:
		# extract command line arguments
		args = parser.parse_args()
		netlist = Netlist(args.c)
		circuit = netlist.as_circuit()

		# solve the circuit at every timestamp for the input signal
		timescale, input_signal, vout = circuit.transient()
		t_start, t_end = circuit.timescale()

		# plot the results
		plt.subplot(1, 2, 1)
		plot(t_start, t_end, input_signal, title="Input Signal")
		plt.subplot(1, 2, 2)
		plot(t_start, t_end, vout, title="Output Signal")
		plt.show()

	except IOError as e:
		parser.print_help()
		print("\nIOError {}".format(e))
		sys.exit(-1)

if __name__ == '__main__':
	main()