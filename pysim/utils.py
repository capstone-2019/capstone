import functools
import time
import numpy as np
import matplotlib.pyplot as plt

def efilter(predicate, iterable):
	"""
	An eagerly evaluated implementation of filter, equivalent to the
	filter behavior in Python 2.
	"""
	return [x for x in iterable if predicate(x)]

def timer_decorator(f):
	"""
	Decorator which prints the time taken to run the decorated function.
	"""
	@functools.wraps(f)
	def g(*args):
		start = time.time()
		result = f(*args)
		elapsed = time.time() - start
		print("Time Elapsed to run {}: {} seconds".format(f, elapsed))
		return result
	return g

def plot(t_start, t_end, samples, title=""):
	"""
	Plots a signal over the interval [t_start, t_end].
	samples: The signal to plot
	title: The plot title
	"""
	x = np.linspace(t_start, t_end, len(samples))
	y = np.array(samples)
	plt.plot(x, y, label=title)
	plt.legend()