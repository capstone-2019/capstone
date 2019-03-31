#ifndef _VOLTAGE_OUT_H_
#define _VOLTAGE_OUT_H_

#include <vector>
#include <string>

class VoltageOut : public Component
{
public:

	static constexpr const char *IDENTIFIER = "VOLTAGE_OUT";
	VoltageOut(const std::vector<std::string>& tokens);
	~VoltageOut() { }

	std::string to_string() override;

private:
	int npos;
	int nneg;
};

#endif /* _VOLTAGE_OUT_H_ */