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
	std::queue<pState> q;
	q.push(start);
	q.push(end);
	std::set<pState> visited;
	while (!q.empty())
	{
		pState temp = q.front();
		visited.insert(temp);
		q.pop();
		std::map<std::string, std::set<pState> > tempC = temp->getChildren();
		for(std::pair<std::string, std::set<pState> > p : tempC)
		{
			for (pState s : p.second)
			{
				//if (std::find(visited.begin(), visited.end(), s) == visited.end())
				if(!inSet(s, visited))
				{
					q.push(s);
					visited.insert(s);
					std::cout << temp->getId() << "<" << p.first << ">" << s->getId() << std::endl;
				}
			}
		}

	}
}