#!/usr/bin/env python2

import math
import sys
import matplotlib.pyplot as plt
import numpy as np
from argparse import ArgumentParser

def high_freq_noise(t_start, t_end,
	nsamples=None,
	sample_rate=30,
	amplitude=1.0,
	period=None):
	"""
	Produces a random sample of high frequency noise. The data will be
	sampled from the standard normal distribution, and the size
	of the returned vector will be equal to nsamples if specified, otherwise
	it will be calculated from t_start, t_end, and the sampling_rate.
	"""

	if nsamples is None:
		nsamples = int(math.ceil((t_end - t_start) * sample_rate))
	sampling_period = float(t_end - t_start) / nsamples

	x = np.linspace(t_start, t_end, num=nsamples)
	y = amplitude * np.random.normal(0, 1, size=nsamples)
	return sampling_period, x, y


def sinusoid(t_start, t_end, period,
	nsamples=None,
	sample_rate=1000,
	amplitude=1.0):
	"""
	Produces a sinusoid of a specified amplitude and period, sampled at
	discrete intervals over the range [t_start, t_end].

	t_start: Beginning time of the sampled range
	t_end: End time of the sampled range
	period: Period of the sinusoid
	sample_rate: How many samples are taken in a timestep of size 1
	amplitude: Amplitude of the sinusoid
	"""

	if nsamples is None:
		nsamples = math.ceil((t_end - t_start) * sample_rate)
	sampling_period = float(t_end - t_start) / nsamples

	f = 1.0 / period
	w = 2 * np.pi * f
	x = np.linspace(t_start, t_end, num=nsamples)
	y = amplitude * np.sin(w * x)
	return sampling_period, x, y

def square(t_start, t_end, period,
	nsamples=None,
	sample_rate=1000,
	amplitude=1.0):
	"""
	Produces a square wave of a specified amplitude and period, sampled at
	discrete intervals over the range [t_start, t_end].

	t_start: Beginning time of the sampled range
	t_end: End time of the sampled range
	period: Period of the sinusoid
	sample_rate: How many samples are taken in a timestep of size 1
	amplitude: Amplitude of the sinusoid
	"""

	if nsamples is None:
		nsamples = math.ceil((t_end - t_start) * sample_rate)
	sampling_period = float(t_end - t_start) / nsamples

	f = 1.0 / period
	x = np.linspace(t_start, t_end, num=nsamples)
	period = int(math.ceil(period))
	pulse_width = period / 2
	y = x % period < pulse_width
	y = np.array([amplitude if sample else 0.0 for sample in y])
	return sampling_period, x, y

def main():
	"""
	Runs the waveform generator. This program accepts command line arguments
	to specify the start and end of the waveform's timescale, the amplitude,
	the period, the type of wave being generated, and the output file. By
	default, this program prints to stdout the contents of a waveform file
	that can be later used as a VoltageIn. However, by using the -o command
	line switch, this output can be captured in a specified file instead.
	Additionally, by using the --plot argument, no output will be generated,
	but the resulting waveform will instead be plotted using matplotlib.
	"""

	# parse command line arguments
	parser = ArgumentParser()
	parser.add_argument('-f', help='Waveform to generate [sin/square/noise]',
		default='sin')
	parser.add_argument('-s', help='Start time for waveform timescale',
		default=0.0, type=float)
	parser.add_argument('-e', help='End time for waveform timescale',
		default=10.0, type=float)
	parser.add_argument('-a', help='Desired amplitude', default=1.0,
		type=float)
	parser.add_argument('-p', help='Desired period', default=1.0,
		type=float)
	parser.add_argument('-o', help='Output file (default = STDOUT)',
		default=None)
	parser.add_argument('--plot', help='Plot (rather than print) the waveform',
		default=False, action='store_true')
	args = parser.parse_args()

	# args not specified at all - don't just fall back on defaults
	if len(sys.argv) == 1:
		parser.print_help()
		sys.exit(-1)

	# find which function the user wants to generate
	if args.f == 'sin':
		wave_fn = sinusoid
	elif args.f == 'square':
		wave_fn = square
	elif args.f == 'noise':
		wave_fn = high_freq_noise
	else:
		print("Unknown waveform type: {}".format(args.f))
		sys.exit(-1)

	# get waveform parameters
	start = args.s
	end = args.e
	period = args.p
	amplitude = args.a

	# generate waveform
	sp, x, y = wave_fn(start, end, period=period, amplitude=amplitude)

	# find output method
	f = sys.stdout
	if args.o is not None:
		f = open(args.o, 'a')
		f.truncate(0)
	output_fn = lambda s : f.write("{}\n".format(s))

	# If user requested fn to be plotted, do so.
	# Otherwise, just write the sampled function to the output file
	# or stdout if no output file was specified.
	if args.plot:
		plt.plot(x, y)
		plt.show()
	else:
		output_fn("{},{},{}".format(sp, start, end))
		for time, value in zip(x, y):
			output_fn("{},{}".format(time, value))

	# clean up any file descriptors left around
	if f is not sys.stdout:
		f.close()

if __name__ == '__main__':
	main()
