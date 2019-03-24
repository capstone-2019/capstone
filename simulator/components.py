import itertools
import numpy as np
import sys

class Component(object):
    """
    Base class that contains common functionality for all circuit components.
    """

    def __init__(self, line):
        """
        Performs common initialization steps for all components, such as
        parsing the component_type and retrieving the component name, which
        are always the first two tokens in any netlist file's component
        description.
        """
        tokens = line.split()
        self.component_type = tokens[0]
        self.name = tokens[1]

    def __repr__(self):
        """
        Converts component to string representation containing the
        name of the component as well as the component type.
        """
        return "{}: {}".format(self.name, self.component_type)

    def __str__(self):
        """
        Converts component to string representation containing the
        name of the component as well as the component type.
        """
        return "%r" % self

    def get_nodes(self):
        """
        Base class implementation of the get_nodes method. By default, do
        not assume that a component has any nodes.
        """
        return []

    def get_name(self):
        """
        Retrieves the name of a component as specified by the user netlist.
        """
        return self.name

    @classmethod
    def unknown_voltage(cls, node_name):
        return "voltage_node_{}".format(node_name)

    @classmethod
    def unknown_current(cls, branch_name):
        return "unknown_current_{}".format(branch_name)

    @classmethod
    def parse_value(self, val, unit_mappings=None):
        """
        Values in netlist files may optinonally contain unit suffixes. This
        method takes in a string consisting of a numeric value, optionally
        followed by a unit suffix. It returns a numeric representation of the
        string. For example: '100meg' will be parsed to 100 * 10^6.

        Callers may optionally provide their own unit mappings, which should
        be a list of tuples of the form (unit_suffix, numeric_scaling_factor).
        If this is not provided, a set of default units will be used instead.
        """
        default_units = [
            ('meg', 1e6),
            ('f',   1e-15),
            ('p',   1e-12),
            ('n',   1e-9),
            ('u',   1e-6),
            ('m',   1e-3),
            ('k',   1e3),
            ('g',   1e9),
            ('t',   1e12),
        ]

        units = default_units if unit_mappings is None else unit_mappings
        unit_scale = 1.0

        for unit, factor in units:
            if unit in val:
                unit_scale = factor
                break

        numeric_part = "".join(itertools.takewhile(str.isdigit, val))
        return float(numeric_part) * unit_scale

    @staticmethod
    def from_netlist_description(line):
        """
        Takes a line in a netlist description which may correspond to a
        componentand returns a corresponding component object constructed using
        the parameters from the netlist file if successful. If the line does
        not correspond to a valid component, this function returns None.
        """
        component_classes = [
            VoltageIn,
            VoltageOut,
            Resistor,
            Capacitor,
            Ground,
        ]
        component_class_map = { c.IDENTIFIER : c for c in component_classes }
        tokens = line.split()
        component_type = tokens[0]
        component_cls = component_class_map.get(component_type)
        return None if component_cls is None else component_cls(line)

    def unknowns(self):
        """
        Default implementation of the unknowns() method. This should be
        overriden in derived classes and eventually removed once every
        derived class implements their own unknwons() methods.
        """
        return []

    def add_matrix_contribution(self, system, soln, prev_soln, dt, time):
        """
        Default implementation of the add_matrix_contribution() method. This
        should be overriden in derived classes and eventually removed once
        every derived class implements their own add_matrix_contribution()
        methods.
        """
        pass

