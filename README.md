# 18-500: Stairway to Hamerschlag

This repository contains the source code for our 18-500 capstone design project, Stairway to Hamerschlag. The purpose of this oproject is to allow musicians to perform signal transformations against live (from an electric guitar) or recorded audio. It consists of three main components:

- A frontend that allows users to select from pre-built effect blocks or build their own circuits to perform audio signal transformations.
- A circuit simulator, which simulates the effects of users' circuits on audio signals as they pass through in real-time.
- An audio processor, which handles reading in live or recorded audio, converting it to a format that can be fed to the circuit simulator. This module also handles playing back the transformed audio signal after it passes through the circuit simulator.

## Front End

- TODO: Stephen fill this in.

## Circuit Simulator

The circuit simulator is made up of two distinct submodules: the netlist parser and the simulation engine. The netlist parser converts users' circuits to the simulator's internal representation, and the simulation engine models the effect a circuit would have on an input audio signal.

### Netlist Parser

The netlist parser transforms files describing user circuits into the simulator's internal format. The netlist file contains a sequence of lines, each denoting a single component or feature of the circuit. These files have `*.nls` file extensions.

#### Voltage Input Signals

An input signal is hooked into a circuit by adding the following line to a netlist:

```
VOLTAGE_IN <name> <node(+)> <node(-)>
```
At most one `VOLTAGE_IN` should be specified in a single netlist.

#### Voltage Outputs

A voltage output describes which nodes the output voltage should be measured across. This determines the signal that will be fed to the audio processor for playback. The format is:

```
VOLTAGE_OUT <name> <node(+)> <node(-)>
```
At most one `VOLTAGE_OUT` should be specified in a single netlist.

#### Resistors

Resistors are characterized by the nodes connected by the device as well as the resistance. The format is:

```
RESISTOR <name> <node(+)> <node(-)> <resistance>
```

#### Capacitors
Capacitors are characterized by the nodes connected by the device as well as the capacitance. The format is:

```
CAPACITOR <name> <node(+)> <node(-)> <capacitance>
```

#### Diodes

Diodes are characterized by the nodes they span. To include a diode in the circuit, the format is:

```
DIODE <name> <node(+)> <node(-)>
```
Diodes will allow current to flow from `<node(+)>` to `<node(-)>` when the voltage threshold is met across the nodes, but will resist current in the reverse direction.

### Simulation Engine

- TODO: Matt fill this in.



## Audio Processor

- TODO: Joeph fill this in.