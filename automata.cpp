#include "automata.h"
#include <iostream>
#include <queue>

Automata::Automata() { }

Automata::Automata(pState s, pState e) : start(s), end(e), size(2) { }

Automata::Automata(const Automata& a) : start(a.start), end(a.end), size(a.size) { }

Automata& Automata::operator= (const Automata& a)
{
	if (this == &a)
		return(*this);

	start = a.start;
	end = a.end;
	size = a.size;

	return (*this);
}

Automata::~Automata() {}

void Automata::print()
{
	std::queue<State> q;
	q.push(*start);
	q.push(*end);
	while (!q.empty())
	{
		State temp = q.front();
		q.pop();
		std::map<std::string, std::vector<pState>> tempC = temp.getChildren();
		for (std::map<std::string, std::vector<pState>>::iterator it = tempC.begin(); it != tempC.end(); ++it)
		{
			for (int i = 0; i < it->second.size(); i++)
			{
				q.push(*it->second[i]);
				std::cout << temp.getId() << "<" << it->first << ">" << it->second[i]->getId() << std::endl;
			}
		}

	}
	
}