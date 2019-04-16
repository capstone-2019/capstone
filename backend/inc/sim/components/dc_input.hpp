/**
 *
 * @file dc_input.hpp
 *
 * @author Matthew Kasper (mkasper@andrew.cmu.edu)
 *
 * @brief This file contains the interface for DC voltage sources.
 * These types of sources differ a bit from VoltageIn instances, because
 * there can be several of these in the same circuit.
 *
 * @date 04-16-2019
 *
 */

#ifndef _DC_INPUT_H_
#define _DC_INPUT_H_

#include <vector>
#include <string>
#include <unordered_map>
#include <linsys.hpp>
#include <components/component_type.hpp>

class DCInput : public Component
{
public:

    /**
     * @brief Identifier to represent a DC Voltage input in a circuit netlist
     * file.
     */
    static constexpr const char *IDENTIFIER = "DC_INPUT";

    /* Construct DC Voltage Input from netlist tokens */
    DCInput(const std::vector<std::string>& tokens);

    /* Destroy a DC Voltage Input */
    ~DCInput() { }

    /* Convert a DC Input to a string */
    std::string to_string() override;

    /* Get the unknowns associated with the DC voltage input */
    std::vector<std::string> unknowns() override;

    /* Map from string representations of unknowns to locations in the
     * unknown vector for transient analysis */
    void map_unknowns(std::unordered_map<std::string, int> mapping) override;

    /* Add KCL contributions of this component into linear system of KCL
     * equations used in transient analysis */
    void add_contribution(LinearSystem& sys,
                          Eigen::VectorXd& soln,
                          Eigen::VectorXd& prev_soln,
                          double dt) override;
private:
    std::string name;  /**< Name of this source */
    int npos;          /**< Positive terminal of the DC voltage source */
    int nneg;          /**< Negative terminal o fhte DC voltage source */
    double V;          /**< Voltage measured across terminals of DC source */
    int n1;            /**< Unknown matrix location for V(npos) */
    int n2;            /**< Unknown matrix loctaion for V(nneg) */
    int ni;            /**< Unknown matrix location for source current */
};

#endif /* _DC_INPUT_H_ */
