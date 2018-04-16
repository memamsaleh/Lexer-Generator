#ifndef THOMPSONCONSTRUCTION_H
#define THOMPSONCONSTRUCTION_H

#include "automata.h"

typedef std::shared_ptr<Automata> pAutomata;

namespace ThompsonConstruction
{
	pAutomata symbol(const std::string input);             // One Symbol or empty expression
	pAutomata kColusre(pAutomata g);                       // Kleene Closure
	pAutomata pClosure(pAutomata g);                       // Positive Closure
	pAutomata concatenate(pAutomata g1, pAutomata g2);     // Concatenation
	pAutomata unionG(pAutomata g1, pAutomata g2);          // Union

}

#endif