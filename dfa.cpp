#include "dfa.h"
#include <queue>
using namespace std;

DFAState::DFAState(set<pState> nfastates)
{
	static unsigned int sid = 0;
	id = sid++;
	states = nfastates;
	isAcceptance = checkAcceptance();
}

bool DFAState::checkAcceptance()
{
	auto cmp = [](pair<int, string> left, pair<int, string> right) { return (left.first) < (right.first); };
	priority_queue<pair<int, string>, vector<pair<int, string>>, decltype(cmp)> acceptance(cmp);

	set<pState>::iterator itr = states.begin();
	for (itr; itr != states.end(); ++itr)
	{
		if (itr->get()->isAcceptance)
		{
			pair<int, string> p(itr->get()->acceptancePriority, itr->get()->acceptanceType);
			acceptance.push(p);
		}
	}

	if (acceptance.empty())
		return false;

	acceptanceType = acceptance.top().second;
	return true;
}

pDFAState DFAState::nextState(const string& input)
{
	set<pState> newSet;
	vector<pState> list;
	set<pState>::iterator itr = states.begin();
	for (itr; itr != states.end(); ++itr)
	{
		vector<pState> temp = itr->get()->getChildren(input);
		list.insert(list.end(), temp.begin(), temp.end());
	}

	for (int i = 0; i < list.size(); i++)
	{
		newSet.insert(list[i]);
		vector<pState> temp = list[i]->getChildren("\\L");
		for (int j = 0; j < temp.size(); j++)
			newSet.insert(temp[j]);
	}

	pDFAState finalState = DFA::getDFA().clearRepetition(pDFAState(new DFAState(newSet)));
	neighbours.insert(pair<string, pDFAState>(input, finalState));
	return finalState;
}

DFA::DFA() { }

DFA& DFA::getDFA()
{
	static DFA instance;
	return instance;
}

DFA::~DFA() { }

void DFA::createDFA(pAutomata nfa, std::set<char> input)
{
	vector<pState> children = nfa->start->getChildren("\\L");
	set<pState> epClosure(children.begin(), children.end());
	epClosure.insert(nfa->start);
	start = pDFAState(new DFAState(epClosure));
	inputs = input;

	states.push_back(start);
	pDFAState deadState(new DFAState(set<pState>()));
	std::map<pair<pDFAState, char>, pDFAState> temp;
	set<char>::iterator itr = input.begin();
	for (itr; itr != input.end(); ++itr)
	{
		pair<pDFAState, char> key(deadState, *itr);
		pair<pair<pDFAState, char>, pDFAState> p(key, deadState);
		temp.insert(p);
	}

	for (int i = 0; i < states.size(); i++)
	{
		set<char>::iterator itr = input.begin();
		for (itr; itr != input.end(); ++itr)
		{
			pair<pDFAState, char> key(states[i], *itr);
			pDFAState newState = states[i]->nextState(string(1, *itr));
			if (newState->isDead())
				temp.insert(pair<pair<pDFAState, char>, pDFAState>(key, deadState));
			else
				temp.insert(pair<pair<pDFAState, char>, pDFAState>(key, newState));
		}
	}
	transitionTable = temp;
}

pDFAState DFA::clearRepetition(pDFAState state)
{
	bool repeated = false;
	for (int i = 0; i < states.size(); i++)
	{
		if (states[i]->isEqual(state))
		{
			state = states[i];
			repeated = true;
		}
	}
	if (!repeated)
		states.push_back(state);
	return state;
}

void DFA::printTransitionTable()
{

}