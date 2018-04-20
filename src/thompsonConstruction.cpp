#include "thompsonConstruction.h"

using namespace ThompsonConstruction;

pAutomata ThompsonConstruction::symbol(const std::string input)
{
	pState s = std::make_shared<State>(State());
	pState e = std::make_shared<State>(State());

	s->addChild(e, input);
	pAutomata a = std::make_shared<Automata>(Automata(s, e));
	return a;
}

pAutomata ThompsonConstruction::kColusre(pAutomata a)
{
	pState s = std::make_shared<State>(State());
	pState e = std::make_shared<State>(State());

	s->addChild(a->start, "\\L");
	a->end->addChild(e, "\\L");

	a->end->addChild(a->start, "\\L");
	s->addChild(e, "\\L");

	pAutomata newA = std::make_shared<Automata>(Automata(s, e));
	newA->size = a->size + 2;
	return newA;
}

pAutomata ThompsonConstruction::pClosure(pAutomata a)
{
	pState s = std::make_shared<State>(State());
	pState e = std::make_shared<State>(State());

	s->addChild(a->start, "\\L");
	a->end->addChild(e, "\\L");

	a->end->addChild(a->start, "\\L");

	pAutomata newA = std::make_shared<Automata>(Automata(s, e));
	newA->size = a->size + 2;
	return newA;
}

pAutomata ThompsonConstruction::concatenate(pAutomata a1, pAutomata a2)
{
	a1->end->addChild(a2->start, "\\L");

	pAutomata a = std::make_shared<Automata>(Automata(a1->start, a2->end));
	a->size = a1->size + a2->size;
	return a;
}


pAutomata ThompsonConstruction::unionG(pAutomata a1, pAutomata a2)
{
	pState s = std::make_shared<State>(State());
	pState e = std::make_shared<State>(State());

	s->addChild(a1->start, "\\L");
	s->addChild(a2->start, "\\L");
	a1->end->addChild(e, "\\L");
	a2->end->addChild(e, "\\L");

	pAutomata a = std::make_shared<Automata>(Automata(s, e));
	a->size = a1->size + a2->size + 2;
	return a;
}