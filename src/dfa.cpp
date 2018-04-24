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
	//acceptancePriority = acceptance.top().first;
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
	deadState = make_shared<DFAState>(DFAState(set<pState>()));
	std::map<pair<pDFAState, char>, pDFAState> temp;

	for(char c : inputs)
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
		for(char c : inputs)
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

void DFA::printTransitionTable(const string& path)
{
	ofstream outFile;
	outFile.open(path);

	outFile << "State |  ";
	for (char c : inputs)
	{
		outFile << c << "  |  ";
	}
	//outFile << endl;

	map<int, string> indexMap;

	indexMap.insert(pair<int, string>(deadState->getId(), "D")); // Dead State

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

void DFA::minimize() 
{
	vector<set<pDFAState> > partitions;
	partitions.push_back(set<pDFAState>()); // Final States
	partitions.push_back(set<pDFAState>()); // Other States
	for (pDFAState state : states)
	{
		if (state->isAcceptance)
			partitions[0].insert(state);
		else
			partitions[1].insert(state);
	}

	bool changed = true;

	while (changed) 
	{
		changed = false;
		vector<set<pDFAState> > tempParts;
		for (set<pDFAState> s : partitions)
		{
			if (s.size() == 1)
			{
				tempParts.push_back(s);
				continue;
			}

			set<pDFAState> mainPart;
			set<pDFAState> otherPart;
			pDFAState first;
			for (pDFAState state : s)
			{
				if (mainPart.empty())
				{
					first = state;
					mainPart.insert(state);
					continue;
				}

				if (matchedNextStates(first, state, partitions))
					mainPart.insert(state);
				else
					otherPart.insert(state);
			}

			tempParts.push_back(mainPart);
			if(!otherPart.empty())
				tempParts.push_back(otherPart);
		}

		if (partitions.size() != tempParts.size())
		{
			changed = true;
			partitions = tempParts;
		}
	}

	replaceWithMinimized(partitions);
}

int DFA::getPartitionIndex(pDFAState d, const vector<set<pDFAState> >& partitions)
{
	int index = -1;

	for (int i = 0; i < partitions.size(); i++)
	{
		for (pDFAState s : partitions[i])
		{
			if (d->isEqualTo(s))
			{
				index = i;
				return index;
			}
		}
	}

	return index;
}

bool DFA::matchedNextStates(pDFAState d1, pDFAState d2, const vector<set<pDFAState> >& all)
{
	for (char c : inputs)
	{
		pDFAState next1 = d1->nextState(string(1, c));
		pDFAState next2 = d2->nextState(string(1, c));
		if (next1->isDead() && next2->isDead())
			continue;

		inDFA(next1);
		inDFA(next2);
		int id1 = getPartitionIndex(next1, all);
		int id2 = getPartitionIndex(next2, all);
		if (id1 != id2)
			return false;
		if (id1 == -1 || id2 == -1)
			return false;
	}

	return true;
}

void DFA::replaceWithMinimized(const vector<set<pDFAState> >& partitions)
{
	set<pDFAState> newStates;
	vector<vector<pair<char, int> > > indices;
	vector<pDFAState> nexts;
	//auto cmp = [](pair<int, string> left, pair<int, string> right) { return (left.first) < (right.first); };

	for (int i = 0; i < partitions.size(); i++)
	{
		//priority_queue<pair<int, string>, vector<pair<int, string> >, decltype(cmp)> acceptance(cmp);
		bool first = true;
		set<pState> partition;
		for (pDFAState s : partitions[i])
		{
			set<pState> temp = s->getStates();
			partition.insert(temp.begin(), temp.end());

			if (first)
			{
				vector<pair<char, int> > pairs;
				for (char c : inputs)
				{
					pair<char, int> indx;
					indx.first = c;
					pDFAState next = s->nextState(string(1, c));
					if (next->isDead())
						indx.second = -1;
					else
					{
						inDFA(next);
						indx.second = getPartitionIndex(next, partitions);
					}
					pairs.push_back(indx);
				}
				first = false;
				indices.push_back(pairs);
			}

			/*
			if (s->isAcceptance)
			{
				pair<int, string> p(s->acceptancePriority, s->acceptanceType);
				acceptance.push(p);
			}
			*/
		}
		pDFAState newState = make_shared<DFAState>(DFAState(partition));
		/*
		if (!acceptance.empty())
		{
			newState->acceptanceType = acceptance.top().second;
			newState->acceptancePriority = acceptance.top().first;
		}
		*/
		newStates.insert(newState);
		nexts.push_back(newState);

	}

	map<pair<pDFAState, char>, pDFAState> newTransitionTable;

	for (char c : inputs)
	{
		pair<pDFAState, char> key(deadState, c);
		pair<pair<pDFAState, char>, pDFAState> p(key, deadState);
		newTransitionTable.insert(p);
	}

	for (int i = 0; i < partitions.size(); i++)
	{
		vector<pair<char, int> > pairs = indices[i];
		for (int j = 0; j < pairs.size(); j++)
		{
			pair<pDFAState, char> key(nexts[i], pairs[j].first);
			int indx = pairs[j].second;

			if(indx == -1)
				newTransitionTable.insert(pair<pair<pDFAState, char>, pDFAState>(key, deadState));
			else
				newTransitionTable.insert(pair<pair<pDFAState, char>, pDFAState>(key, nexts[indx]));
		}

	}

	start = nexts[getPartitionIndex(start, partitions)];
	states = newStates;
	transitionTable = newTransitionTable;
}