#ifndef STATE_H
#define STATE_H

#include <map>
#include <set>
#include <vector>
#include <string>
#include <algorithm>
#include <memory>

class State;

typedef std::shared_ptr<State> pState;

class State
{
public:
	bool isAcceptance = false;
	std::string acceptanceType = "";
	int acceptancePriority = -1;

	// Constructors
	State();                                           // Default Constructor
	State(const State& n);                             // Copy Constructor
	~State();                                          // Destructor

	// Operators
	State& operator= (const State& n);                 // Assignment Operator

	// Functions
	std::map<std::string, std::set<pState> >& getChildren();
	void addChild(pState n, const std::string& val);
	std::set<pState> getInputClosure(const std::string& val);
	unsigned int getId() const;

private:
	unsigned int id;
	std::map<std::string, std::set<pState> > children;
};

inline std::map<std::string, std::set<pState> >& State::getChildren()
{
	return children;
}

inline unsigned int State::getId() const { return id; }

bool inSet(pState ps, std::set<pState> pset); // A problem with pointers and the < operator (This is a temporary fix)

#endif