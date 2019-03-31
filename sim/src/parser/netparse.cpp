#include <string>
#include <string.h>
#include <vector>
#include <memory>
#include <assert.h>
#include <parser/netparser.hpp>

using std::vector;
using std::string;

/**
 * @brief Tokenize a string based on a given set of delimiting characters.
 *
 * @param s The string to tokenize
 * @param delims The delimiting characters. Defaults to a single space.
 *
 * @return A vector of the tokens found in the input string.
 */
vector<string> tokenize(const char *s, const char *delims) 
{
    vector<string> tokens;
    char *tmp = strdup(s);
    assert(tmp != NULL);
        
    char *token = strtok(tmp, delims);
    while (token != NULL) {
        tokens.push_back(token);
        token = strtok(NULL, delims);
    }

    free(tmp);
    return tokens;
}

/**
 * @brief Constructs a net netlist iterator instance.
 * @param netfile The file containing the netlist to iterate over.
 */
NetlistIterator::NetlistIterator(const char *netfile)
{
    string line;
    std::ifstream infile = std::ifstream(netfile);
    while (getline(infile, line)) {
        lines.push_back(line);
    }
}

/**
 * @brief Destroys a netlist iterator object
 */
NetlistIterator::~NetlistIterator()
{
}

