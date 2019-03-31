#ifndef _RESISTOR_H_
#define _RESISTOR_H_

#include <vector>
#include <string>

class Resistor : public Component
{
public:
	static constexpr const char *IDENTIFIER = "RESISTOR";

	Resistor(const std::vector<std::string>& tokens);
	~Resistor() { }

	std::string to_string() override;

private:
	int npos;
	int nneg;
	double resistance;
};

#endif /* _RESISTOR_H_ */