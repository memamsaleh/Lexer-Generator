#include "state.h"
#include <queue>

unsigned int sid = 0;

State::State() : acceptancePriority(-1), isAcceptance(false), acceptanceType("")
{
	//static unsigned int sid = 0;
	id = sid++;
}

State::State(const State& n) : acceptancePriority(n.acceptancePriority), isAcceptance(n.isAcceptance), acceptanceType(n.acceptanceType), id(n.id), children(n.children) {}

State::~State() {}

State& State::operator= (const State& n)
{
	if (this == &n)
		return (*this);

	id = n.id;
	isAcceptance = n.isAcceptance;
	acceptanceType = n.acceptanceType;
	acceptancePriority = n.acceptancePriority;
	children = n.children;

	return (*this);
}

void State::addChild(pState n, const std::string& val)
{
	std::map<std::string, std::set<pState>>::iterator itr = children.find(val);
	if (itr != children.end())
	{
		children[val].insert(n);
	}
	else
	{
		std::set<pState> v;
		v.insert(n);
		std::pair<std::string, std::set<pState>> p(val, v);
		children.insert(p);
	}
}

std::set<pState> State::getInputClosure(const std::string& val)
{
	if (val != "\\L")
		return children[val];

	/*
	std::vector<pState> closure;
	closure.push_back(std::make_shared<State>(*this));
	std::vector<pState> visited;
	for (std::size_t i = 0; i < closure.size(); i++)
	{
		if (!(std::find(visited.begin(), visited.end(), closure[i]) != visited.end()))
		{
			visited.push_back(closure[i]);
			std::set<pState> inputchildren;
			std::map<std::string, std::set<pState>>::iterator it = closure[i]->children.find(val);
			if(it != closure[i]->children.end())
				inputchildren = closure[i]->children[val];
			if (!inputchildren.empty())
				closure.insert(closure.end(), inputchildren.begin(), inputchildren.end());
		}
	}

	return std::set<pState>(visited.begin(), visited.end());
	*/
	std::set<pState> closure;
	std::queue<pState> q;
	pState thisState = std::make_shared<State>(*this);
	closure.insert(thisState);
	q.push(thisState);
	while (!q.empty())
	{
		pState s = q.front();
		q.pop();
		std::set<pState> childs;
		std::map<std::string, std::set<pState>>::iterator it = s->children.find(val);
		if (it != s->children.end())
			childs = s->children[val];
		std::set<pState>::iterator itr = childs.begin();
		for (itr; itr != childs.end(); ++itr)
		{
			if (std::find(closure.begin(), closure.end(), *itr) == closure.end())
			{
				closure.insert(*itr);
				q.push(*itr);
			}
		}
	}
	return closure;
}