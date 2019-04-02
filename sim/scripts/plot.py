"""
plot.py - This script is a utility used by the circuit simulator
to plot the results of simulation.

Data is fed into this script via a pipe which is written into by
the circuit simulator.
"""

import sys
import numpy as np
import matplotlib.pyplot as plt

def main():
    timescale = []
    input_signal = []
    output_signal = []

    for line in sys.stdin:
        t, vin, vout = map(float, line.split())
        timescale.append(t)
        input_signal.append(vin)
        output_signal.append(vout)

    timescale = np.array(timescale)
    input_signal = np.array(input_signal)
    output_signal = np.array(output_signal)

    plt.subplot(1, 2, 1)
    plt.plot(timescale, input_signal)
    plt.subplot(1, 2, 2)
    plt.plot(timescale, output_signal)
    plt.show()

if __name__ == '__main__':
    main()
