#include "state.h"

State::State() : acceptancePriority(-1), isAcceptance(false), acceptanceType("")
{
	static unsigned int sid = 0;
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
	std::map<std::string, std::vector<pState>>::iterator itr = children.find(val);
	if (itr != children.end())
	{
		children[val].push_back(n);
	}
	else
	{
		std::vector<pState> v;
		v.push_back(n);
		std::pair<std::string, std::vector<pState>> p(val, v);
		children.insert(p);
	}
}

std::vector<pState> State::getChildren(const std::string& val)
{
	return children[val];
}