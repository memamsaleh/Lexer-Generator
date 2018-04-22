#include <iostream>
#include "automata.h"
#include "thompsonConstruction.h"
#include "nfa.h"
#include "dfa.h"
#include "lexer.h"

int main()
{
	NFA nfa;
	pAutomata p = nfa.getNFA("grammar.txt");

	//p->print();

	DFA& d = DFA::getDFA();
	d.createDFA(p, nfa.inputs);
	
	d.printTransitionTable(nfa.inputs);

	Lexer l("input.txt");

	std::cout << std::endl;
	std::cout << "   Lexeme   |   Token" << std::endl;
	std::cout << "   ---------|--------" << std::endl;

	Token t = l.getToken();
	while (t.lexeme != "")
	{
		std::cout << "   " << t.lexeme;
		for (int i = 0; i < 9 - t.lexeme.size(); i++)
			std::cout << " ";
		std::cout << "|   " << t.type << std::endl;
		t = l.getToken();
	}

	std::cin.get();
	return 0;
}