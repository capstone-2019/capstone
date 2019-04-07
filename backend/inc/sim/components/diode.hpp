#ifndef _DIODE_H_
#define _DIODE_H_

#include <vector>
#include <string>
#include <unordered_map>
#include <components/component.hpp>
#include <linsys.hpp>

class Diode : public Component
{
public:
    /** @brief String appearing in netlist to identify new diode */
    static constexpr const char *IDENTIFIER = "DIODE";

    /* construct diode from tokens in netlist file */
    Diode(const std::vector<std::string>& tokens);

    /**
     * @brief Destroys a diode.
     */
    ~Diode() { }

    std::string to_string() override;
    std::vector<std::string> unknowns() override;
    void map_unknowns(std::unordered_map<std::string, int> mapping) override;
    void add_contribution(LinearSystem& sys,
                          Eigen::VectorXd& soln,
                          Eigen::VectorXd& prev_soln,
                          double dt) override;
private:
    int npos;
    int nneg;

    int n1;
    int n2;

    double N;
    double IS;
    double VT;
};

#endif /* _DIODE_H_ */
