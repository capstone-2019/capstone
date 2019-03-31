#ifndef _CAPACITOR_H_
#define _CAPACITOR_H_

#include <vector>
#include <string>

class Capacitor : public Component
{
public:
	static constexpr const char *IDENTIFIER = "CAPACITOR";

	Capacitor(const std::vector<std::string>& tokens);
	~Capacitor() { }

	std::string to_string() override;
private:
	int npos;
	int nneg;
	double capacitance;
};

#endif /* _CAPACITOR_H_ */