class Resistor(Component):
    """
    Class representing a resistor, which is a type of passive, linear
    component.

    A resistor is described in a netlist file using the following syntax:
    RESISTOR <name> <n+> <n-> <resistance>
    """

    IDENTIFIER = "RESISTOR"

    def __init__(self, line):
        """
        Initializes a resistor by performing the default component
        initialization, then further parsing out the node IDs and the
        associated resistance.
        """
        super(Resistor, self).__init__(line)
        tokens = line.split()
        self.node_pos = tokens[2]
        self.node_neg = tokens[3]
        self.resistance = self.parse_value(tokens[4])

    def __repr__(self):
        """
        Returns a string representation of a resistor containing the
        component name and the resistance.
        """
        return "{}: resistor of {} ohms".format(self.name, self.resistance)

    def get_nodes(self):
        """
        Returns a list of the nodes connected by this device, containing
        the negative node first followed by the positive node (this is
        a convention, but of course negative and positive are relative).
        """
        return [self.node_neg, self.node_pos]

    def add_matrix_contribution(self, system, soln, prev_soln, dt, time):
        """
        Computes and adds the contributions of this element to the
        system matrices.

        The current through a resistor is given by i = V/R by ohm's law.
        This means that for a resistor connecting N0 to N1, the current
        contribution into N1 is (V1 - V0) / R and the courrent contribution
        into N0 is -(V1 - V0) / R.
        """

        conductance = 1.0 / self.resistance

        unknown1, unknown2 = self.unknowns()
        n1, n2 = system.unknowns[unknown1], system.unknowns[unknown2]
        system.add_to_lhs(n1, n1, +conductance)
        system.add_to_lhs(n2, n2, +conductance)
        system.add_to_lhs(n1, n2, -conductance)
        system.add_to_lhs(n2, n1, -conductance)

        n1_rhs_contrib = (-prev_soln[n1][0] * conductance +
                           prev_soln[n2][0] * conductance)
        system.add_to_rhs(n1, n1_rhs_contrib)

        n2_rhs_contrib = -n1_rhs_contrib
        system.add_to_rhs(n2, n2_rhs_contrib)

    def unknowns(self):
        return (self.unknown_voltage(self.node_pos),
                self.unknown_voltage(self.node_neg))


class Capacitor(Component):
    """
    Class representing a capacitor, which is a passive energy storage
    component.

    A capacitor is descirbed in a netlist file using the following syntax:
    CAPACITOR <name> <n+> <n-> <capacitance>
    """

    IDENTIFIER = "CAPACITOR"

    def __init__(self, line):
        """
        Initializes a resistor by performing the default component
        initialization, then further parsing out the node IDs and the
        associated capacitance.
        """
        super(Capacitor, self).__init__(line)
        tokens = line.split()
        self.node_pos = tokens[2]
        self.node_neg = tokens[3]
        self.capacitance = self.parse_value(tokens[4])

    def __repr__(self):
        """
        Returns a string representation of a capacitor containing the
        component name and the capacitance.
        """
        return "{}: capacitor of {} farads".format(self.name, self.capacitance)

    def add_matrix_contribution(self, system, soln, prev_soln, dt, time):
        # Find unknowns that this component contributes to
        unknown1, unknown2 = self.unknowns()
        n1, n2 = system.unknowns[unknown1], system.unknowns[unknown2]

        # Contributions to the LHS of the system
        C = float(self.capacitance)
        system.add_to_lhs(n1, n1, +C/dt)
        system.add_to_lhs(n2, n2, +C/dt)
        system.add_to_lhs(n2, n1, -C/dt)
        system.add_to_lhs(n1, n2, -C/dt)

        # Contributions to the RHS of the system
        vt0 = prev_soln[n1][0] - prev_soln[n2][0]
        vt1 = soln[n1][0] - soln[n2][0]
        di = (vt1 - vt0) * (C / dt)
        system.add_to_rhs(n1, di)
        system.add_to_rhs(n2, -di)


    def get_nodes(self):
        """
        Returns a list of the nodes connected by this device, containing
        the negative node first followed by the positive node (this is
        a convention, but of course negative and positive are relative).
        """
        return [self.node_neg, self.node_pos]

    def unknowns(self):
        return (self.unknown_voltage(self.node_pos),
                self.unknown_voltage(self.node_neg))

    def get_nodes(self):
        return [self.node_neg, self.node_pos]

    def unknowns(self):
        return (self.unknown_voltage(self.node_pos),
                self.unknown_voltage(self.node_neg))

