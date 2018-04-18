#ifndef DFA_H
#define DFA_H

#include <set>
#include "thompsonConstruction.h"

class DFAState;

typedef std::shared_ptr<DFAState> pDFAState;

class DFAState
{
public:
	std::string acceptanceType;
	bool isAcceptance;

	DFAState(std::set<pState> nfastates);

	inline unsigned int getId() { return id; }
	inline bool isDead() { return states.empty(); }
	inline bool isEqual(pDFAState other) { return states == other->states; }
	bool checkAcceptance();
	pDFAState nextState(const std::string& input);
private:
	unsigned int id;
	std::set<pState> states;
	std::map<std::string, pDFAState> neighbours;
};

class DFA
{
public:
	std::vector<pDFAState> states;
	pDFAState start;
	std::map<std::pair<pDFAState, char>, pDFAState> transitionTable;

	static DFA& getDFA();

	DFA(const DFA&) = delete;
	void operator=(const DFA&) = delete;

	void createDFA(pAutomata nfa, std::set<char> input);
	pDFAState clearRepetition(pDFAState state);
	void printTransitionTable();

private:
	DFA();
	~DFA();

	std::set<char> inputs;
};

#endif