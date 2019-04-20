/**
 *
 * @file netparse.cpp
 *
 * @date April 1, 2019
 *
 * @brief Contains the implementation of the netlist parser, which is used
 * to go from the netlist file representation of a circuit to the simulator's
 * internal format.
 *
 * @author Matthew Kasper (mkasper@andrew.cmu.edu)
 *
 */

#include <string>
#include <string.h>
#include <vector>
#include <memory>
#include <assert.h>
#include <parser/netparser.hpp>
#include <iostream>
#include <components/component.hpp>
#include <errors.hpp>
#include <sim.hpp>
#include <stdio.h>

using std::vector;
using std::string;

static const char *effect_idenfifiers[] = {
    "REVERB",
    "FUZZ",
    NULL,
};

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

static AudioManager::filetype_t get_filetype(const string& fname) {
    string extension = fname.substr(fname.find_last_of(".") + 1);
    if (extension == "wav" || extension == "ogg")
        return AudioManager::FILETYPE_WAV;
    return AudioManager::FILETYPE_TXT;
}

static bool is_effect_block(const string& s) {
    for (int i = 0; effect_idenfifiers[i] != NULL; i++) {
        if (s == effect_idenfifiers[i]) return true;
    }
    return false;
}

static vector<string> get_effect_blocks(NetlistIterator& ni) {
    vector<string> blocks;
    for (auto it = ni.begin(); it != ni.end(); ++it) {
        vector<string> tokens = tokenize(*it);
        if (is_effect_block(tokens[0]))
            blocks.push_back(*it);
    }
    return blocks;
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
 *                          NetlistParser Helpers                           *
 ****************************************************************************/



static AudioManager::input_t get_input_source(simparams_t *params) {

    if (params->signal_file != NULL) {
        return AudioManager::INPUT_FILE;
    } else if (params->live_input) {
        return AudioManager::INPUT_HARDWARE;
    } else {
        sim_error("invalid input parameters\n");
        return AudioManager::INPUT_FILE;
    }
}

static AudioManager::output_t get_outputs(simparams_t *params) {

    AudioManager::output_t ret = 0;

    if (params->outfile != NULL) {
        ret |= AudioManager::OUTPUT_FILE;
    }

    if (params->live_output) {
        ret |= AudioManager::OUTPUT_HARDWARE;
    }

    return ret;
}
/****************************************************************************
 *                              NetlistParser                               *
 ****************************************************************************/

/**
 * @brief Constructs a netlist parser.
 *
 * @param netfile The file containing the circuit netlist description.
 */
NetlistParser::NetlistParser(simparams_t *params) {

    const char *netfile = params->circuit_file;
    const char *sigfile = params->signal_file;
    const char *outfile = params->outfile;

    NetlistIterator ni (netfile);

    /**
     * Construct the AudioManager for the circuit simulation.
     *
     * @bug THIS WILL NOT WORK FOR LIVE AUDIO.
     */
    AudioManager::input_t input_source = get_input_source(params);
    AudioManager::output_t output_source = get_outputs(params);
    AudioManager::filetype_t filetype = input_source == AudioManager::INPUT_FILE ?
                                        get_filetype(sigfile) :
                                        AudioManager::FILETYPE_NONE;


    vector<string> effect_blocks = get_effect_blocks(ni);

    this->am = new AudioManager(
        input_source,
        output_source,
        sigfile,
        outfile,
        filetype,
        effect_blocks);

    input_signal_file = sigfile;

    for (auto it = ni.begin(); it != ni.end(); it++) {
        const string& line = *it;
        vector<string> tokens = tokenize(line);
        if (tokens[0] == "GROUND") {
            ground_id = stoi(tokens[1]);
        }
        else {
            Component *c = component_from_tokens(tokens);
            if (c != NULL) {
                components.push_back(c);
            }
        }
    }

    c.register_ground(ground_id);
}

/**
 * @brief Destroys the netlist parser, freeing all allocated resources.
 *
 * @bug Should use smart pointers to make sure components get free'd.
 */
NetlistParser::~NetlistParser() {
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

    /* resistor */
    if (tokens[0] == Resistor::IDENTIFIER) {
        Resistor *res = new Resistor(tokens);
        c.register_resistor(res);
        return res;
    }

    /* capacitor */
    else if (tokens[0] == Capacitor::IDENTIFIER) {
        Capacitor *cap = new Capacitor(tokens);
        c.register_capacitor(cap);
        return cap;
    }

    /* input voltage */
    else if (tokens[0] == VoltageIn::IDENTIFIER) {
        VoltageIn *vin = new VoltageIn(tokens, am);
        c.register_vin(vin);
        return vin;
    }

    /* output voltage */
    else if (tokens[0] == VoltageOut::IDENTIFIER) {
        VoltageOut *vout = new VoltageOut(tokens, am);
        c.register_vout(vout);
        return vout;
    }

    else if (tokens[0] == Diode::IDENTIFIER) {
        Diode *d = new Diode(tokens);
        c.register_diode(d);
        return d;
    }

    /* bad identifier */
    else {
        return NULL;
    }
}
