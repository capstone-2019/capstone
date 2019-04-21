#ifndef _INDUCTOR_H_
#define _INDUCTOR_H_

#include <vector>
#include <string>
#include <unordered_map>
#include <components/component.hpp>
#include <linsys.hpp>


class Inductor : public Component
{
public:
    static constexpr const char *IDENTIFIER = "INDUCTOR";
    Inductor(const std::vector<std::string>& tokens);
    ~Inductor() { }
    std::string to_string() override;
    std::vector<std::string> unknowns() override;
    void map_unknowns(std::unordered_map<std::string, int> mapping) override;
    void add_contribution(LinearSystem& sys,
                          Eigen::VectorXd& soln,
                          Eigen::VectorXd& prev_soln,
                          double dt) override;
private:
    std::string name;
    int npos;            /**< Positive terminal */
	int nneg;            /**< Negative terminal */
	double inductance;  /**< inductance in henrys */
	int n1; /**< Matrix index for unknown npos voltage */
	int n2; /**< Matrix index for unknown nneg voltage */
    int ni;
};

#endif /* _INDUCTOR_H_ */
