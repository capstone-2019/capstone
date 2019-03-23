# Circuit Simulator

This module contains the basics of a circuit simulator inspired by well-known simulators like SPICE. This module contains one script which runs the main simulator, `sim.py`. It also contains an auxilary script to do things such as generate test waveforms: `waveform.py`.

## Simulator Usage

The `sim.py` script expects a single command line argument: the netlist file of the circuit to simulate. The expected usage is:

```
./sim.py -c <path-to-netlist>
```

This will launch the simulator, which will perform a transient analysis of the specified circuit over the signal connected to the input voltage in the netlist file. Once the transient analysis is complete, the input and output signals will be plotted for the user to view.

## Waveform Generator Usage

The `waveform.py` script generates waveforms for use in the testing process. The expected usage is as follows:

```
./waveform.py [-h] [-f F] [-s S] [-e E] [-a A] [-p P] [-o O] [--plot]

optional arguments:
  -h, --help  show this help message and exit
  -f F        Waveform to generate [sin/square/noise]
  -s S        Start time for waveform timescale
  -e E        End time for waveform timescale
  -a A        Desired amplitude
  -p P        Desired period
  -o O        Output file (default = STDOUT)
  --plot      Plot (rather than print) the waveform
```

For example, to generate a sine wave with an amplitude of 5V and a period of 10s, sampled from t=0 to t=20 and store the results in waves/sin10.txt, you could use:

```
./waveform.py -f sin -p 10 -s 0 -e 20 -o waves/sin10.txt
```

## Netlist Format

The netlist specifies the properties of the circuit to simulate, and ends in a `.nls` file extension. 

#### Voltage Sources
The netlist must specify the input voltage source. Currently, the way an input voltage source is specified in a netlist is as follows:

```
VOLTAGE_IN <name> <node+> <node-> <signal-file>
```

The signal file is a file containing a series of comma separated pairs, where the first element is a timestamp, and the second element is a voltage. The `<node+>` and `<node->` fields specify which nodes in the circuit the source is connected across.

#### Voltage Outputs
The netlist must also specify the output voltage to record. This is done by including a line like the following:

```
VOLTAGE_OUT name <node+> <node->
```

This will cause the output signal to be measured across the nodes `<node+>` to `<node->`.

#### Ground
The netlist is responsible for telling the simulator which node to use as a reference when measuring all other nodal voltages. This is done by including the following line in the netlist:

```
GROUND <node-id>
```

#### Resistors
Resistors can be specified in a netlist by providing the ids of the nodes spanned by the device as well as the resistance. This is done by adding a line like the following to the netlist:

```
RESISTOR <name> <node+> <node-> <resistance>
```

#### Capacitors
Capacitors can be specified in a netlist by providing the ids of the nodes spanned by the device as well as the capacitance. This is done by adding a line like the following to the netlist:

```
CAPACITOR <name> <node+> <node-> <capacitance>
```

#### Specifying Component Values
The netlist parser will understand common unit prefixes when parsing component values. The full list of supported prefixes is:

<center>

|  Prefix |   Meaning	   |
|---	   |---	          |
|   `meg`	|  `1e6`      	|
|  `f` 	|  `1e-15`    	|
|  `p` 	|  `1e-12` 	   |
|  `n` 	|  `1e-9`     	|
|  `u` 	|  `1e-6`	      |
|  `m`  	|  `1e-3`	      |
|  `k` 	|  `1e3`      	|
|  `g` 	|  `1e9`      	|
|  `t` 	|  `1e12` 	   |

</center>

For example, the line `RESISTOR r1 0 1 100k` specifies a 100 kohm resistor spanning node 0 to node 1.

#### Comments in Netlist Files

Comments are supported in netlist files using the `#` symbol. This symbol can begin a comment at the beggining of a new line, or in the middle of a line, in which case anything coming after the `#` symbol will be ignored by the parser.

## Future Work

This simulator is still a work in progress. Next steps involve adding support for diodes and transistors, as well as working out some bugs in the inductor model.
