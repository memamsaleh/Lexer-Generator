#ifndef AUTOMATA_H
#define AUTOMATA_H

#include "state.h"

class Automata
{
public:
	int size;
	pState start;
	pState end;

	Automata();
	Automata(pState s, pState e);
	Automata(const Automata& g);
	~Automata();

	Automata& operator= (const Automata& g);

	void print();

};


#endif