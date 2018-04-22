#include "dfa.h"
#include <queue>
#include <fstream>
#include <iostream>

using namespace std;

DFAState::DFAState(set<pState> nfastates)
{
	static unsigned int did = 0;
	id = did++;
	states = nfastates;
	isAcceptance = checkAcceptance();
}

DFAState::DFAState(const DFAState& d) : id(d.id), states(d.states), acceptanceType(d.acceptanceType), isAcceptance(d.isAcceptance)
{

}

DFAState::~DFAState() { }

DFAState& DFAState::operator= (const DFAState& d)
{
	if (this == &d)
		return (*this);

	id = d.id;
	states = d.states;
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

bool DFA::inDFA(pDFAState& state)
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
	for (pair<pair< pDFAState, char>, pDFAState> p : transitionTable)
	{
		cout << p.first.first->getId() << " <" << p.first.second << "> " << p.second->getId() << endl;
	}
}

void DFA::printTransitionTable(std::set<char> input)
{
	ofstream outFile;
	outFile.open("transitionTable.txt");

	outFile << "State |  ";
	for (char c : input)
	{
		outFile << c << "  |  ";
	}
	//outFile << endl;

	map<int, string> indexMap;

	indexMap.insert(pair<int, string>(1, "D")); // Dead State

	int index = 1;
	string StateName = "";

	for (pDFAState d : states)
	{
		StateName = "S" + std::to_string(index);
		pair<int, string> p(d->getId(), StateName);
		indexMap.insert(p);
		index++;
	}

	int Id = -1;

	for (pair<pair< pDFAState, char>, pDFAState> p : transitionTable)
	{
		int id = p.first.first->getId();
		string name = indexMap[id];

		if (Id != id)
		{
			Id = id;
			outFile << endl << name;
			for (int i = 0; i < 6 - name.size(); i++)
				outFile << " ";
			outFile << "|";
		}

		string name2 = indexMap[p.second->getId()];
		outFile << " " << name2;
		for (int i = 0; i < 4 - name2.size(); i++)
			outFile << " ";
		outFile << "|";
		//outFile << "<" << p.first.second << "> " << indexMap[p.second->getId()] << " | ";
	}

	outFile.close();
}
