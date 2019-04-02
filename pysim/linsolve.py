import numpy as np
import sys
import copy

class LinearSystem(object):
    """
    Represents a linear system of equations. In the context of the circuit
    simulator, this represents the equations derived from applying a KCL,
    plus additional constraints such as Vsrc - Vgnd = 0 (assuming the source
    is directly connected to ground).
    """

    def __init__(self, unknowns, ground):
        """
        Constructs a new linear system of equations.

        unknowns: The set of all unknown currents or voltages to be solved
        for.

        ground: The ground node, which will be used to fix the voltages
        of all other notes relative to a known voltage (zero).
        """

        # save all the unknowns and construct a labels array storing their
        # human-readable names
        self.unknowns = unknowns
        self.total_unknowns = len(unknowns)
        self.labels = np.empty((self.total_unknowns, 1), dtype=object)
        for unknown, row in self.unknowns.items():
            self.labels[row][0] = unknown

        # zero out the LHS and RHS matrices
        self.lhs = np.zeros((self.total_unknowns, self.total_unknowns))
        self.rhs = np.zeros((self.total_unknowns, 1))

        # record our ground, and place a 1 in the correspinding place in the
        # LHS matrix (leaving the RHS matrix zero'd) to indicate that the
        # ground voltage is zero
        self.ground = int(ground)
        self.lhs[self.ground][self.ground] = 1

    def __repr__(self):
        """
        Produces a string representation of the linear system of equations.
        """
        s = "-" * 78 + "\n"
        s += "\nLinear System to Solve: \n\n"
        s += "LHS Matrix\n{}\n\nRHS Matrix\n{}\n\nUnknowns\n{}\n\n".format(
            self.lhs, self.rhs, self.labels)
        s += "-" * 78
        s += "\n"
        return s

    def solve(self):
        """
        Returns a solution to the linear system of equations. If the system
        has no solution, or if the system has an infinite number of solutions,
        this function will print the system for debugging and raise a
        LinalgError exception.
        """
        try:
            # solve system using gauss-jordan elimination
            return np.linalg.solve(self.lhs, self.rhs)
        except np.linalg.linalg.LinAlgError as e:
            # Failed to solve the system - log the matrices for debugging
            # and re-raise the exception
            exc_type, exc_val, exc_traceback = sys.exc_info()
            print("LinalgError ({}): System has no unique solution".format(e))
            print("The failing system was:")
            print(self)
            raise exc_type, exc_val, exc_traceback

    def add_to_lhs(self, r, c, delta):
        """
        Adds the contribution of a circuit component to the position
        (r, c) in the LHS matrix of a linear system of equations.
        """
        if r != self.ground:
            self.lhs[r][c] += delta

    def add_to_rhs(self, r, delta):
        """
        Adds the contribution of a circuit component to the position
        r in the RHS matrix of a linear system of equations.
        """
        if r != self.ground:
            self.rhs[r][0] += delta
