#!/usr/bin/env python2

from argparse import ArgumentParser
from exceptions import IOError
from netlist import Netlist
import sys
from utils import plot
import matplotlib.pyplot as plt
from scipy.io import wavfile
import numpy as np
import sounddevice as sd

TARGET_AMPLITUDE = 80

def main():
	"""
	Main function to drive the simulator. The expected usage is:
	./sim.py -c <path to circuit netlist>
	"""

	# set up argument parser
	parser = ArgumentParser()
	parser.add_argument('-c', help='Circuit netlist file', required=True)
	parser.add_argument('-o', help='Output .wav file', default=None)

	try:
		# extract command line arguments
		args = parser.parse_args()
		netlist = Netlist(args.c)
		circuit = netlist.as_circuit()

		# solve the circuit at every timestamp for the input signal
		timescale, input_signal, vout = circuit.transient()
		vout = np.array(vout)
		t_start, t_end = circuit.timescale()

		# write data to output wavfile
		outfile = args.o
		if outfile is not None:
			rate = timescale[1] - timescale[0]
			fs = int(1.0 / rate)
			max_amp = np.max(np.abs(vout))
			sigf32 = (vout/max_amp).astype(np.float32)
			sd.play(sigf32, fs*100)
			# wavfile.write(outfile, fs, sigf32)

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