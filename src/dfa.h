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
	//int acceptancePriority; // For minimization

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
	inline const std::set<pState>& getStates() const { return states; }
	inline void print() { for (pState s : states) std::cout << s->getId() << " "; std::cout << std::endl; }
private:
	unsigned int id;
	std::set<pState> states;
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
	void minimize();
	bool inDFA(pDFAState& state);
	void printTransitionTable(const std::string& path);

private:
	DFA();
	~DFA();

	std::set<char> inputs;
	// For Minimization
	pDFAState deadState;

	int getPartitionIndex(pDFAState d, const std::vector<std::set<pDFAState> >& partitions);
	bool matchedNextStates(pDFAState d1, pDFAState d2, const std::vector<std::set<pDFAState> >& all);
	void replaceWithMinimized(const std::vector<std::set<pDFAState> >& partitions);

};

#endif