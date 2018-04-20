#include "dfa.h"
#include <queue>
using namespace std;

unsigned int did = 0;

DFAState::DFAState(set<pState> nfastates)
{
	//static unsigned int sid = 0;
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

bool DFAState::checkAcceptance()
{
	auto cmp = [](pair<int, string> left, pair<int, string> right) { return (left.first) < (right.first); };
	priority_queue<pair<int, string>, vector<pair<int, string>>, decltype(cmp)> acceptance(cmp);

	set<pState>::iterator itr = states.begin();
	for (itr; itr != states.end(); ++itr)
	{
		pState s = *itr;
		if (s->isAcceptance)
		//if(itr->get()->isAcceptance)
		{
			pair<int, string> p(s->acceptancePriority, s->acceptanceType);
			//pair<int, string> p(itr->get()->acceptancePriority, itr->get()->acceptanceType);
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
	//if (input == "!")
		//return pDFAState(this);

	set<pState> newSet;
	set<pState> temp;
	//vector<pState> list;
	set<pState>::iterator itr = states.begin();
	for (itr; itr != states.end(); ++itr)
	{
		temp = itr->get()->getInputClosure(input);
		set<pState>::iterator itr2 = temp.begin();

		for (itr2; itr2 != temp.end(); ++itr2)
		{
			set<pState> temp2 = itr2->get()->getInputClosure("\\L");
			newSet.insert(temp2.begin(), temp2.end());
		}
		//newSet.insert(temp.begin(), temp.end());
	}



	//pDFAState d = make_shared<DFAState>(DFAState(newSet));
	/*
	if (!DFA::getDFA().inDFA(d))
	{
		neighbours.insert(pair<string, pDFAState>(input, d));
	}
	*/
	//pDFAState finalState = DFA::getDFA().clearRepetition(d);
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
	//vector<pState> children = nfa->start->getChildren("\\L");
	set<pState> epClosure = nfa->start->getInputClosure("\\L");
	//epClosure.insert(nfa->start);
	start = make_shared<DFAState>(DFAState(epClosure));
	inputs = input;

	states.insert(start);
	pDFAState deadState = make_shared<DFAState>(DFAState(set<pState>()));
	std::map<pair<pDFAState, char>, pDFAState> temp;
	set<char>::iterator itr = input.begin();
	for (itr; itr != input.end(); ++itr)
	{
		pair<pDFAState, char> key(deadState, *itr);
		pair<pair<pDFAState, char>, pDFAState> p(key, deadState);
		temp.insert(p);
	}

	queue<pDFAState> q;
	q.push(start);
	while(!q.empty())
	{
		pDFAState s = q.front();
		q.pop();
		set<char>::iterator itr = input.begin();
		for (itr; itr != input.end(); ++itr)
		{
			pair<pDFAState, char> key(s, *itr);
			pDFAState newState = s->nextState(string(1, *itr));
			if (newState->isDead())
				temp.insert(pair<pair<pDFAState, char>, pDFAState>(key, deadState));
			else
			{
				temp.insert(pair<pair<pDFAState, char>, pDFAState>(key, newState));
				if (!inDFA(newState))
				{
					states.insert(newState);
					q.push(newState);
				}
			}
		}
	}
	transitionTable = temp;
}

bool DFA::inDFA(pDFAState state)
{
	bool repeated = false;
	set<pDFAState>::iterator itr = states.begin();
	for (itr; itr != states.end(); ++itr)
	{
		if (*itr == state)
		{
			repeated = true;
		}
	}
	return repeated;
}

void DFA::printTransitionTable()
{

}