class VoltageIn(Component):
    """
    Class to represent an independent voltage input. As of now, the simulator
    only supports circuits with a single voltage input, and each input must be
    tied to a file that specifies the series of voltages to feed through the
    circuit.

    The netlist format is:
    VOLTAGE_IN <name> <n+> <n-> <input_file_name>
    """

    IDENTIFIER = "VOLTAGE_IN"

    def __init__(self, line):
        """
        Constructs a voltage input by performing default component
        initialization plus parsing out the positive and negative terminals
        as well as the input file name.
        """
        super(VoltageIn, self).__init__(line)
        tokens = line.split()
        self.node_pos = tokens[2]
        self.node_neg = tokens[3]
        self.input_file = sys.stdin
        meta = next(self.input_file).split(",")
        self.sampling_period = float(meta[0])
        self.t_start = 0

    def __repr__(self):
        """
        Create string representation of voltage input.
        """
        return "{}: Voltage Input from {}, spanning nodes {} -> {}".format(
            self.name, self.input_file, self.node_neg, self.node_pos)

    @classmethod
    def process_input_line(self, line):
        time, voltage = map(float, line.split(","))
        return time, voltage

    def timescale(self):
        return self.t_start, self.t_end

    def timestep(self):
        return self.sampling_period

    def __iter__(self):
        data = map(self.process_input_line, sys.stdin)
        for time, voltage in data:
            self.V = voltage
            self.t_end = time
            yield (time, voltage)


    def get_nodes(self):
        """
        Returns a list of the nodes connected by this device, containing
        the negative node first followed by the positive node (this is
        a convention, but of course negative and positive are relative).
        """
        return [self.node_neg, self.node_pos]

    def add_matrix_contribution(self, system, soln, prev_soln, dt, time):
        unknown_voltage, unknown_current = self.unknowns()
        node_current_id = system.unknowns[unknown_current]
        n1 = system.unknowns[self.unknown_voltage(self.node_pos)]
        n2 = system.unknowns[self.unknown_voltage(self.node_neg)]

        system.add_to_lhs(node_current_id, n1, +1)
        system.add_to_lhs(node_current_id, n2, -1)

        prev_soln_delta = prev_soln[n1][0] - prev_soln[n2][0]
        system.add_to_rhs(node_current_id, self.V - prev_soln_delta)

        system.add_to_lhs(n1, node_current_id, -1)
        system.add_to_lhs(n2, node_current_id, +1)

        system.add_to_rhs(n1, +prev_soln[node_current_id][0])
        system.add_to_rhs(n2, -prev_soln[node_current_id][0])

    def unknowns(self):
        return (self.unknown_voltage(self.node_pos),
                self.unknown_current(self.name))

class VoltageOut(Component):
    """
    Class to represent a voltage output, which is the value that will
    be measured by the simulator. As of now, the circuit supports just a
    single voltage output. This component can optionally be tied to an output
    file, where all the simulated measurements will be logged as the simulation
    advances.

    The netlist format is:
    VOLTAGE_OUT <name> <n+> <n-> [output_file_name]
    """

    IDENTIFIER = "VOLTAGE_OUT"

    def __init__(self, line):
        """
        Constructs a voltage output by performing default component
        initialization plus parsing out the positive and negative terminals
        as well as the optional output file name.
        """
        super(VoltageOut, self).__init__(line)
        tokens = line.split()
        self.node_pos = tokens[2]
        self.node_neg = tokens[3]
        self.output_file = None if len(tokens) < 5 else tokens[4]

    def __repr__(self):
        """
        Create string representation of voltage output.
        """
        return "{}: Voltage Output, spanning nodes {} -> {}".format(
            self.name, self.node_neg, self.node_pos)

    def get_nodes(self):
        """
        Returns a list of the nodes connected by this device, containing
        the negative node first followed by the positive node (this is
        a convention, but of course negative and positive are relative).
        """
        return [self.node_neg, self.node_pos]

    def terminals(self):
        return (self.unknown_voltage(self.node_pos),
                self.unknown_voltage(self.node_neg))


class Ground(Component):

    IDENTIFIER = "GROUND"

    def __init__(self, line):
        tokens = line.split()
        self.node_id = tokens[1]

    def __repr__(self):
        return "GND"
