#ifndef DFA_H
#define DFA_H

#include <set>
#include <vector>
#include <iostream>
#include "thompsonConstruction.h"

class DFAState;

typedef std::shared_ptr<DFAState> pDFAState;

class DFAState
{
public:
	std::string acceptanceType;
	bool isAcceptance;

	DFAState(std::set<pState> nfastates);
	DFAState(const DFAState& d);
	~DFAState();

	DFAState& operator= (const DFAState& d);

	inline unsigned int getId() { return id; }
	inline bool isDead() { return states.empty(); }
	//inline bool isEqualTo(pDFAState other) { return states == other->states; }
	bool isEqualTo(pDFAState other); // A problem with pointers and the < operator (This is a temporary fix)
	bool checkAcceptance();
	pDFAState nextState(const std::string& input);
	inline void print() { for (pState s : states) std::cout << s->getId() << " "; std::cout << std::endl; }
private:
	unsigned int id;
	std::set<pState> states;
	std::map<std::string, pDFAState> neighbours;
};

class DFA
{
public:
	std::set<pDFAState> states;
	pDFAState start;
	std::map<std::pair<pDFAState, char>, pDFAState> transitionTable;

	static DFA& getDFA();

	DFA(const DFA&) = delete;
	void operator=(const DFA&) = delete;

	void createDFA(pAutomata nfa, std::set<char> input);
	bool inDFA(pDFAState state);
	void printTransitionTable();

private:
	DFA();
	~DFA();

	std::set<char> inputs;
};

#endif