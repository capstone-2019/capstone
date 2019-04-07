/**
 *
 * @file component.hpp
 *
 * @date March 31, 2019
 *
 * @brief This file defines the interface between the components module and
 * the rest of the circuit simulator.
 *
 * Including this header in another source file will give the programmer
 * access to the Component type as well as all derived types such as Resistor,
 * Capacitor, VoltageIn, and VoltageOut.
 *
 * @bug As more component types become supported, their headers need to be
 * #included here.
 *
 * @author Matthew Kasper (mkasper@andrew.cmu.edu)
 *
 */

#ifndef _COMPONENT_H_
#define _COMPONENT_H_

#include <components/component_type.hpp>
#include <components/resistor.hpp>
#include <components/capacitor.hpp>
#include <components/voltagein.hpp>
#include <components/voltageout.hpp>
#include <components/diode.hpp>

#endif /* _COMPONENT_H_ */
