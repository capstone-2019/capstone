#include <string>
#include <string.h>
#include <vector>
#include <memory>
#include <assert.h>
#include <parser/netparser.hpp>
#include <iostream>
#include <components/component.hpp>

using std::vector;
using std::string;

/****************************************************************************
 *                          String Parsing Helpers                          *
 ****************************************************************************/

/**
 * @brief Tokenize a string based on a given set of delimiting characters.
 *
 * @param s The string to tokenize
 * @param delims The delimiting characters. Defaults to a single space.
 *
 * @return A vector of the tokens found in the input string.
 */
static vector<string> tokenize(const std::string& s, const char *delims = " ")
{
    vector<string> tokens;
    char *tmp = strdup(s.c_str());
    assert(tmp != NULL);

    char *token = strtok(tmp, delims);
    while (token != NULL) {
        tokens.push_back(token);
        token = strtok(NULL, delims);
    }

    free(tmp);
    return tokens;
}

/****************************************************************************
 *                             NetlistIterator                              *
 ****************************************************************************/

/**
 * @brief Constructs a net netlist iterator instance.
 *
 * @param netfile The file containing the netlist to iterate over.
 */
NetlistIterator::NetlistIterator(const char *netfile)
{
    string line;
    std::ifstream infile = std::ifstream(netfile);

    /* make sure that the provided file actually exists */
    if (!infile.good()) {
        std::cerr << "Parser Failure: "
                  << netfile
                  << " - No such file or directory."
                  << std::endl;
        exit(EXIT_FAILURE);
    }

    /* create a vector containing the cleaned contents of the file  */
    while (getline(infile, line)) {
        const string cleaned = clean_line(line);
        if (cleaned.size() > 0) {
            lines.push_back(cleaned);
            vector<string> tokens = tokenize(cleaned.c_str());
        }
    }
}

/**
 * @brief Cleans a line that may contain excess whitespace or comments.
 *
 * @param line The line to clean.
 *
 * @return A new string containing the same contents as the original line
 * with excess whitespace and comments removed.
 */
string NetlistIterator::clean_line(const string& line) {
    return fix_whitespace(remove_comments(line));
}

/**
 * @brief Removes excess leading/trailing whitespace from a string.
 *
 * @param line The string to trim.
 *
 * @bug Does not handle extra interior whitespace well.
 *
 * @return Contents of the original string with excess whitespace removed.
 */
string NetlistIterator::fix_whitespace(const string& line) {
    int start, end;

    /* find first non whitespace character in the line */
    for (start = 0; start < line.size(); start++) {
        if (!isspace(line[start]))
            break;
    }

    /* find last non whitespace character in the line */
    for (end = line.size() - 1; end >= 0; end--) {
        if (!isspace(line[end]))
            break;
    }

    /* entire line was whitespace */
    if (start == end)
        return "";

    int len = end - start + 1;
    return line.substr(start, len);
}

/**
 * @brief Removes comments from a line. Comments are defined to be anything
 * that comes after the COMMENT_SYMBOL (#) character in a line.
 *
 * @param line String to remove comments from.
 *
 * @return Contents of the input string but with comments removed.
 */
string NetlistIterator::remove_comments(const string& line) {
    std::size_t i = line.find_first_of(COMMENT_SYMBOL);
    if (i == string::npos)
        return line;
    return line.substr(0, i);
}

/****************************************************************************
 *                              NetlistParser                               *
 ****************************************************************************/

/**
 * @brief Constructs a netlist parser.
 *
 * @param netfile The file containing the circuit netlist description.
 */
NetlistParser::NetlistParser(const char *netfile) {
    NetlistIterator ni (netfile);
    for (auto it = ni.begin(); it != ni.end(); it++) {
        const string& line = *it;
        vector<string> tokens = tokenize(line);
        if (tokens[0] == "GROUND") {
            ground_id = stoi(tokens[1]);
        }
        else {
            components.push_back(component_from_tokens(tokens));
        }
    }
}

/**
 * @brief Destroys the netlist parser, freeing all allocated resources.
 *
 * @bug Should use smart pointers.
 */
NetlistParser::~NetlistParser() {
    for (Component *c : components) {
        delete c;
    }
}

/**
 * @brief Creates a component from a vector of tokens found on a single line
 * in a netlist.
 *
 * @param tokens The vector of tokens.
 *
 * @return A pointer to a newly constructed component.
 *
 * @bug Should use smart pointers.
 */
Component *NetlistParser::component_from_tokens(vector<string> &tokens) {
    if (tokens[0] == Resistor::IDENTIFIER) {
        Resistor *res = new Resistor(tokens);
        c.register_resistor(res);
        return res;
    }
    else if (tokens[0] == Capacitor::IDENTIFIER) {
        Capacitor *cap = new Capacitor(tokens);
        c.register_capacitor(cap);
        return cap;
    }
    else if (tokens[0] == VoltageIn::IDENTIFIER) {
        VoltageIn *vin = new VoltageIn(tokens);
        c.register_vin(vin);
        return vin;
    }
    else if (tokens[0] == VoltageOut::IDENTIFIER) {
        VoltageOut *vout = new VoltageOut(tokens);
        c.register_vout(vout);
        return vout;
    }
    else if (tokens[0] == "GROUND") {
        c.register_ground(stoi(tokens[1]));
        return NULL;
    } else {
        std::cerr << "Unrecognized token " << tokens[0] << std::endl;
        return NULL;
    }
}
