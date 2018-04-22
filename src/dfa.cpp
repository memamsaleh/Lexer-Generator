#include "dfa.h"
#include <queue>

#include <iostream>

using namespace std;

DFAState::DFAState(set<pState> nfastates)
{
	static unsigned int did = 0;
	id = did++;
	states = nfastates;
	isAcceptance = checkAcceptance();
}

DFAState::DFAState(const DFAState& d) : id(d.id), states(d.states), neighbours(d.neighbours), acceptanceType(d.acceptanceType), isAcceptance(d.isAcceptance)
{

}

DFAState::~DFAState() { }

DFAState& DFAState::operator= (const DFAState& d)
{
	if (this == &d)
		return (*this);

	id = d.id;
	states = d.states;
	neighbours = d.neighbours;
	acceptanceType = d.acceptanceType;
	isAcceptance = d.isAcceptance;

	return (*this);
}

bool DFAState::isEqualTo(pDFAState other)
{
	if (states.size() != other->states.size())
		return false;

	set<pState> others = other->states;

	for (pState s : states)
	{
		if (!inSet(s, others))
			return false;
	}

	return true;
}

bool DFAState::checkAcceptance()
{
	auto cmp = [](pair<int, string> left, pair<int, string> right) { return (left.first) < (right.first); };
	priority_queue<pair<int, string>, vector<pair<int, string> >, decltype(cmp)> acceptance(cmp);

	for(pState s : states)
	{
		if (s->isAcceptance)
		{
			pair<int, string> p(s->acceptancePriority, s->acceptanceType);
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
	//if (input == "\\L")
		//return pDFAState(*this);

	set<pState> newSet;
	set<pState> temp;
	for(pState s : states)
	{
		temp = s->getInputClosure(input);
		for(pState s2 : temp)
		{
			set<pState> temp2 = s2->getInputClosure("\\L");
			newSet.insert(temp2.begin(), temp2.end());
		}
	}

	return make_shared<DFAState>(DFAState(newSet));
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
	set<pState> epClosure = nfa->start->getInputClosure("\\L");
	start = make_shared<DFAState>(DFAState(epClosure));
	inputs = input;

	states.insert(start);
	pDFAState deadState = make_shared<DFAState>(DFAState(set<pState>()));
	std::map<pair<pDFAState, char>, pDFAState> temp;

	for(char c : input)
	{
		pair<pDFAState, char> key(deadState, c);
		pair<pair<pDFAState, char>, pDFAState> p(key, deadState);
		temp.insert(p);
	}

	queue<pDFAState> q;
	q.push(start);
	while(!q.empty())
	{
		pDFAState s = q.front();
		q.pop();
		for(char c : input)
		{
			pair<pDFAState, char> key(s, c);
			pDFAState newState = s->nextState(string(1, c));
			if (newState->isDead())
				newState = deadState;
			else
			{
				if (!inDFA(newState))
				{
					states.insert(newState);
					q.push(newState);
				}
			}
			temp.insert(pair<pair<pDFAState, char>, pDFAState>(key, newState));
		}
	}
	transitionTable = temp;
}

bool DFA::inDFA(pDFAState state)
{
	bool repeated = false;
	for(pDFAState s : states)
	{
		if(s->isEqualTo(state))
		{
			repeated = true;
			state = s;
		}
	}
	return repeated;
}

void DFA::printTransitionTable()
{

}