#ifndef STATE_H
#define STATE_H

#include <map>
#include <vector>
#include <string>
#include <memory>

class State;

typedef std::shared_ptr<State> pState;

class State
{
public:
	bool isAcceptance = false;
	std::string acceptanceType = "";

	// Constructors
	State();                                           // Default Constructor
	State(const State& n);                             // Copy Constructor
	~State();                                          // Destructor

	// Operators
	State& operator= (const State& n);                 // Assignment Operator

	// Functions
	std::map<std::string, std::vector<pState>>& getChildren();
	void addChild(pState n, const std::string& val);
	std::vector<pState> getChildren(const std::string& val);
	int getId() const;

private:
	int id;
	std::map<std::string, std::vector<pState>> children;
};

inline std::map<std::string, std::vector<pState>>& State::getChildren()
{
	return children;
}

inline int State::getId() const { return id; }

#endif