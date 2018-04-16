#include <iostream>
#include "automata.h"
#include "thompsonConstruction.h"
#include "nfa.h"

int main()
{
	//std::shared_ptr<State> s1(new State());
	//std::shared_ptr<State> s2(new State());
	//std::shared_ptr<State> s3(new State());
	//std::shared_ptr<State> e(new State());
	//pAutomata a = ThompsonConstruction::symbol("a");
	//a->print();
	//pAutomata b = ThompsonConstruction::symbol("b");
	//b->print();
	//a = ThompsonConstruction::concatenate(a, b);
	//a->print();

	//s1->addChild(s2, "a");
	//s2->addChild(s3, "b");
	//s1->addChild(s3, "a");


	//Automata a(s1, e);

	//a.print();
	NFA nfa;
	pAutomata p = nfa.getNFA("grammar.txt");

	//pAutomata p = RegexEvaluater::evaluate("a-z");
	//p->print();

	std::cin.get();
	return 0;
}