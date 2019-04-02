"""
plot.py - This script is a utility used by the circuit simulator
to plot the results of simulation.

Data is fed into this script via a pipe which is written into by
the circuit simulator.
"""

import sys
import numpy as np
import matplotlib.pyplot as plt


class Plotter(object):

    def __init__(self, infile_name=None):

        # Get the input data source
        if infile_name is None:
            infile = sys.stdin
            opened_infile = False
        else:
            infile = open(infile_name, 'r')
            opened_infile = True

        # collect the data from the input file
        times = []
        vins = []
        vouts = []
        for line in filter(lambda s: len(s) > 0, map(str.strip, infile)):
            t, vin, vout = line.split()
            times.append(t)
            vins.append(vin)
            vouts.append(vout)

        # convert signals to np arrays
        self.num_frames = len(times)
        self.sample_rate = 1.0 / (float(times[1]) - float(times[0]))
        self.t = np.array(times, dtype=np.float64)
        self.vin = np.array(vins, dtype=np.float64)
        self.vout = np.array(vouts, dtype=np.float64)
        fft_vin = abs(np.fft.fft(self.vin) / self.num_frames)
        self.fft_vin = fft_vin[range(self.num_frames / 2)]
        fft_vout = abs(np.fft.fft(self.vout) / self.num_frames)
        self.fft_vout = fft_vout[range(self.num_frames / 2)]

        # close input file if necessary
        if opened_infile:
            infile.close()

    def plot_time_domain(self, signal):
        plt.plot(self.t, signal)

    def plot_freq_doman(self, fft):
        k = np.arange(self.num_frames)
        T = self.num_frames / self.sample_rate
        frq = k / T
        frq = frq[range(self.num_frames / 2)]
        plt.plot(frq, fft)

    def show(self):
        plt.subplot(2, 2, 1)
        self.plot_time_domain(self.vin)
        plt.subplot(2, 2, 2)
        self.plot_time_domain(self.vout)
        plt.subplot(2, 2, 3)
        self.plot_freq_doman(self.fft_vin)
        plt.subplot(2, 2, 4)
        self.plot_freq_doman(self.fft_vout)
        plt.show()

def main():
    p = Plotter()
    p.show()

if __name__ == '__main__':
    main()
