/**
 *
 * @file netparser.hpp
 *
 * @date March 31, 2019
 *
 * @brief This file provides the API by which the rest of the circuit
 * simulator can parse netlist files.
 *
 * The NetlistParser class can be used to go from a netlist file to the
 * simulator's internal circuit representation.
 *
 * @bug No known bugs.
 *
 * @author Matthew Kasper (mkasper@andrew.cmu.edu)
 *
 */

#ifndef _NETPARSER_H_
#define _NETPARSER_H_

#include <vector>
#include <string>
#include <fstream>
#include <components/component.hpp>

/****************************************************************************
 *                             NetlistIterator                              *
 ****************************************************************************/

/**
 * @brief Class to provide a nice interface for iterating over the contents
 * of a netlist. Allows users to iterate line by line over a netlist file,
 * with things like comments and extra whitespace automatically removed.
 */
class NetlistIterator
{
public:

    /* construct a netlist iterator */
    NetlistIterator(const char *netfile);
    /* destroy a netlist iterator */
    ~NetlistIterator() { };

    /** @brief Defines iterator type over lines in the netlist */
    typedef std::vector<std::string>::iterator iterator;
    /** @brief Defines const iterator type over lines in the netlist */
    typedef std::vector<std::string>::const_iterator const_iterator;

    /** @brief Returns an iterator referring to beginning of the netlist */
    iterator begin() { return lines.begin(); }
    /** @brief Returns an iterator referring to end of the netlist */
    iterator end() { return lines.end(); }
    /** @brief Returns a const iterator to the beginning of the netlist */
    const_iterator cbegin() { return lines.cbegin(); }
    /** @brief Returns a const iterator to the end of the netlist */
    const_iterator cend() { return lines.cend(); }

private:
    /** @brief the symbol used to indicate comments in a netlist */
    static constexpr const char *COMMENT_SYMBOL = "#";

    /* removes extra whitespace/comments from a netlist file line */
    static std::string clean_line(const std::string& line);
    /* removes comments from a line */
    static std::string remove_comments(const std::string& line);
    /* removes extra whitespace from a line */
    static std::string fix_whitespace(const std::string& line);

    /** @brief A vector of each line in the netlist file */
    std::vector<std::string> lines;
};

/****************************************************************************
 *                              NetlistParser                               *
 ****************************************************************************/

class NetlistParser
{
public:
    NetlistParser(const char *netfile);
    ~NetlistParser();
    std::vector<Component*> get_components();
private:
    std::vector<Component*> components;
    static Component *component_from_tokens(std::vector<std::string>& tokens);
};

#endif /* _NETPARSER_H_ */
