#ifndef _VOLTAGE_IN_H_
#define _VOLTAGE_IN_H_

#include <vector>
#include <string>

class VoltageIn : public Component
{
public:
	static constexpr const char *IDENTIFIER = "VOLTAGE_IN";

	VoltageIn(const std::vector<std::string>& tokens);
	~VoltageIn() { }
	std::string to_string() override;
private:
	int npos;
	int nneg;
	std::string signal_file;
};

#endif /* _VOLTAGE_IN_H_ */