#ifndef _NETPARSER_H_
#define _NETPARSER_H_

#include <vector>
#include <string>
#include <fstream>

std::vector<std::string> tokenize(const char *s, const char *delims = " ");

class NetlistIterator
{
private:
    /** @brief A vector of each line in the netlist file */
    std::vector<std::string> lines;

public:

    /* construct a netlist iterator */
    NetlistIterator(const char *netfile);
    /* destroy a netlist iterator */
    ~NetlistIterator();

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
};

#endif /* _NETPARSER_H_ */
