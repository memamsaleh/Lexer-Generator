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

	DFA& d = DFA::getDFA();
	d.createDFA(p, nfa.inputs);
	Lexer l("input.txt");

	Token t = l.getToken();
	while (t.lexeme != "")
	{
		std::cout << t.lexeme << " " << t.type << std::endl;
		t = l.getToken();
	}

	std::cin.get();
	return 0;
}